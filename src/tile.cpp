#include "tile.h"
#include "TextureManager.h" 

Tile::Tile() :
    tile_sprite(TextureManager::GetTexture("grid")),
    overlay_sprite(TextureManager::GetTexture("grid"))
{
    has_ship = false;
    revealed = false;
    ship_id = -1;
}

void Tile::SetShip(int id)
{
    this->has_ship = true;
    this->ship_id = id;
}

bool Tile::GetShipStatus() const
{
    return has_ship;
}

int Tile::GetShipId() const
{
    return ship_id;
}

void Tile::SetTexture(std::string name)
{
    tile_sprite.setTexture(TextureManager::GetTexture(name));
}

void Tile::OverlayTexture(std::string name)
{
    overlay_sprite.setTexture(TextureManager::GetTexture(name));
}