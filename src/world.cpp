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
#include "logging.h"

#include "block.h"

#include "lighting/src/Light/LightSystem.h"
#include <SFML/src/SFML/Graphics/stb_image/stb_image.h>

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

World::World(sf::RenderWindow *window, sf::View *view)
{
    m_window = window;
    m_view = view;

    m_player = new Player("../textures/player.png");

    /*
    const int gridSize = ceil(WORLD_TILE_TYPE_COUNT / 2.0);
    std::cout << " GRIDSIZE : " << gridSize << std::endl;
    const unsigned short textureSize = gridSize * WORLD_TILE_SIZE;
    m_tileTypesSuperImage.create(textureSize, textureSize);
    m_tileTypesSuperTexture.create(textureSize, textureSize);
    */

    m_tileTypesSuperImage.create(WORLD_TILE_SIZE * WORLD_TILE_TYPE_COUNT, WORLD_TILE_SIZE);
    m_tileTypesSuperTexture.create(WORLD_TILE_SIZE * WORLD_TILE_TYPE_COUNT, WORLD_TILE_SIZE);
    m_tileTypesSuperTexture.bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    m_tileMapFinalTexture.create(1600, 900);
    m_tileMapFinalTexture.bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

            destX = rowIndex * WORLD_TILE_SIZE;
            destY = columnIndex * WORLD_TILE_SIZE;
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
        //FIXME: use i, not hardcode dirt
        loaded = currentTile.loadFromFile(m_blockTextures[i]);
        //would indicate we couldn't find a tile. obviously, we need that..
        assert(loaded);

        destX = i * WORLD_TILE_SIZE;
        m_tileTypesSuperImage.copy(currentTile, destX, destY);
    }

    m_tileTypesSuperImage.saveToFile("TEST.png");
    m_tileTypesSuperTexture.loadFromImage(m_tileTypesSuperImage);

    loadMap();

    if (m_shader.loadFromFile("tilerenderer.frag", sf::Shader::Fragment)) {
        std::cout << "Successfully loaded tilerenderer fragment shader!" << std::endl;
    } else {
        std::cout << "failed to load tilerenderer fragment shader!" << std::endl;
        assert(0);
    }
    m_shader.setParameter("screen_size", sf::Vector2f(1600, 900));
    m_shader.setParameter("tile_types_super_texture", m_tileTypesSuperTexture);

    //saveMap();
}

World::~World()
{
    delete m_player;
}

void World::render()
{
    //FIXME: NEEDED?
    m_tileMapFinalSprite.setTexture(m_tileMapFinalTexture);

 //   m_window->setView(m_window->getDefaultView());
    sf::RenderStates state;
    state.shader = &m_shader;
    m_window->draw(m_tileMapFinalSprite, state);
//    m_window->setView(*m_view);

    //player drawn on top... since we don't have anything like z-ordering or layering (TODO)
    m_window->draw(*m_player);
    m_player->render(m_window);
}

void World::handleEvent(const sf::Event& event)
{

    switch (event.type) {
    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
            m_inputXDirection += 1.f;
        }
        if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
            m_inputXDirection -= 1.f;
        }
        if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
            m_inputYDirection += 1.f;
        }
        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
            m_inputYDirection -= 1.f;
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
    }

    /*
    std::stringstream ss;
    std::string str;
    ss << "tilemap_pixels_" << rand() << ".png";
    str = ss.str();
    m_tileMapPixelsImage.saveToFile(str.c_str());
    */
}

