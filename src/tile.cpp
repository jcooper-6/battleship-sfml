#include "tile.h"
#include "TextureManager.h" 

// SFML 3.0 requires sprites to be initialized with a texture immediately via an initializer list
Tile::Tile() :
    tile_sprite(TextureManager::GetTexture("water")),
    overlay_sprite(TextureManager::GetTexture("water"))
{
    has_ship = false;
    revealed = false;
}

void Tile::SetShip()
{
    this->has_ship = true;
}

bool Tile::GetShipStatus() const
{
    return has_ship;
}

void Tile::SetTexture(std::string name)
{
    tile_sprite.setTexture(TextureManager::GetTexture(name));
}

void Tile::OverlayTexture(std::string name)
{
    overlay_sprite.setTexture(TextureManager::GetTexture(name));
}