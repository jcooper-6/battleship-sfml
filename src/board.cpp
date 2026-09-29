#include "board.h"
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include "random.h"

Board::Board(int rows, int cols)
{
    revealed_tiles = 0;
    this->rows = rows;
    this->cols = cols;
    hits_landed = 0;
    sunk_ships = 0;
    attacks_made = 0;
}

void Board::placeShips()
{
    std::vector<int> ship_lengths = { 5, 4, 3, 3, 2 };
    std::vector<std::string> ship_textures = { "ship_xlarge", "ship_large", "ship_medium1", "ship_medium2", "ship_small" };
    int current_id = 0;

    for (int length : ship_lengths)
    {
        bool placed = false;
        while (!placed)
        {
            int r = Random::Int(0, rows - 1);
            int c = Random::Int(0, cols - 1);
            bool horizontal = Random::Int(0, 1) == 0;

            // Check grid bounds

            if (horizontal && c + length > cols) continue;
            if (!horizontal && r + length > rows) continue;

            // Check for overlapping with another ship
            bool overlap = false;
            for (int i = 0; i < length; i++)
            {
                int check_r = r + (horizontal ? 0 : i);
                int check_c = c + (horizontal ? i : 0);
                if (tiles[check_r][check_c].GetShipStatus())
                {
                    overlap = true;
                    break;
                }
            }

            if (!overlap)
            {
                // 1. Set tiles as a ship
                for (int i = 0; i < length; i++)
                {
                    int place_r = r + (horizontal ? 0 : i);
                    int place_c = c + (horizontal ? i : 0);
                    tiles[place_r][place_c].SetShip(current_id);
                }

                VisualShip vs(current_id, TextureManager::GetTexture(ship_textures[current_id]));

                // Fetch the grid offsets for this board
                float board_x_offset = tiles[0][0].tile_sprite.getPosition().x;
                float board_y_offset = tiles[0][0].tile_sprite.getPosition().y;

                float x_pos = (c * 32.0f) + board_x_offset;
                float y_pos = (r * 32.0f) + board_y_offset;

                if (horizontal) {
                    vs.sprite.setPosition({ x_pos, y_pos });
                    vs.sprite.setRotation(sf::degrees(0));
                }
                else {
                    // Offset X by 32 pixels so it pivots downward into the correct column
                    vs.sprite.setPosition({ x_pos + 32.0f, y_pos });
                    vs.sprite.setRotation(sf::degrees(90));
                }

                visual_ships.push_back(vs);
                current_id++;
                placed = true;
            }
        }
    }
}

void Board::createBoard(float x_offset, float y_offset)
{
    tiles.clear();
    visual_ships.clear();
    this->revealed_tiles = 0;
    this->hits_landed = 0;
    this->sunk_ships = 0;
    this->attacks_made = 0;

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

            this->tiles[i][j].tile_sprite.setPosition({ x_pos, y_pos });
            this->tiles[i][j].overlay_sprite.setPosition({ x_pos, y_pos });
        }
    }
}


void Board::Attack_Tile(int r, int c)
{
    if (r < 0 || r >= rows || c < 0 || c >= cols) return;

    Tile& target = tiles[r][c];
    if (target.revealed) return;

    target.revealed = true;
    revealed_tiles++;
    attacks_made++;

    if (target.GetShipStatus())
    {
        target.OverlayTexture("hit");
        hits_landed++;
        sunk_ships++;

        int id = target.GetShipId();

        // Reveal the full ship graphic immediately upon sink
        visual_ships[id].is_revealed = true;

        Attack_Tile_Cascade(r - 1, c, id);
        Attack_Tile_Cascade(r + 1, c, id);
        Attack_Tile_Cascade(r, c - 1, id);
        Attack_Tile_Cascade(r, c + 1, id);
    }
    else
    {
        target.OverlayTexture("miss");
    }
}
void Board::Attack_Tile_Cascade(int r, int c, int ship_id)
{
    if (r < 0 || r >= rows || c < 0 || c >= cols) return;

    Tile& target = tiles[r][c];
    // Stop if already revealed OR if it belongs to a different ship ID
    if (target.revealed || target.GetShipId() != ship_id) return;

    target.revealed = true;
    revealed_tiles++;
    hits_landed++;
    target.OverlayTexture("hit");

    Attack_Tile_Cascade(r - 1, c, ship_id);
    Attack_Tile_Cascade(r + 1, c, ship_id);
    Attack_Tile_Cascade(r, c - 1, ship_id);
    Attack_Tile_Cascade(r, c + 1, ship_id);
}

int Board::Get_Hits_Landed() const { return hits_landed; }
int Board::Get_Revealed_Tiles() const { return revealed_tiles; }
int Board::Get_Sunk_Ships() const { return sunk_ships; }
int Board::Get_Attacks_Made() const { return attacks_made; }
std::vector<std::vector<Tile>>& Board::Get_Tiles()
{
    return this->tiles;
}
std::vector<VisualShip>& Board::Get_Visual_Ships() {
    return visual_ships;
}