#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Tile
{
private:
    bool has_ship;

public:
    bool revealed;
    sf::Sprite tile_sprite;
    sf::Sprite overlay_sprite;

    Tile();

    void SetShip();
    bool GetShipStatus() const;

    void SetTexture(std::string name);
    void OverlayTexture(std::string name);
};