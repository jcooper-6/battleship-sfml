#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
private:
    sf::Sprite sprite;

public:
    Button(float x, float y, const std::string& texture_name);

    bool is_clicked(sf::Vector2f mouse_pos) const;
    void change_texture(const std::string& texture_name);
    sf::Sprite& get_sprite();
};