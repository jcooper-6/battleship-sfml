#pragma once
#include <vector>
#include <string>
#include "tile.h"


struct VisualShip {
    int id;
    bool is_revealed;
    sf::Sprite sprite;

    VisualShip(int ship_id, const sf::Texture& tex) : id(ship_id), is_revealed(false), sprite(tex) {}
};

class Board
{
private:
    int rows;
    int cols;
    int revealed_tiles;
    int hits_landed;
    int sunk_ships;
    int attacks_made;
    std::vector<std::vector<Tile>> tiles;
    std::vector<VisualShip> visual_ships;

    void Attack_Tile_Cascade(int r, int c, int ship_id);

public:
    Board(int rows, int cols);

    void placeShips();
    void createBoard(float x_offset, float y_offset);

    std::vector<std::vector<Tile>>& Get_Tiles();
    std::vector<VisualShip>& Get_Visual_Ships();

    void Attack_Tile(int r, int c);

    int Get_Hits_Landed() const;
    int Get_Revealed_Tiles() const;
    int Get_Sunk_Ships() const;
    int Get_Attacks_Made() const;
};