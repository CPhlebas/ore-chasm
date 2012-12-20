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

#include "world.h"
#include "debug.h"

#include "block.h"
#include "game.h"
#include "sky.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <random>
#include <assert.h>
#include <math.h>

#include <GL/gl.h>

#include <eigen3/Eigen/Core>

#include <allegro5/allegro.h>

static World* s_instance = 0;

World* World::instance()
{
    assert(s_instance);
    return s_instance;
}

void World::createInstance(ALLEGRO_DISPLAY *display)
{
    if (!s_instance) {
        s_instance = new World(display);
    } else {
        assert(0);
    }
}

World::World(ALLEGRO_DISPLAY *display) : m_display(display)
{
    m_player = new Player("../textures/player.png");
    m_entities.insert(m_entities.end(), m_player);

    /*
    const int gridSize = ceil(WORLD_TILE_TYPE_COUNT / 2.0);
    std::cout << " GRIDSIZE : " << gridSize << std::endl;
    const unsigned short textureSize = gridSize * Block::blockSize;
    m_tileTypesSuperImage.create(textureSize, textureSize);
    m_tileTypesSuperTexture.create(textureSize, textureSize);
    */

    m_tileTypesSuperImage.create(Block::blockSize * Block::blockTypeMap.size(), Block::blockSize);
    m_tileTypesSuperTexture.create(Block::blockSize * Block::blockTypeMap.size(), Block::blockSize);

    m_tileMapFinalTexture.create(SCREEN_W, SCREEN_H);
    m_tileMapFinalSprite.setTexture(m_tileMapFinalTexture);

    assert(m_shader.isAvailable()); // if the system doesn't support it, we're fucked

    //FIXME/TODO: use RenderTexture, so we're not slow as all mighty fuck
    sf::Image currentTile;
    bool loaded;

    unsigned int destX = 0;
    unsigned int destY = 0;

    /*
     TODO: for when we hit the 256 tile limit ... hopefully that won't happen for a while :)
        // iterate through each TILE_TYPE in m_blockTextures and create a super
        // texture out of this which we can pass to the shader as a list of known
        // tiles.
        for (int columnIndex = 0; columnIndex < gridSize; ++columnIndex) {
            for (int rowIndex = 0; rowIndex < gridSize; ++rowIndex) {
                std::cout << "accfessing block texture at i value: " << i << std::endl;
                loaded = currentTile.loadFromFile(m_blockTextures[i]);
                //would indicate we couldn't find a tile. obviously, we need that..
                assert(loaded);

                destX = rowIndex * Block::blockSize;
                destY = columnIndex * Block::blockSize;
                std::cout << "placing tile at X: " << destX << " y: " << destY << std::endl;
                m_tileTypesSuperImage.copy(currentTile, destX, destY);

                ++i;
                if (i >= WORLD_TILE_TYPE_COUNT) {
                    break;
                }
            }
        }
    */

    int i = 0;
    for (auto blockStruct : Block::blockTypeMap) {
        loaded = currentTile.loadFromFile(blockStruct.second.texture);

        //would indicate we couldn't find a tile. obviously, we need that..
        assert(loaded);

        destX = i * Block::blockSize;
        m_tileTypesSuperImage.copy(currentTile, destX, destY);
        ++i;
    }

    m_tileTypesSuperTexture.loadFromImage(m_tileTypesSuperImage);

    loadMap();
    //FIXME: saveMap();

    if (m_shader.loadFromFile("tilerenderer.frag", sf::Shader::Fragment)) {
        std::cout << "Successfully loaded tilerenderer fragment shader!" << std::endl;
    } else {
        std::cout << "failed to load tilerenderer fragment shader!" << std::endl;
        assert(0);
    }

    //FIXME: hardcoding :(
    //m_shader.setParameter("TILE_SIZE", Block::blockSize, Block::blockSize);
    m_shader.setParameter("tile_types_super_texture", m_tileTypesSuperTexture);

    //FIXME: height
//    m_sky = new Sky(m_window, m_view, 0.0f);
}

