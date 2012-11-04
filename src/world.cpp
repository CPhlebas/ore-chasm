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

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <pugixml.hpp>

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

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("../../level1.tmx");


    std::cout << "Load result: " << result.description()
    << ", mesh name: " << doc.child("mesh").attribute("name").value() << std::endl;
}
