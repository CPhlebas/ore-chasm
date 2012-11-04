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

World::World()
{
    loadMap();
    saveMap();
}

void World::render()
{

}

void World::update()
{

}

void World::loadMap()
{
    std::cout << "loading map!" << std::endl;
    std::cout << "SIZEOF BLOCK: " << sizeof(Block) << " BYTES!" << "MAX SIZE (so far): 8400*2400*sizeof = " << ((8400*2400*sizeof(Block))/(pow(10.0, 6.0))) << " MiB!" << std::endl;
    generateMap();
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
            color.r = m_blocks[column * WORLD_ROWCOUNT + row].type;
            image.setPixel(row, column, color);
        }
    }

    bool saved = image.saveToFile("../../saves/level1.png");
    assert(saved);

    const int elapsedTime = clock.getElapsedTime().asMilliseconds();
    std::cout << "Time taken for map saving: " << elapsedTime << " Milliseconds" << std::endl;

}
