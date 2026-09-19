#include <SFML/Graphics.hpp> 
#include <iostream>
#include <optional>
#include "TextureManager.h"
#include "random.h"
#include "board.h"
#include "timer.h"
#include "counter.h"
#include "button.h"

static bool mouse_in_bounds(int rows, int cols, sf::Vector2f mouse_pos_grid, int x_offset_grid)
{
    if (mouse_pos_grid.x >= x_offset_grid && mouse_pos_grid.x < cols + x_offset_grid && mouse_pos_grid.y >= 0 && mouse_pos_grid.y < rows)
        return true;
    return false;
}

static void update_mouse_pos(sf::Vector2f& mouse_pos_grid, sf::Window& window, int grid_size)
{
    sf::Vector2i raw_mouse_pos = sf::Mouse::getPosition(window);
    mouse_pos_grid.x = static_cast<float>(raw_mouse_pos.x) / grid_size;
    mouse_pos_grid.y = static_cast<float>(raw_mouse_pos.y) / grid_size;
}

void initialize_boards(Board& p_board, Board& a_board, int cols, int grid_size) {
    p_board.createBoard(0.0f, 0.0f);
    a_board.createBoard(static_cast<float>((cols + 2) * grid_size), 0.0f);
    p_board.placeShips();
    a_board.placeShips();

    for (auto& row : p_board.Get_Tiles()) {
        for (auto& tile : row) {
            if (tile.GetShipStatus()) {
                tile.OverlayTexture("ship");
            }
        }
    }
}

int main()
{
    int grid_size = 32;
    int cols = 10;
    int rows = 10;
    int ship_count = 10;

    bool game_over = false;
    sf::Vector2f mouse_pos_grid;

    int screen_width = (cols * 32 * 2) + 64;
    int screen_height = (rows * 32) + 100;

    Board player_board(rows, cols, ship_count);
    Board ai_board(rows, cols, ship_count);

    initialize_boards(player_board, ai_board, cols, grid_size);

    float ui_y = static_cast<float>(rows * grid_size);

    Timer game_timer(screen_width / 2.0f - 31.5f, ui_y);// Center Timer
    Counter p_ships_counter(20.0f, ui_y);                // Player ships remaining (left)
    Counter ai_ships_counter(screen_width - 83.0f, ui_y); // AI ships remaining (right)
    Button reset_button(screen_width / 2.0f - 32.0f, ui_y + 40.0f, "face_happy"); // Below timer

    sf::Clock delta_clock;

    // SFML 3.0 VideoMode constructor requires a Vector2u
    sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(screen_width), static_cast<unsigned int>(screen_height) }), "Battleship SFML");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        // Calculate delta time for the Timer
        float delta_time = delta_clock.restart().asSeconds();

        update_mouse_pos(mouse_pos_grid, window, grid_size);

        // SFML 3.0 uses std::optional for polling events
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            // SFML 3.0 event type checking
            if (event->is<sf::Event::Closed>())
                window.close();

            // Extract MouseButtonPressed data safely using getIf<>
            if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
            {
                // SFML 3.0 uses scoped enums
                if (mouseEvent->button == sf::Mouse::Button::Left)
                {
                    // Check Reset Button Clicks
                    sf::Vector2i raw_mouse = sf::Mouse::getPosition(window);
                    sf::Vector2f exact_mouse_pos(static_cast<float>(raw_mouse.x), static_cast<float>(raw_mouse.y));

                    if (reset_button.is_clicked(exact_mouse_pos))
                    {
                        // Reset everything
                        player_board = Board(rows, cols, ship_count);
                        ai_board = Board(rows, cols, ship_count);
                        initialize_boards(player_board, ai_board, cols, grid_size);

                        game_over = false;
                        game_timer.reset();
                        reset_button.change_texture("face_happy");
                    }

                    // Check Grid Clicks
                    int ai_grid_offset = cols + 2;

                    if (mouse_in_bounds(rows, cols, mouse_pos_grid, ai_grid_offset) && !game_over)
                    {
                        int grid_x = static_cast<int>(mouse_pos_grid.x) - ai_grid_offset;
                        int grid_y = static_cast<int>(mouse_pos_grid.y);

                        Tile& target = ai_board.Get_Tiles()[grid_y][grid_x];

                        if (!target.revealed)
                        {
                            // 1. Player Attack
                            ai_board.Attack_Tile(&target);

                            if (ai_board.Get_Hits_Landed() == ship_count) {
                                std::cout << "Player Wins!\n";
                                game_over = true;
                                reset_button.change_texture("face_win");
                            }

                            // 2. AI Random Attack Turn
                            if (!game_over) {
                                bool ai_fired = false;
                                while (!ai_fired)
                                {
                                    int ai_row = Random::Int(0, rows - 1);
                                    int ai_col = Random::Int(0, cols - 1);
                                    Tile& p_target = player_board.Get_Tiles()[ai_row][ai_col];

                                    if (!p_target.revealed) {
                                        player_board.Attack_Tile(&p_target);
                                        ai_fired = true;

                                        if (player_board.Get_Hits_Landed() == ship_count) {
                                            std::cout << "AI Wins!\n";
                                            game_over = true;
                                            reset_button.change_texture("face_lose");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // --- TIMER UPDATE ---
        if (!game_over)
        {
            game_timer.update(delta_time);
            if (game_timer.is_game_over())
            {
                std::cout << "Time's up! AI Wins!\n";
                game_over = true;
                reset_button.change_texture("face_lose");
            }
        }

        // --- Rendering ---

        window.clear(sf::Color(200, 200, 200));

        for (auto& row : player_board.Get_Tiles()) {
            for (auto& tile : row) {
                window.draw(tile.tile_sprite);
                window.draw(tile.overlay_sprite);
            }
        }

        for (auto& row : ai_board.Get_Tiles()) {
            for (auto& tile : row) {
                window.draw(tile.tile_sprite);
                window.draw(tile.overlay_sprite);
            }
        }

        // Draw Timer
        for (const auto& sprite : game_timer.get_sprites()) {
            window.draw(sprite);
        }

        // Draw Counters (Math: Total Ships - Hits Landed = Ships left to destroy)
        int player_ships_left = ship_count - player_board.Get_Hits_Landed();
        for (const auto& sprite : p_ships_counter.get_sprites(player_ships_left)) {
            window.draw(sprite);
        }

        int ai_ships_left = ship_count - ai_board.Get_Hits_Landed();
        for (const auto& sprite : ai_ships_counter.get_sprites(ai_ships_left)) {
            window.draw(sprite);
        }

        // Draw Reset Button
        window.draw(reset_button.get_sprite());

        window.display();
    }
    return 0;
}