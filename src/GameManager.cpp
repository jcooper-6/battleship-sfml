#include "GameManager.h"

// Initialize all members in the constructor
GameManager::GameManager()
    : player_board(ROWS, COLS),
    ai_board(ROWS, COLS),
    leaderboard("leaderboard.txt"),
    game_over(false),
    total_game_time(0.0f),
    final_accuracy(0.0f),
    // Calculate UI positions dynamically based on constants
    game_timer(((COLS* GRID_SIZE * 2) + 64) / 2.0f - 31.5f, static_cast<float>(ROWS* GRID_SIZE)),
    p_ships_counter(20.0f, static_cast<float>(ROWS* GRID_SIZE)),
    ai_ships_counter(((COLS* GRID_SIZE * 2) + 64) - 83.0f, static_cast<float>(ROWS* GRID_SIZE)),
    reset_button(((COLS* GRID_SIZE * 2) + 64) / 2.0f - 32.0f, static_cast<float>(ROWS* GRID_SIZE) + 40.0f, "face_happy"),
    ocean_bg(TextureManager::GetTexture("ocean_bg"))
{
    // Window Setup (SFML 3.0)
    int screen_width = (COLS * GRID_SIZE * 2) + 64;
    int screen_height = (ROWS * GRID_SIZE) + 100;

    window.create(sf::VideoMode({ static_cast<unsigned int>(screen_width), static_cast<unsigned int>(screen_height) }),
        "Battleship SFML",
        sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    if (!font.openFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!\n";
    }

    resetGame();
}

void GameManager::resetGame() {
    // Reinitialize boards and reset states
    player_board = Board(ROWS, COLS);
    ai_board = Board(ROWS, COLS);

    player_board.createBoard(0.0f, 0.0f);
    ai_board.createBoard(static_cast<float>((COLS + 2) * GRID_SIZE), 0.0f);

    player_board.placeShips();
    ai_board.placeShips();

    game_over = false;
    total_game_time = 0.0f;
    game_timer.reset();
    reset_button.change_texture("face_happy");
    leaderboard.Reset();
}

void GameManager::run() {
    while (window.isOpen()) {
        float dt = delta_clock.restart().asSeconds();
        if (!game_over) {
            total_game_time += dt;
        }

        processEvents();
        update(dt);
        render();
    }
}

void GameManager::processEvents() {
    // Get mouse position converted to grid coordinates
    sf::Vector2i raw_mouse = sf::Mouse::getPosition(window);
    sf::Vector2f exact_mouse_pos(static_cast<float>(raw_mouse.x), static_cast<float>(raw_mouse.y));
    sf::Vector2f mouse_pos_grid(exact_mouse_pos.x / GRID_SIZE, exact_mouse_pos.y / GRID_SIZE);

    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        // Intercept typing for leaderboard name entry
        if (leaderboard.IsEnteringName()) {
            leaderboard.HandleInput(*event, total_game_time, final_accuracy);
            continue;
        }

        if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseEvent->button == sf::Mouse::Button::Left) {

                // 1. Check Reset Button
                if (reset_button.is_clicked(exact_mouse_pos)) {
                    resetGame();
                }

                // 2. Check AI Grid Clicks
                if (isMouseInAIGrid(mouse_pos_grid) && !game_over && !leaderboard.IsShowingScores()) {

                    int ai_grid_offset = COLS + 2;
                    int grid_x = static_cast<int>(mouse_pos_grid.x) - ai_grid_offset;
                    int grid_y = static_cast<int>(mouse_pos_grid.y);

                    Tile& target = ai_board.Get_Tiles()[grid_y][grid_x];

                    if (!target.revealed) {
                        // Player Attack
                        ai_board.Attack_Tile(grid_y, grid_x);

                        // Check Player Win Condition
                        if (ai_board.Get_Sunk_Ships() == SHIP_COUNT) {
                            std::cout << "Player Wins!\n";
                            game_over = true;
                            reset_button.change_texture("face_win");
                            final_accuracy = static_cast<float>(SHIP_COUNT) / ai_board.Get_Attacks_Made();
                            leaderboard.StartNameEntry();
                        }

                        // Trigger AI Turn if game isn't over
                        if (!game_over) {
                            executeAITurn();
                        }
                    }
                }
            }
        }
    }
}

void GameManager::executeAITurn() {
    bool ai_fired = false;
    while (!ai_fired) {
        int ai_row = Random::Int(0, ROWS - 1);
        int ai_col = Random::Int(0, COLS - 1);
        Tile& p_target = player_board.Get_Tiles()[ai_row][ai_col];

        if (!p_target.revealed) {
            player_board.Attack_Tile(ai_row, ai_col);
            ai_fired = true;

            if (player_board.Get_Sunk_Ships() == SHIP_COUNT) {
                std::cout << "AI Wins!\n";
                game_over = true;
                reset_button.change_texture("face_lose");
            }
        }
    }
}

bool GameManager::isMouseInAIGrid(sf::Vector2f mouse_pos_grid) {
    int x_offset = COLS + 2;
    return (mouse_pos_grid.x >= x_offset &&
        mouse_pos_grid.x < COLS + x_offset &&
        mouse_pos_grid.y >= 0 &&
        mouse_pos_grid.y < ROWS);
}

void GameManager::update(float dt) {
    if (!game_over) {
        game_timer.update(dt);
        if (game_timer.is_game_over()) {
            std::cout << "Time's up! AI Wins!\n";
            game_over = true;
            reset_button.change_texture("face_lose");
        }
    }
}

void GameManager::render() {
    window.clear();

    // Layer 1: Background
    window.draw(ocean_bg);

    // Layer 2: Grid Lines
    for (auto& row : player_board.Get_Tiles()) {
        for (auto& tile : row) window.draw(tile.tile_sprite);
    }
    for (auto& row : ai_board.Get_Tiles()) {
        for (auto& tile : row) window.draw(tile.tile_sprite);
    }

    // Layer 3: Ships
    for (const auto& ship : player_board.Get_Visual_Ships()) {
        window.draw(ship.sprite);
    }
    for (const auto& ship : ai_board.Get_Visual_Ships()) {
        if (ship.is_revealed || game_over) window.draw(ship.sprite);
    }

    // Layer 4: Hit/Miss Overlays
    for (auto& row : player_board.Get_Tiles()) {
        for (auto& tile : row) {
            if (tile.revealed) window.draw(tile.overlay_sprite);
        }
    }
    for (auto& row : ai_board.Get_Tiles()) {
        for (auto& tile : row) {
            if (tile.revealed) window.draw(tile.overlay_sprite);
        }
    }

    // Layer 5: UI Components
    for (const auto& sprite : game_timer.get_sprites()) window.draw(sprite);

    int player_ships_left = SHIP_COUNT - player_board.Get_Sunk_Ships();
    for (const auto& sprite : p_ships_counter.get_sprites(player_ships_left)) window.draw(sprite);

    int ai_ships_left = SHIP_COUNT - ai_board.Get_Sunk_Ships();
    for (const auto& sprite : ai_ships_counter.get_sprites(ai_ships_left)) window.draw(sprite);

    window.draw(reset_button.get_sprite());
    leaderboard.Draw(window, font);

    window.display();
}