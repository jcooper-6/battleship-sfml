#include "leaderboard.h"
#include <iostream>

Leaderboard::Leaderboard(const std::string& file) : filename(file) {
    is_entering_name = false;
    is_showing_scores = false;
    LoadScores();
}

void Leaderboard::LoadScores() {
    scores.clear();
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string name;
        float time, accuracy;
        while (file >> name >> time >> accuracy) {
            scores.push_back({ name, time, accuracy });
        }
        file.close();
    }
}

void Leaderboard::SaveScores() {
    // Sort: lowest time first, then highest accuracy
    std::sort(scores.begin(), scores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        if (a.time == b.time) return a.accuracy > b.accuracy;
        return a.time < b.time;
        });

    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& score : scores) {
            file << score.name << " " << score.time << " " << score.accuracy << "\n";
        }
        file.close();
    }
}

void Leaderboard::StartNameEntry() {
    is_entering_name = true;
    is_showing_scores = false;
    current_input_name = "";
}

void Leaderboard::HandleInput(const sf::Event& event, float time, float accuracy) {
    if (!is_entering_name) return;

    if (const auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
        char unicode = static_cast<char>(textEvent->unicode);

        // Backspace
        if (unicode == '\b' && !current_input_name.empty()) {
            current_input_name.pop_back();
        }
        // Enter Key
        else if (unicode == '\r' || unicode == '\n') {
            if (current_input_name.empty()) current_input_name = "Anonymous";
            scores.push_back({ current_input_name, time, accuracy });
            SaveScores();
            is_entering_name = false;
            is_showing_scores = true;
        }
        // Printable ASCII (Excluding spaces for simpler text parsing)
        else if (unicode >= 33 && unicode < 127 && current_input_name.size() < 12) {
            current_input_name += unicode;
        }
    }
}

void Leaderboard::Draw(sf::RenderWindow& window, sf::Font& font) const {
    if (!is_entering_name && !is_showing_scores) return;

    // Semi-transparent dark overlay
    sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    if (is_entering_name) {
        sf::Text promptText(font, "You Won!\nEnter Name: " + current_input_name + "_", 30);
        promptText.setFillColor(sf::Color::White);
        promptText.setPosition({ window.getSize().x / 2.0f - promptText.getGlobalBounds().size.x / 2.0f, window.getSize().y / 2.0f - 50.0f });
        window.draw(promptText);
    }
    else if (is_showing_scores) {
        sf::Text title(font, "LEADERBOARD", 40);
        title.setFillColor(sf::Color::Yellow);
        title.setPosition({ window.getSize().x / 2.0f - title.getGlobalBounds().size.x / 2.0f, 50.0f });
        window.draw(title);

        float y_offset = 120.0f;
        for (size_t i = 0; i < std::min(scores.size(), size_t(8)); ++i) {
            std::ostringstream oss;
            oss << i + 1 << ". " << scores[i].name
                << "  |  " << std::fixed << std::setprecision(1) << scores[i].time << "s"
                << "  |  " << std::fixed << std::setprecision(0) << (scores[i].accuracy * 100.0f) << "%";

            sf::Text scoreText(font, oss.str(), 24);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition({ window.getSize().x / 2.0f - scoreText.getGlobalBounds().size.x / 2.0f, y_offset });
            window.draw(scoreText);
            y_offset += 40.0f;
        }
    }
}

void Leaderboard::Reset() {
    is_entering_name = false;
    is_showing_scores = false;
}