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
 //   saveMap();
}

void World::render()
{
    m_window->draw(*m_player);

    for (int i = 0; i < WORLD_RENDERABLE_BLOCKS; ++i) {
        m_window->draw(m_renderableBlocks[i]);
    }
}

void World::handleEvent(const sf::Event& event)
{
    float xDir = 0.f;
    float yDir = 0.f;

    switch (event.type) {
    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
            xDir -= 1.f;
        }
        if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
            xDir += 1.f;
        }
        if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
            yDir -= 1.f;
        }
        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
            yDir += 1.f;
        }
        break;
        //FIXME:
//
//    case sf::Event::KeyReleased:
//        if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
//            xDir += 1.f;
//        }
//        if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
//            xDir -= 1.f;
//        }
//        if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
//            yDir += 1.f;
//        }
//        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
//            yDir -= 1.f;
//        }
//        break;
    }

    std::cout << "moving player by xDir: " << xDir << " yDir: " << yDir << std::endl;
    //FIXME: bring in elapsedTime here ...

    m_player->move(xDir, yDir);
    m_view->setCenter(m_player->getPosition());
}

void World::update()
{
    sf::Vector2f center = m_view->getCenter();
    std::cout << "VIEWPORT: view x: " << center.x << " View y: " << center.y << std::endl;
    const sf::Vector2f playerPosition = m_player->getPosition();
    std::cout << "VIEWPORT: view x: " << center.x << " View y: " << center.y << std::endl;

    int tilesBeforeX = center.x / 16;
    int tilesBeforeY = center.y / 16;
    std::cout << "tilesbeforeX: " << tilesBeforeX << " tilesbeforeY: " << tilesBeforeY << std::endl;

}

void World::loadMap()
{
    std::cout << "loading map!" << std::endl;
    std::cout << "SIZEOF BLOCK: " << sizeof(Block) << " BYTES!" << "MAX SIZE (so far): 8400*2400*sizeof = " << ((WORLD_ROWCOUNT * WORLD_COLUMNCOUNT*sizeof(Block))/(pow(10.0, 6.0))) << " MiB!" << std::endl;
    std::cout << "SIZEOF m_blocks: " << sizeof(m_blocks)/1e6 << std::endl;
    std::cout << "ALIGNOF BLOCK: " << alignof(Block) << std::endl;
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
