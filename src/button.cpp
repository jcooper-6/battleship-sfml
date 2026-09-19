#include "Button.h"
#include "TextureManager.h"

// Initialize the sf::Sprite member with its required texture via the initializer list
Button::Button(float x, float y, const std::string& texture_name)
    : sprite(TextureManager::GetTexture(texture_name))
{
    sprite.setPosition({ x, y });
}

bool Button::is_clicked(sf::Vector2f mouse_pos) const {
    return sprite.getGlobalBounds().contains(mouse_pos);
}

void Button::change_texture(const std::string& texture_name) {
    sprite.setTexture(TextureManager::GetTexture(texture_name));
}

sf::Sprite& Button::get_sprite() {
    return sprite;
}