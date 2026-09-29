#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Tile
{
private:
    bool has_ship;
    int ship_id;

public:
    bool revealed;
    sf::Sprite tile_sprite;
    sf::Sprite overlay_sprite;

    Tile();

    void SetShip(int id);
    bool GetShipStatus() const;
    int GetShipId() const;

    void SetTexture(std::string name);
    void OverlayTexture(std::string name);
};