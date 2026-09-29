#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>

struct ScoreEntry {
    std::string name;
    float time;
    float accuracy;
};

class Leaderboard {
private:
    std::vector<ScoreEntry> scores;
    std::string current_input_name;
    bool is_entering_name;
    bool is_showing_scores;
    std::string filename;

    void LoadScores();
    void SaveScores();

public:
    Leaderboard(const std::string& file = "leaderboard.txt");

    void StartNameEntry();
    void HandleInput(const sf::Event& event, float time, float accuracy);
    void Draw(sf::RenderWindow& window, sf::Font& font) const;

    bool IsEnteringName() const { return is_entering_name; }
    bool IsShowingScores() const { return is_showing_scores; }
    void Reset();
};