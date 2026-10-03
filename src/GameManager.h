#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include "board.h"
#include "timer.h"
#include "counter.h"
#include "button.h"
#include "leaderboard.h"
#include "TextureManager.h"
#include "random.h"

class GameManager {
private:
    // --- Global Game Constants ---
    const int GRID_SIZE = 32;
    const int COLS = 10;
    const int ROWS = 10;
    const int SHIP_COUNT = 5;

    // --- Core Window & Timing ---
    sf::RenderWindow window;
    sf::Clock delta_clock;

    // --- Game State ---
    bool game_over;
    float total_game_time;
    float final_accuracy;

    // --- Entities ---
    Board player_board;
    Board ai_board;

    // --- UI Components ---
    sf::Sprite ocean_bg;
    sf::Font font;
    Timer game_timer;
    Counter p_ships_counter;
    Counter ai_ships_counter;
    Button reset_button;
    Leaderboard leaderboard;

    // --- Private Helper Methods ---
    void processEvents();
    void update(float dt);
    void render();
    void resetGame();
    void executeAITurn();
    bool isMouseInAIGrid(sf::Vector2f mouse_pos_grid);

public:
    GameManager();
    void run();
};