#include "timer.h"
#include "TextureManager.h"

Timer::Timer(float x, float y) : pos_x(x), pos_y(y), time_remaining(600.0f) {}

void Timer::update(float delta_time) {
    if (time_remaining > 0) {
        time_remaining -= delta_time;
        if (time_remaining < 0) {
            time_remaining = 0;
        }
    }
}

bool Timer::is_game_over() const {
    return time_remaining <= 0;
}

void Timer::reset() {
    time_remaining = 600.0f;
}

std::vector<sf::Sprite> Timer::get_sprites() {
    std::vector<sf::Sprite> sprites;
    int count = static_cast<int>(time_remaining);

    if (count > 999) count = 999;

    // SFML 3.0 requires passing the texture directly to the Sprite constructor
    const sf::Texture& digits_tex = TextureManager::GetTexture("digits");
    sf::Sprite first_num(digits_tex);
    sf::Sprite second_num(digits_tex);
    sf::Sprite third_num(digits_tex);

    // SFML 3.0 requires a single sf::Vector2f via {} 
    first_num.setPosition({ pos_x, pos_y });
    second_num.setPosition({ pos_x + 21.0f, pos_y });
    third_num.setPosition({ pos_x + 42.0f, pos_y });

    int first_digit = count / 100;
    int second_digit = (count / 10) % 10;
    int third_digit = count % 10;

    // SFML 3.0 requires {{x, y}, {width, height}} for IntRect
    first_num.setTextureRect(sf::IntRect({ first_digit * 21, 0 }, { 21, 32 }));
    second_num.setTextureRect(sf::IntRect({ second_digit * 21, 0 }, { 21, 32 }));
    third_num.setTextureRect(sf::IntRect({ third_digit * 21, 0 }, { 21, 32 }));

    sprites.push_back(first_num);
    sprites.push_back(second_num);
    sprites.push_back(third_num);

    return sprites;
}