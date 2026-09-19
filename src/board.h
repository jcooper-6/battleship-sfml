#pragma once
#include <vector>
#include "Tile.h"

class Board
{
private:
    int rows;
    int cols;
    int ships;
    int revealed_tiles;
    int hits_landed;
    std::vector<std::vector<Tile>> tiles;

public:
    Board(int rows, int cols, int ships);

    void placeShips();
    void createBoard(float x_offset, float y_offset);

    std::vector<std::vector<Tile>>& Get_Tiles();
    void Attack_Tile(Tile* tile);
    int Get_Hits_Landed() const;
};