World::~World()
{
    delete m_player;
//    delete m_sky;
}

void World::render()
{
    //Sky at bottom layer

    m_window->draw(m_tileMapFinalSprite, state);

    //set our view so that the player will stay relative to the view, in the center.
    m_window->setView(*m_view);

    //player drawn on top... since we don't have anything like z-ordering or layering (TODO)
    for (Entity * currentEntity : m_entities) {
        m_window->draw(*currentEntity);
        currentEntity->render(m_window);
    }

    m_window->setView(m_window->getDefaultView());

    // ==================================================
    ALLEGRO_MOUSE_STATE state;
    al_get_mouse_state(&state);

    sf::Vector2i mouse(state.x, state.y);

    const float radius = 16.0f;
    const float halfRadius = radius * 0.5;
    const float halfBlockSize = Block::blockSize * 0.5;

    // NOTE: (SCREEN_H % Block::blockSize) is what we add so that it is aligned properly to the tile grid, even though the screen is not evenly divisible by such.
    sf::Vector2f crosshairPosition(mouse.x - mouse.x % Block::blockSize + (SCREEN_W % Block::blockSize) - tileOffset().x + Block::blockSize, mouse.y - mouse.y % Block::blockSize + (SCREEN_H % Block::blockSize) - tileOffset().y + Block::blockSize);

    sf::RectangleShape crosshair = sf::RectangleShape();
    crosshair.setSize(sf::Vector2f(radius, radius));
    crosshair.setPosition(crosshairPosition);
    crosshair.setFillColor(sf::Color::Transparent);
    crosshair.setOutlineColor(sf::Color::Red);
    crosshair.setOutlineThickness(-2.0f);
    crosshair.setOrigin(halfRadius - halfBlockSize, halfRadius - halfBlockSize);
    m_window->draw(crosshair);
    // ==================================================
//    m_sky->render();
}

void World::handleEvent(const ALLEGRO_EVENT& event)
{

    //FIXME:!!!! unused, we just pass events to the player..among other children (currently just player though)
    //here, the inventory ui and other stuff may need to be factored in. who knows.
    switch (event.type) {
        case ALLEGRO_EVENT_KEY_DOWN:
        if (event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_W || event.keyboard.keycode == ALLEGRO_KEY_UP) {
        }

        break;

        case ALLEGRO_EVENT_KEY_UP:
        if (event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
        }
 
        if (event.keyboard.keycode == ALLEGRO_KEY_W || event.keyboard.keycode == ALLEGRO_KEY_UP) {
        }

        break;

        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
            ALLEGRO_MOUSE_STATE state;
            al_get_mouse_state(&state);
            if (state.buttons & ALLEGRO_MOUSE_LEFT) {
                m_mouseLeftHeld = true;
            }
        break;
        }

    case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
            ALLEGRO_MOUSE_STATE state;
            al_get_mouse_state(&state);
            if (state.buttons & ~ALLEGRO_MOUSE_LEFT) {
                m_mouseLeftHeld = false;
            }
        break;
    }
    }

    m_player->handleEvent(event);
}

void World::update(const float elapsedTime)
{
    if (m_mouseLeftHeld) {
        performBlockAttack();
    }

//    m_sky->update(elapsedTime);

for (Entity * currentEntity : m_entities) {
        currentEntity->update(elapsedTime);
    }

    m_view->setCenter(m_player->getPosition());

    //calculateAttackPosition();
    generatePixelTileMap();
}

