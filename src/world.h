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

class Sky;
//height
static constexpr unsigned short WORLD_ROWCOUNT = 8400;
//width
static constexpr unsigned short WORLD_COLUMNCOUNT = 2400;
/*
 e.g. [ ] [ ] [ ] [ ] [ ]  ... 8400
        [ ] [ ] [ ] [ ] [ ]  ... 8400
        ...
        ...
        ...
        2400


*/

//NOTE: update this when you update m_blockTextures
static constexpr unsigned short WORLD_TILE_TYPE_COUNT = 4;

class World
{
public:
    World(sf::RenderWindow *window, sf::View *view);

    static World* instance();
    static void createInstance(sf::RenderWindow *_window, sf::View *_view);

    void update(const float elapsedTime);
    void render();

    void loadMap();

    void handleEvent(const sf::Event& event);

    /**
     * Determines the health and texture of the Block.
     * NOTE: MUST be in sync with index of m_blockTextures
     */
    enum BlockType {
        Null = 0,
        Dirt,
        Stone,
        Grass
    };

    /**
     * Coincides with BlockType (and goes in order.
     * The enum BlockType acts as index for this.
     * NOTE: update TILE_TYPE_COUNT when this gets something
     * added to it, as well as BlockType.
     */
    const char* const m_blockTextures[WORLD_TILE_TYPE_COUNT] {
        "../textures/null.png",
        "../textures/dirt.png",
        "../textures/stone.png",
        "../textures/grass.png"
    };

    bool isTileSolid(const sf::Vector2f& vecDest);

    //create containers of various entities, and implement a tile system
    //game.cpp calls into this each tick, which this descends downward into each entity
private:

    ~World();

        /**
     * From scratch, create a randomly generated tileset and store it in our array
     */
    void generateMap();

     /**
     * FIXME: presently only calculates the center of the screen according to resolution.
     * i'm not sure how zooming will be affected with this..i don't *think* it would. but verify
     * if this is ideal or not
     * NOTE: doesn't *actually* use m_view->getViewport, just a simple SCREEN_W,H / 2
     */
    sf::Vector2f viewportCenter() const;
    void calculateAttackPosition();
    void generatePixelTileMap();
    void performBlockAttack();
    void saveMap();

    sf::Vector2f tileOffset() const;

    std::vector<Entity*> m_entities;

    // it's faster and easier to manage with a linear array. access is trivial - array[y][x] simply becomes array[y*rowlength + x]
    // makes sure that the memory allocated is in fact contiguous.
    // [column * WORLD_ROWCOUNT + row]
    Block m_blocks[WORLD_ROWCOUNT * WORLD_COLUMNCOUNT];

    Player *m_player = nullptr;

    Sky *m_sky = nullptr;

    //FIXME: just a ptr to the game.cpp one :(  same with window
    sf::View *m_view = nullptr;
    sf::RenderWindow *m_window = nullptr;

    sf::Shader m_shader;

    /**
     * Final texture that is blitted to screen, with the shader and render states
     * (for the tilemap)
     */
    sf::Texture m_tileMapFinalTexture;
    sf::Sprite m_tileMapFinalSprite;

    sf::Image m_tileMapPixelsImage;
    sf::Texture m_tileMapPixelsTexture;

    /**
     * A super image which is loaded ONLY at init, which is a tilesheet/spritesheet
     * of every tile that is possible. Used for passing it to the tile rendering shader
     * (also at init).
     */
    sf::Image m_tileTypesSuperImage;

    /**
     * What is actually passed to the frag shader.
     */
    sf::Texture m_tileTypesSuperTexture;

    float m_inputXDirection = 0.f;
    float m_inputYDirection = 0.f;

    bool m_mouseLeftHeld = false;

    /**
     * In client window coordinates (relative)
     */
    sf::Vector2f m_relativeVectorToAttack;
};

#endif
