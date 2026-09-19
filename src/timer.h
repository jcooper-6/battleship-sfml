#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Timer {
private:
    float pos_x;
    float pos_y;
    float time_remaining;

public:
    Timer(float x, float y);

    void update(float delta_time);
    bool is_game_over() const;
    void reset();
    std::vector<sf::Sprite> get_sprites();
};