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
#include <assert.h>

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


    //should indicate no error in loading process.
    assert(result.status == 0);
    std::cout << "Load result: " << result.description() << std::endl;

    // <map>
    pugi::xml_node map = doc.first_child();
    std::cout << "Map loaded. Level info: { ######################## " << std::endl
    << "height: " << map.attribute("height").as_int() << std::endl
    << "width: " << map.attribute("width").as_int() << std::endl
    << "tileWidth: " << map.attribute("tileWidth").as_int() << std::endl
    << "tileHeight: " << map.attribute("tileHeight").as_int() << std::endl
    << "} #########################################" << std::endl;

    //iterate through <tileset>'s which define what each tile actually is supposed to be (file name and what not)
//    pugi::xml_node tileset = map.child();
 //  std::cout << "TEST: " << tileset.name() << std::endl;
 // std::cout << "TEST: " << tileset.name() << std::endl;
    std::cout << "Loading tileset nodes" << std::endl;
    for (pugi::xml_node tileset = map.child("tileset"); tileset; tileset = map.next_sibling("tileset"))
    {
        //we don't actually use tileset, just the <image> inside them.
        pugi::xml_node image = tileset.first_child();
        std::cout << "image: " << image.attribute("height") << std::endl;
    }
    std::cout << sizeof(int) << std::endl;

}