void World::update()
{
    //std::cout << "moving player by xDir: " << m_inputXDirection << " yDir: " << m_inputYDirection << std::endl;
    //FIXME: bring in elapsedTime here ...to calculate player movements accurately

    m_player->move(m_inputXDirection, m_inputYDirection);
//    m_view->setCenter(m_player->getPosition());


//    std::cout << "VIEWPORT: view x: " << center.x << " View y: " << center.y << std::endl;
    const sf::Vector2f playerPosition = m_player->getPosition();
//    std::cout << "player pos x : " << playerPosition.x << " view pos x: " << m_view->getCenter().x << "\n";
//    std::cout << "VIEWPORT: view x: " << center.x << " View y: " << center.y << std::endl;

    //consider block map as starting at player pos == 0,0 and going down and to the right-ward
    //tilesBefore{X,Y} is only at the center of the view though..find the whole screen real estate
    //column
    int tilesBeforeX = playerPosition.x / WORLD_TILE_SIZE;
    //row
    int tilesBeforeY = playerPosition.y / WORLD_TILE_SIZE;

    //FIXME: only calculate this crap when we move/change tiles
    //FIXME: USE SCREEN_H, SCREEN_W
    const int startRow = tilesBeforeY - ((900/2) / WORLD_TILE_SIZE);
    const int endRow = tilesBeforeY + ((900/2) / WORLD_TILE_SIZE);

    //columns are our X value, rows the Y
    const int startColumn = tilesBeforeX - ((1600/2) / WORLD_TILE_SIZE);
    const int endColumn = tilesBeforeX + ((1600/2) / WORLD_TILE_SIZE);

    if (std::abs(startColumn) != startColumn) {
        std::cout << "FIXME, WENT INTO NEGATIVE COLUMN!!";
        assert(0);
    } else if (std::abs(startRow) != startRow) {
        std::cout << "FIXME, WENT INTO NEGATIVE ROW!!";
        assert(0);
    }

    // std::cout << "tilesBeforeX: " << tilesBeforeX << " tilesBeforeY: " << tilesBeforeY << " startRow: " << startRow << " startColumn: " << startColumn << " endRow: " << endRow << " endColumn: " <<  endColumn << "\n";
    // std::cout << "sending visible tilemap to shader!" << "\n";

    // only make it as big as we need it, remember this is a pixel representation of the visible
    // tile map, with the red channel identifying what type of tile it is
    // x is columns..since they move from left to right, rows start at top and move to bottom
    // (and yes..i confused this fact before, leaving a headache here ;)
    m_tileMapPixelsImage.create(endColumn - startColumn, endRow - startRow);

    int x = 0;
    int  y = 0;


    if (m_player->getPosition().x > 1000) {
//        std::cout << "image size, width: " << m_tileMapPixelsImage.getSize().x << " height: " << m_tileMapPixelsImage.getSize().y << "\n" << "startcolumn: " << startColumn << " end column: " << endColumn << "\n";
    }

    // [y*rowlength + x]
    for (int currentRow = startRow; currentRow < (endRow - startRow); ++currentRow) {
        for (int currentColumn = startColumn; currentColumn < (endColumn - startColumn); ++currentColumn) {
            // std::cout << "currentColumn: " << currentColumn << " WORLD_ROWCOUNT: " << WORLD_ROWCOUNT << " currentRow: " << currentRow << "\n";
            const sf::Color color(m_blocks[currentRow * WORLD_ROWCOUNT + currentColumn].type, 0, 0);
            // std::cout << "setting pixels x: " << x << " y: " << y << "\n";
            // std::cout << "currentrow: " << currentRow << " currentColumn: " << currentColumn << "\n";
            m_tileMapPixelsImage.setPixel(x, y, color);
            ++x;
        }
        ++y;
        x = 0;
    }


 //   std::cout << "image size, width: " << m_tileMapPixelsImage.getSize().x << " height: " << m_tileMapPixelsImage.getSize().y << "\n" << "startcolumn: " << startColumn << " end column: " << endColumn << "\n";
    m_tileMapPixelsTexture.loadFromImage(m_tileMapPixelsImage);
    m_tileMapPixelsTexture.bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    m_shader.setParameter("tilemap_pixels", m_tileMapPixelsTexture);
    // std::cout << "finished sending tilemap to shader!!\n";
}

void World::loadMap()
{
    std::cout << "loading map!" << std::endl;
    std::cout << "SIZEOF m_blocks: " << sizeof(m_blocks) / 1e6 << " MiB" << std::endl;
    generateMap();

    m_player->setPosition(800, 450);
//   m_view->setCenter(m_player->getPosition());
}

void World::generateMap()
{
    std::random_device device;
    std::mt19937 rand(device());
    //FIXME: convert to 1, n
    std::uniform_int_distribution<> distribution(0, 2);
    //std::cout << distribution(rand) << ' ';

    sf::Clock clock;

    for (int row = 0; row < WORLD_ROWCOUNT; ++row) {
        for (int column = 0; column < WORLD_COLUMNCOUNT; ++column) {
            m_blocks[column * WORLD_ROWCOUNT + row].type = distribution(rand);
        }
    }

    const int elapsedTime = clock.getElapsedTime().asMilliseconds();
    std::cout << "Time taken for map generation: " << elapsedTime << " Milliseconds" << std::endl;
}

void World::saveMap()
{
    std::cout << "saving map!" << std::endl;
    sf::Image image;
    image.create(WORLD_ROWCOUNT, WORLD_COLUMNCOUNT, sf::Color::White);

    sf::Color color;

    sf::Clock clock;

    for (int row = 0; row < WORLD_ROWCOUNT; ++row) {
        for (int column = 0; column < WORLD_COLUMNCOUNT; ++column) {
//            color.r = m_blocks[column * WORLD_ROWCOUNT + row].type;
            image.setPixel(row, column, color);
        }
    }

    bool saved = image.saveToFile("../../saves/level1.png");
    assert(saved);

    const int elapsedTime = clock.getElapsedTime().asMilliseconds();
    std::cout << "Time taken for map saving: " << elapsedTime << " Milliseconds" << std::endl;

}
