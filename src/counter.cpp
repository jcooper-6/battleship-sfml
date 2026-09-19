#include "Counter.h"
#include "TextureManager.h"

Counter::Counter(float x, float y) : pos_x(x), pos_y(y) {}

std::vector<sf::Sprite> Counter::get_sprites(int ships_remaining) {
    std::vector<sf::Sprite> sprites;

    const sf::Texture& digits_tex = TextureManager::GetTexture("digits");
    sf::Sprite first_num(digits_tex);
    sf::Sprite second_num(digits_tex);
    sf::Sprite third_num(digits_tex);

    first_num.setPosition({ pos_x, pos_y });
    second_num.setPosition({ pos_x + 21.0f, pos_y });
    third_num.setPosition({ pos_x + 42.0f, pos_y });

    int count = ships_remaining;

    if (count < 0) count = 0;

    int first_digit = count / 100;
    int second_digit = (count / 10) % 10;
    int third_digit = count % 10;

    first_num.setTextureRect(sf::IntRect({ first_digit * 21, 0 }, { 21, 32 }));
    second_num.setTextureRect(sf::IntRect({ second_digit * 21, 0 }, { 21, 32 }));
    third_num.setTextureRect(sf::IntRect({ third_digit * 21, 0 }, { 21, 32 }));

    sprites.push_back(first_num);
    sprites.push_back(second_num);
    sprites.push_back(third_num);

    return sprites;
}