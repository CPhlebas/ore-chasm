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
#include "game.h"

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
    const unsigned short textureSize = gridSize * Block::blockSize;
    m_tileTypesSuperImage.create(textureSize, textureSize);
    m_tileTypesSuperTexture.create(textureSize, textureSize);
    */

    m_tileTypesSuperImage.create(Block::blockSize * WORLD_TILE_TYPE_COUNT, Block::blockSize);
    m_tileTypesSuperTexture.create(Block::blockSize * WORLD_TILE_TYPE_COUNT, Block::blockSize);
    m_tileTypesSuperTexture.bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    m_tileMapFinalTexture.create(SCREEN_W, SCREEN_H);
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
        //FIXME: use i, not hardcode dirt
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

}

World::~World()
{
    delete m_player;
}

void World::render()
{
    //FIXME: NEEDED?
    m_tileMapFinalSprite.setTexture(m_tileMapFinalTexture);

    sf::RenderStates state;
    state.shader = &m_shader;
    m_window->draw(m_tileMapFinalSprite, state);

    m_window->setView(*m_view);

    //player drawn on top... since we don't have anything like z-ordering or layering (TODO)
    m_window->draw(*m_player);
    m_player->render(m_window);

    m_window->setView(m_window->getDefaultView());

    // ==================== draw debug =====================
/*    sf::VertexArray line(sf::Lines, 2);
    line.append(sf::Vertex(viewportCenter()));
    line.append(sf::Vertex(m_relativeVectorToAttack));
    m_window->draw(line);
    */

    const sf::Vector2i mouse = sf::Mouse::getPosition(*m_window);
    const float radius = 10.0f;
    sf::CircleShape crosshair = sf::CircleShape(radius);
    crosshair.setPosition(sf::Vector2f(mouse.x, mouse.y));
    crosshair.setFillColor(sf::Color::Transparent);
    crosshair.setOutlineColor(sf::Color::Red);
    crosshair.setOutlineThickness(2.0f);
    crosshair.setOrigin(radius, radius);
    m_window->draw(crosshair);
    // ==================================================

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
    case sf::Event::MouseButtonPressed:
        performBlockAttack();
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

void World::performBlockAttack()
{
//    std::cout << "relative vector to attack x: " << (int)m_relativeVectorToAttack.x << " y: " << (int)m_relativeVectorToAttack.y << "\n";
/*
    const sf::Vector2f viewCenter = m_view->getCenter();

    sf::Vector2f viewPosition;
//    std::cout << "viewportcenter" << " viewportcenter y: " << viewportCenter().y << " view->getcenter() y: " << viewCenter.y << "\n";
    viewPosition.x = viewCenter.x - viewportCenter().x;
    viewPosition.y = viewCenter.y - viewportCenter().y;
    const int column = int((m_relativeVectorToAttack.x + viewPosition.x) / Block::blockSize);
    const int row = int((m_relativeVectorToAttack.y + viewPosition.y) / Block::blockSize);
//    std::cout << "relativevector y: " << m_relativeVectorToAttack.y << " view position y: " << viewPosition.y << "\n";

    */

    //FIXME: this function needs a lot of help. it's just a copy from pixelmap generation
    //so make it so it doesn't iterate over the whole visible screen but just the blockPickingRadius size.
    const int radius = Player::blockPickingRadius / Block::blockSize;

    /*
    const int startRow = (m_player->getPosition().y / Block::blockSize) - radius;
    const int startColumn = (m_player->getPosition().x / Block::blockSize) - radius;
    const int endRow = (m_player->getPosition().y / Block::blockSize) + radius;
    const int endColumn = (m_player->getPosition().x / Block::blockSize) + radius;
    */
    sf::Vector2i mouse = sf::Mouse::getPosition(*m_window);

    const int attackX = mouse.x / int(Block::blockSize);
    const int attackY = mouse.y / int(Block::blockSize);

    const sf::Vector2f playerPosition = m_player->getPosition();
    //consider block map as starting at player pos == 0,0 and going down and to the right-ward
    //tilesBefore{X,Y} is only at the center of the view though..find the whole screen real estate
    //column
    int tilesBeforeX = playerPosition.x / Block::blockSize;
    //row
    int tilesBeforeY = playerPosition.y / Block::blockSize;

    //FIXME: only calculate this crap when we move/change tiles
    //FIXME: USE SCREEN_H, SCREEN_W
    const int startRow = tilesBeforeY - ((SCREEN_H * 0.5) / Block::blockSize);
    const int endRow = tilesBeforeY + ((SCREEN_H * 0.5) / Block::blockSize);

    //columns are our X value, rows the Y
    const int startColumn = tilesBeforeX - ((SCREEN_W * 0.5) / Block::blockSize);
    const int endColumn = tilesBeforeX + ((SCREEN_W * 0.5) / Block::blockSize);

    int index = 0;

    for (int row = startRow; row < endRow; ++row) {
        for (int column = startColumn; column < endColumn; ++column) {
            if (row == attackY && column == attackX) {
//            if (row == (200/Block::blockSize) && column == (200 / Block::blockSize)) {
                index = column * WORLD_ROWCOUNT + row;
                std::cout << "del index: " << index << "\n";
                assert(index < WORLD_ROWCOUNT * WORLD_COLUMNCOUNT);
                m_blocks[index].type = 0;
                break;
            }
        }
    }

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
    //FIXME: USE SCREEN_H, SCREEN_W
    const int startRow = tilesBeforeY - ((SCREEN_H * 0.5) / Block::blockSize);
    const int endRow = tilesBeforeY + ((SCREEN_H * 0.5) / Block::blockSize);

    //columns are our X value, rows the Y
    const int startColumn = tilesBeforeX - ((SCREEN_W * 0.5) / Block::blockSize);
    const int endColumn = tilesBeforeX + ((SCREEN_W * 0.5) / Block::blockSize);

    if (std::abs(startColumn) != startColumn) {
        std::cout << "FIXME, WENT INTO NEGATIVE COLUMN!!";
        assert(0);
    } else if (std::abs(startRow) != startRow) {
        std::cout << "FIXME, WENT INTO NEGATIVE ROW!!";
        assert(0);
    }

    // std::cout << "sending visible tilemap to shader!" << "\n";

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
            // std::cout << "currentColumn: " << currentColumn << " WORLD_ROWCOUNT: " << WORLD_ROWCOUNT << " currentRow: " << currentRow << "\n";

            const int index = currentColumn * WORLD_ROWCOUNT + currentRow;
            assert(index < WORLD_ROWCOUNT * WORLD_COLUMNCOUNT);

            const sf::Color color(m_blocks[index].type, 0, 0);

            // std::cout << "setting pixels x: " << x << " y: " << y << "\n";
            // std::cout << "currentrow: " << currentRow << " currentColumn: " << currentColumn << "\n";
            m_tileMapPixelsImage.setPixel(x, y, color);
            ++x;
        }
        ++y;
        x = 0;
    }

    //FIXME: hugely fucking expensive..fix the above loops so we *generate* it upside down
    // or...change the shader to calculate it properly
    m_tileMapPixelsImage.flipVertically();

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
    std::uniform_int_distribution<> distribution(1, 3);
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