unsigned char World::calculateTileMeshingType(const int tileX, const int tileY) const
{
    const bool left = tileBlendTypeMatch(tileX, tileY, tileX - 1, tileY);
    const bool right = tileBlendTypeMatch(tileX, tileY, tileX + 1, tileY);

    const bool top = tileBlendTypeMatch(tileX, tileY, tileX, tileY - 1);
    const bool bottom = tileBlendTypeMatch(tileX, tileY, tileX, tileY + 1);

    const bool topLeft = tileBlendTypeMatch(tileX, tileY, tileX - 1, tileY - 1);
    const bool bottomLeft = tileBlendTypeMatch(tileX, tileY, tileX - 1, tileY + 1);

    const bool topRight = tileBlendTypeMatch(tileX, tileY, tileX + 1, tileY - 1);
    const bool bottomRight = tileBlendTypeMatch(tileX, tileY, tileX + 1, tileY + 1);

    // behold the motherfucking magic
    const unsigned char result = (1 * topRight) + (2 * bottomRight) + (4 * bottomLeft) + (8 * topLeft) + (16 * top) + (32 * right) + (64 * bottom) + (128 * left);
    return result;
}

bool World::isTileSolid(const sf::Vector2f& vecDest) const
{
    const int column = int(std::ceil(vecDest.x) / Block::blockSize);
    const int row = int(std::ceil(vecDest.y) / Block::blockSize);

    int index = column * WORLD_ROWCOUNT + row;
    assert(index < WORLD_ROWCOUNT * WORLD_COLUMNCOUNT);

    const unsigned char tileType = World::m_blocks[index].primitiveType;

    //FIXME: do water, lava, doors..what else?
    return  tileType != 0;
}

bool World::tileBlendTypeMatch(const int sourceTileX, const int sourceTileY, const int nearbyTileX, const int nearbyTileY) const
{
    bool isMatched = false;
    const int srcIndex = sourceTileX * WORLD_ROWCOUNT + sourceTileY;
    const int nearbyIndex = nearbyTileX * WORLD_ROWCOUNT + nearbyTileY;

    if (m_blocks[srcIndex].primitiveType == m_blocks[nearbyIndex].primitiveType) {
        isMatched = true;
    }

    //TODO future, use blending types..not just tile type is same

    return isMatched;
}

sf::Vector2f World::viewportCenter() const
{
    return sf::Vector2f(SCREEN_W * 0.5, SCREEN_H * 0.5);
}

//FIXME: unused..will be used for shooting and such. not for block breaking.
void World::calculateAttackPosition()
{
    /*    const sf::Vector2f _viewportCenter = viewportCenter();

        const sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);

        sf::Vector2f diffVect;
        diffVect.x = mousePos.x - _viewportCenter.x;
        diffVect.y = mousePos.y - _viewportCenter.y;

        const double angle = atan2(diffVect.y, diffVect.x);
        const float newX = _viewportCenter.x + cos(angle) * Player::blockPickingRadius;
        const float newY= _viewportCenter.y  + sin(angle) * Player::blockPickingRadius;
        m_relativeVectorToAttack = sf::Vector2f(newX, newY);
    */
}

