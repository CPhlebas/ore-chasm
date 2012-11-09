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

#include "block.h"

#include "lighting/src/Light/LightSystem.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
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

    loadMap();

    for (int i = 0; i < WORLD_RENDERABLE_BLOCKS; ++i) {
        m_renderableBlocks[i] = new Entity("../textures/player.png");
    }

    if (m_shader.loadFromFile("tilerenderer.frag", sf::Shader::Fragment)) {
        std::cout << "Successfully loaded tilerenderer fragment shader!" << std::endl;
    } else {
        std::cout << "failed to load tilerenderer fragment shader!" << std::endl;
        assert(0);
    }

//TODO:    m_shader.setParameter("texture", );

//   saveMap();
}

void World::render()
{

    for (int i = 0; i < WORLD_RENDERABLE_BLOCKS; ++i) {
//        m_renderableBlocks[i].getTexture().
        m_window->draw(*m_renderableBlocks[i]);
        m_renderableBlocks[i]->render(m_window);
    }


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
}

void World::update()
{
    std::cout << "moving player by xDir: " << m_inputXDirection << " yDir: " << m_inputYDirection << std::endl;
    //FIXME: bring in elapsedTime here ...

    m_player->move(m_inputXDirection, m_inputYDirection);
    m_view->setCenter(m_player->getPosition());

//    std::cout << "VIEWPORT: view x: " << center.x << " View y: " << center.y << std::endl;
    const sf::Vector2f playerPosition = m_player->getPosition();
//    std::cout << "VIEWPORT: view x: " << center.x << " View y: " << center.y << std::endl;

    //consider block map as starting at player pos == 0,0 and going down and to the right-ward
    int tilesBeforeX = playerPosition.x / 16;
    //shouldn't change...yet
    const int tilesBeforeY = playerPosition.y / 16;

    // [y*rowlength + x]
    for (int i = 1; i <= WORLD_RENDERABLE_BLOCKS; ++i) {
        int type = m_blocks[tilesBeforeY * WORLD_ROWCOUNT + tilesBeforeX].type;
        Entity *currentBlock = m_renderableBlocks[i - 1];

        const char* texture;
        switch (type) {
        case 0:
            texture = "../textures/dirt.png";
            break;

        case 1:
            texture = "../textures/stone.png";
            break;

        default:
            texture = "../textures/stone.png";
//                assert(0);
        }
        currentBlock->setTexture(texture);

        currentBlock->setPosition(floor(i / (5 * i)) * 16, floor(i / 500) * 16);
        std::cout << "iterating, i value: " << i << std::endl;

        ++tilesBeforeX;
    }

    std::cout << "tilesbeforeX: " << tilesBeforeX << " tilesbeforeY: " << tilesBeforeY << std::endl;

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
    std::uniform_int_distribution<> distribution(0, 1);
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
