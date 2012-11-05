/******************************************************************************
 *   Copyright (C) 2012 by Shaun Reich <sreich@kde.org>                       *
 *                                                                            *
 *   This program is free software; you can redistribute it and/or            *
 *   modify it under the terms of the GNU General Public License as           *
 *   published by the Free Software Foundation; either version 2 of           *
 *   the License, or (at your option) any later version.                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU General Public License for more details.                             *
 *                                                                            *
 *   You should have received a copy of the GNU General Public License        *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 *****************************************************************************/

#ifndef WORLD_H
#define WORLD_H

#include "block.h"
#include "player.h"

#include <stdlib.h>

//height
static const int WORLD_ROWCOUNT = 8400;
//width
static const int WORLD_COLUMNCOUNT = 2400;

/*
 e.g. [ ] [ ] [ ] [ ] [ ]  ... 8400
        [ ] [ ] [ ] [ ] [ ]  ... 8400
        ...
        ...
        ...
        2400


*/

// height is the same as width (they're square)
static const int TILE_SIZE = 16;

class World
{
public:
    World(sf::RenderWindow *window, sf::View *view);

    void update();
    void render();

    void loadMap();

    /**
     * Determines the health and texture of the Block.
     */
    enum BlockType {
        Dirt = 0,
        Stone
    };

    //create containers of various entities, and implement a tile system
    //game.cpp calls into this each tick, which this descends downward into each entity
private:
    // it's faster and easier to manage with a linear array. access is trivial - array[y][x] simply becomes array[y*rowlength + x]
    // makes sure that the memory allocated is in fact contiguous.
    // [column * WORLD_ROWCOUNT + row]
    Block m_blocks[WORLD_ROWCOUNT * WORLD_COLUMNCOUNT];
    Player *m_player = nullptr;

    //FIXME: just a ptr to the game.cpp one :(  same with window
    sf::View *m_view = nullptr;
    sf::RenderWindow *m_window = nullptr;

    /**
     * From scratch, create a randomly generated tileset and store it in our array
     */
    void generateMap();

    void saveMap();
};

#endif
