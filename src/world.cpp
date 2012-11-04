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
#include <assert.h>
#include <math.h>

#include <SFML/Graphics/Drawable.hpp>

World::World()
{
    loadMap();
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

}