//FIXME: this function needs a lot of help. it's just a copy from pixelmap generation
//so make it so it doesn't iterate over the whole visible screen but just the blockPickingRadius size.
void World::performBlockAttack()
{
    /*
    const sf::Vector2f viewCenter = m_view->getCenter();

    sf::Vector2f viewPosition;
    //    std::cout << "viewportcenter" << " viewportcenter y: " << viewportCenter().y << " view->getcenter() y: " << viewCenter.y << "\n";
    viewPosition.x = viewCenter.x - viewportCenter().x;
    viewPosition.y = viewCenter.y - viewportCenter().y;
    const int column = int((m_relativeVectorToAttack.x + viewPosition.x) / Block::blockSize);
    const int row = int((m_relativeVectorToAttack.y + viewPosition.y) / Block::blockSize);
    //    std::cout << "relativevector y: " << m_relativeVectorToAttack.y << " view position y: " << viewPosition.y << "\n";

    const int startRow = (m_player->getPosition().y / Block::blockSize) - radius;
    const int startColumn = (m_player->getPosition().x / Block::blockSize) - radius;
    const int endRow = (m_player->getPosition().y / Block::blockSize) + radius;
    const int endColumn = (m_player->getPosition().x / Block::blockSize) + radius;
    */

    sf::Vector2i mouse = sf::Mouse::getPosition(*m_window);

    //FIXME: eventually will need to make this go to the players center
    // can we divide player pos by half of screen h/w ?
    const sf::Vector2i center = sf::Vector2i(SCREEN_W * 0.5, SCREEN_H * 0.5);

    // if the attempted block pick location is out of range, do nothing.
    if (mouse.x < center.x - Player::blockPickingRadius ||
            mouse.x > center.x + Player::blockPickingRadius ||
            mouse.y < center.y - Player::blockPickingRadius ||
            mouse.y > center.y + Player::blockPickingRadius) {
        return;
    }

    mouse.x /= int(Block::blockSize);
    mouse.y /= int(Block::blockSize);

    const int radius = Player::blockPickingRadius / Block::blockSize;

    int attackX = mouse.x + (m_view->getCenter().x - SCREEN_W * 0.5) / Block::blockSize;
    int attackY = mouse.y + (m_view->getCenter().y - SCREEN_H * 0.5) / Block::blockSize;

    const sf::Vector2f playerPosition = m_player->getPosition();

    //consider block map as starting at player pos == 0,0 and going down and to the right-ward
    //tilesBefore{X,Y} is only at the center of the view though..find the whole screen real estate
    // which is why startRow etc add and subtract half the screen
    //column
    int tilesBeforeX = playerPosition.x / Block::blockSize;
    //row
    int tilesBeforeY = playerPosition.y / Block::blockSize;

    const int startRow = tilesBeforeY - ((SCREEN_H * 0.5) / Block::blockSize);
    const int endRow = tilesBeforeY + ((SCREEN_H * 0.5) / Block::blockSize);

    //columns are our X value, rows the Y
    const int startColumn = tilesBeforeX - ((SCREEN_W * 0.5) / Block::blockSize);
    const int endColumn = tilesBeforeX + ((SCREEN_W * 0.5) / Block::blockSize);

    int index = 0;

    for (int row = startRow; row < endRow; ++row) {
        for (int column = startColumn; column < endColumn; ++column) {
            if (row == attackY && column == attackX) {
                index = column * WORLD_ROWCOUNT + row;
                assert(index < WORLD_ROWCOUNT * WORLD_COLUMNCOUNT);
                World::m_blocks[index].primitiveType = 0;
                return;
            }
        }
    }

    std::cout << "ERROR: " << " no block found to attack?" << "\n";
}

void World::generatePixelTileMap()
{
    const sf::Vector2f playerPosition = m_player->getPosition();
    //consider block map as starting at player pos == 0,0 and going down and to the right-ward
    //tilesBefore{X,Y} is only at the center of the view though..find the whole screen real estate
    //column
    int tilesBeforeX = playerPosition.x / Block::blockSize;
    //row
    int tilesBeforeY = playerPosition.y / Block::blockSize;

    //FIXME: only calculate this crap when we move/change tiles
    // -1 so that we render an additional row and column..to smoothly scroll
    const int startRow = tilesBeforeY - ((SCREEN_H * 0.5) / Block::blockSize) - 1;
    const int endRow = tilesBeforeY + ((SCREEN_H * 0.5) / Block::blockSize);

    //columns are our X value, rows the Y
    const int startColumn = tilesBeforeX - ((SCREEN_W * 0.5) / Block::blockSize) - 1;
    const int endColumn = tilesBeforeX + ((SCREEN_W * 0.5) / Block::blockSize);

    if (std::abs(startColumn) != startColumn) {
        std::cout << "FIXME, WENT INTO NEGATIVE COLUMN!!";
        assert(0);
    } else if (std::abs(startRow) != startRow) {
        std::cout << "FIXME, WENT INTO NEGATIVE ROW!!";
        assert(0);
    }

    // only make it as big as we need it, remember this is a pixel representation of the visible
    // tile map, with the red channel identifying what type of tile it is
    // x is columns..since they move from left to right, rows start at top and move to bottom
    // (and yes..i confused this fact before, leaving a headache here ;)
    m_tileMapPixelsImage.create(endColumn - startColumn, endRow - startRow);

    int x = 0;
    int  y = 0;

    // [y*rowlength + x]
    for (int currentRow = startRow; currentRow < endRow; ++currentRow) {
        for (int currentColumn = startColumn; currentColumn < endColumn; ++currentColumn) {

            const int index = currentColumn * WORLD_ROWCOUNT + currentRow;
            assert(index < WORLD_ROWCOUNT * WORLD_COLUMNCOUNT);

            const sf::Color color(m_blocks[index].primitiveType, 0, 0);

            m_tileMapPixelsImage.setPixel(x, y, color);
            ++x;
        }
        ++y;
        x = 0;
    }

    //FIXME: hugely fucking expensive..fix the above loops so we *generate* it upside down
    // or...change the shader to calculate it properly
    m_tileMapPixelsImage.flipVertically();

    m_tileMapPixelsTexture.loadFromImage(m_tileMapPixelsImage);

    m_shader.setParameter("tilemap_pixels", m_tileMapPixelsTexture);
    // to get per-pixel smooth scrolling, we get the remainders and pass it as an offset to the shader
    m_shader.setParameter("offset", tileOffset().x, tileOffset().y);
}

