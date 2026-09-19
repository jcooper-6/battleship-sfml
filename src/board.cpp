#include "board.h"
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include "random.h"

Board::Board(int rows, int cols, int ships)
{
    revealed_tiles = 0;
    this->rows = rows;
    this->cols = cols;
    this->ships = ships;
    hits_landed = 0;
}

// We could use an AI or better algo for this instead of random
void Board::placeShips()
{
    int ships_added = 0;
    while (ships_added < ships)
    {
        int row = Random::Int(0, rows - 1);
        int col = Random::Int(0, cols - 1);
        if (!tiles[row][col].GetShipStatus())
        {
            tiles[row][col].SetShip();
            ships_added++;
        }
    }
}

void Board::createBoard(float x_offset, float y_offset)
{
    tiles.clear();
    this->revealed_tiles = 0;
    this->hits_landed = 0;

    for (int i = 0; i < rows; i++)
    {
        std::vector<Tile> current_row;
        tiles.push_back(current_row);
        for (int j = 0; j < cols; j++)
        {
            Tile t;
            tiles[i].push_back(t);
        }
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            float x_pos = (j * 32.0f) + x_offset;
            float y_pos = (i * 32.0f) + y_offset;

            // SFML 3.0 requires Vector2f rather than (float, float)
            this->tiles[i][j].tile_sprite.setPosition({ x_pos, y_pos });
            this->tiles[i][j].overlay_sprite.setPosition({ x_pos, y_pos });
        }
    }
}

std::vector<std::vector<Tile>>& Board::Get_Tiles()
{
    return this->tiles;
}

void Board::Attack_Tile(Tile* tile)
{
    if (!tile->revealed)
    {
        tile->revealed = true;
        revealed_tiles++;

        if (tile->GetShipStatus())
        {
            tile->OverlayTexture("hit");
            hits_landed++;
        }
        else
        {
            tile->OverlayTexture("miss");
        }
    }
}

int Board::Get_Hits_Landed() const
{
    return hits_landed;
}