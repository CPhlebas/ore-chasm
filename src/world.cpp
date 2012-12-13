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

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <GL/gl.h>

static World* s_instance = 0;

World* World::instance()
{
    assert(s_instance);
    return s_instance;
}

void World::createInstance(sf::RenderWindow *_window, sf::View *_view)
{
    if (!s_instance) {
        s_instance = new World(_window, _view);
    } else {
        assert(0);
    }
}


World::World(sf::RenderWindow *window, sf::View *view)
{
    m_window = window;
    m_view = view;

    m_player = new Player("../textures/player.png");
    /*
    const int gridSize = ceil(WORLD_TILE_TYPE_COUNT / 2.0);
    std::cout << " GRIDSIZE : " << gridSize << std::endl;
    const unsigned short textureSize = gridSize * Block::blockSize;
    m_tileTypesSuperImage.create(textureSize, textureSize);
    m_tileTypesSuperTexture.create(textureSize, textureSize);
    */

    m_tileTypesSuperImage.create(Block::blockSize * WORLD_TILE_TYPE_COUNT, Block::blockSize);
    m_tileTypesSuperTexture.create(Block::blockSize * WORLD_TILE_TYPE_COUNT, Block::blockSize);

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

    for (int i = 0; i < WORLD_TILE_TYPE_COUNT; ++i) {
        loaded = currentTile.loadFromFile(m_blockTextures[i]);
        //would indicate we couldn't find a tile. obviously, we need that..
        assert(loaded);

        destX = i * Block::blockSize;
        m_tileTypesSuperImage.copy(currentTile, destX, destY);
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
    m_sky = new Sky(m_window, m_view, 0.0f);
}

World::~World()
{
    delete m_player;
    delete m_sky;
}

void World::render()
{
    //Sky at bottom layer

    sf::RenderStates state;
    state.shader = &m_shader;
    m_window->draw(m_tileMapFinalSprite, state);

    //set our view so that the player will stay relative to the view, in the center.
    m_window->setView(*m_view);

    //player drawn on top... since we don't have anything like z-ordering or layering (TODO)
    m_window->draw(*m_player);
    m_player->render(m_window);

    m_window->setView(m_window->getDefaultView());

    // ==================================================
    const sf::Vector2i mouse = sf::Mouse::getPosition(*m_window);
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
    m_sky->render();
}

void World::handleEvent(const sf::Event& event)
{

    switch (event.type) {
    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
            m_inputXDirection = 1.f;
        }
        if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
            m_inputXDirection = -1.f;
        }
        if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
            m_inputYDirection = 1.f;
        }
        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
            m_inputYDirection = -1.f;
        }
        break;

    case sf::Event::KeyReleased:
        if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
            m_inputXDirection = 0.f;
        }
        if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
            m_inputXDirection = 0.f;
        }
        if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
            m_inputYDirection = 0.f;
        }
        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
            m_inputYDirection = 0.f;
        }
        break;

    case sf::Event::MouseButtonPressed:
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            m_mouseLeftHeld = true;
        }
        break;

    case sf::Event::MouseButtonReleased:
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            m_mouseLeftHeld = false;
        }
    }
}

void World::update(const float elapsedTime)
{
    if (m_mouseLeftHeld) {
        performBlockAttack();
    }

    m_sky->update(elapsedTime);

    m_player->move(m_inputXDirection * elapsedTime * Player::movementSpeed, m_inputYDirection * elapsedTime * Player::movementSpeed);
    m_view->setCenter(m_player->getPosition());

    //calculateAttackPosition();
    generatePixelTileMap();
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
                World::m_blocks[index].type = 0;
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

            const sf::Color color(m_blocks[index].type, 0, 0);

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
            m_blocks[column * WORLD_ROWCOUNT + lastRow].type = 0;
        }
    }

    for (; lastRow < WORLD_ROWCOUNT; ++lastRow) {
        for (int column = 0; column < WORLD_COLUMNCOUNT; ++column) {
            m_blocks[column * WORLD_ROWCOUNT + lastRow].type = distribution(rand);
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
            color.r = m_blocks[column * WORLD_ROWCOUNT + row].type;
            image.setPixel(row, column, color);
        }
    }

    bool saved = image.saveToFile("levelsave.png");
    assert(saved);

    const int elapsedTime = clock.getElapsedTime().asMilliseconds();
    std::cout << "Time taken for map saving: " << elapsedTime << " Milliseconds" << std::endl;

}