sf::Vector2f World::tileOffset() const
{
    const sf::Vector2f playerPosition = m_player->getPosition();
    // to get per-pixel smooth scrolling, we get the remainders and pass it as an offset to things that need to know the tile positions
    const sf::Vector2f ret = sf::Vector2f(int(playerPosition.x) & Block::blockSize - 1, int(playerPosition.y) & Block::blockSize - 1);
    return ret;
}

void World::loadMap()
{
    std::cout << "loading map!" << std::endl;
    std::cout << "SIZEOF m_blocks: " << sizeof(m_blocks) / 1e6 << " MiB" << std::endl;
    generateMap();

    m_player->setPosition(2800, 2450);
}

void World::generateMap()
{
    sf::Clock stopwatch;

    std::random_device device;
    std::mt19937 rand(device());
    //FIXME: convert to 1, n
    std::uniform_int_distribution<> distribution(1, 3);

    int lastRow = 0;

    // 200 rows of "sky"
    for (; lastRow < 15; ++lastRow) {
        for (int column = 0; column < WORLD_COLUMNCOUNT; ++column) {
            m_blocks[column * WORLD_ROWCOUNT + lastRow].primitiveType = 0;
        }
    }

    for (; lastRow < WORLD_ROWCOUNT; ++lastRow) {
        for (int column = 0; column < WORLD_COLUMNCOUNT; ++column) {
            m_blocks[column * WORLD_ROWCOUNT + lastRow].primitiveType = distribution(rand);
        }
    }

    const int elapsedTime = stopwatch.getElapsedTime().asMilliseconds();
    std::cout << "Time taken for map generation: " << elapsedTime << " Milliseconds" << std::endl;
}

void World::saveMap()
{
    std::cout << "saving map!" << std::endl;
    sf::Image image;
    image.create(WORLD_ROWCOUNT, WORLD_COLUMNCOUNT, sf::Color::White);

    sf::Color color(0, 0, 0, 255);
    sf::Clock clock;

    for (int row = 0; row < WORLD_ROWCOUNT; ++row) {
        for (int column = 0; column < WORLD_COLUMNCOUNT; ++column) {
            color.r = m_blocks[column * WORLD_ROWCOUNT + row].primitiveType;
            image.setPixel(row, column, color);
        }
    }

    bool saved = image.saveToFile("levelsave.png");
    assert(saved);

    const int elapsedTime = clock.getElapsedTime().asMilliseconds();
    std::cout << "Time taken for map saving: " << elapsedTime << " Milliseconds" << std::endl;

}
