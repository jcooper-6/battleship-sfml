#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Counter {
private:
    float pos_x;
    float pos_y;

public:
    Counter(float x, float y);
    std::vector<sf::Sprite> get_sprites(int ships_remaining);
};