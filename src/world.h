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
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>

//height
static const unsigned short WORLD_ROWCOUNT = 8400;
//width
static const unsigned short WORLD_COLUMNCOUNT = 2400;
static const unsigned short WORLD_RENDERABLE_BLOCKS = 1000;
/*
 e.g. [ ] [ ] [ ] [ ] [ ]  ... 8400
        [ ] [ ] [ ] [ ] [ ]  ... 8400
        ...
        ...
        ...
        2400


*/

// height is the same as width (they're square)
static const unsigned char WORLD_TILE_SIZE = 16;
//NOTE: update this when you update m_blockTextures
static const unsigned short WORLD_TILE_TYPE_COUNT = 2;

class World
{
public:
    World(sf::RenderWindow *window, sf::View *view);

    void update();
    void render();

    void loadMap();

    void handleEvent(const sf::Event& event);

    /**
     * Determines the health and texture of the Block.
     */
    enum BlockType {
        Dirt = 0,
        Stone
    };

    /**
     * Coincides with BlockType (and goes in order.
     * The enum BlockType acts as index for this.
     * NOTE: update TILE_TYPE_COUNT when this gets something
     * added to it.
     */
    const char* const m_blockTextures[WORLD_TILE_TYPE_COUNT] {
        "../textures/dirt.png",
        "../textures/stone.png"
    };

    //create containers of various entities, and implement a tile system
    //game.cpp calls into this each tick, which this descends downward into each entity
private:
    // it's faster and easier to manage with a linear array. access is trivial - array[y][x] simply becomes array[y*rowlength + x]
    // makes sure that the memory allocated is in fact contiguous.
    // [column * WORLD_ROWCOUNT + row]
    Block m_blocks[WORLD_ROWCOUNT * WORLD_COLUMNCOUNT];
    Entity *m_renderableBlocks[WORLD_RENDERABLE_BLOCKS];

    Player *m_player = nullptr;

    //FIXME: just a ptr to the game.cpp one :(  same with window
    sf::View *m_view = nullptr;
    sf::RenderWindow *m_window = nullptr;

    sf::Shader m_shader;

    /**
     * A super image which is loaded ONLY at init, which is a tilesheet/spritesheet
     * of every tile that is possible. Used for passing it to the tile rendering shader
     * (also at init).
     */
    sf::Image *m_tileTypesSuperImage = nullptr;

    /**
     * What is actually passed to the frag shader.
     */
    sf::Texture *m_tileTypesSuperTexture = nullptr;

    float m_inputXDirection = 0.f;
    float m_inputYDirection = 0.f;

    /**
     * From scratch, create a randomly generated tileset and store it in our array
     */
    void generateMap();

    void saveMap();
};

#endif
