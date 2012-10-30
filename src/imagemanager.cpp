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

#include "imagemanager.h"

#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>

static ImageManager* s_instance = 0;

ImageManager::ImageManager() : textures(), resourceDirs()
{
}

ImageManager::~ImageManager()
{
    textures.clear();
    resourceDirs.clear();
}

ImageManager* ImageManager::instance()
{
    if (!s_instance) {
        s_instance = new ImageManager();
    }

    return s_instance;
}

const sf::Texture& ImageManager::loadTexture( const std::string& filename )
{
    // Check, whether the image already exists
    for( std::map<std::string, sf::Texture>::const_iterator it = textures.begin();
            it != textures.end();
            ++it)
    {
        if( filename == it->first )
        {
            std::cout << "DEBUG_MESSAGE: " << filename << " using existing texture.\n";
            return it->second;
        }
    }

    // The image doesen't exists. Create it and save it.
    sf::Texture texture;

    // Search project's main directory
    if( texture.loadFromFile( filename ) )
    {
        textures[filename] = texture;
        std::cout << "DEBUG_MESSAGE: " << filename << " loading new texture.\n";
        return textures[filename];
    }

    // If the image has still not been found, search all registered directories
    for( std::vector<std::string>::iterator it = resourceDirs.begin();
            it != resourceDirs.end();
            ++it )
    {
        if( texture.loadFromFile( (*it) + filename ) )
        {
            textures[filename] = texture;
            std::cout << "DEBUG_MESSAGE: " << filename << " loading texture.\n";
            return textures[filename];
        }

    }

    std::cout << "GAME_ERROR: Texture was not found. It is filled with an empty texture.\n";
    textures[filename] = texture;
    return textures[filename];
}

void ImageManager::deleteTexture( const sf::Texture& texture )
{
    for( std::map<std::string, sf::Texture>::const_iterator it = textures.begin();
            it != textures.end();
            ++it)
    {
        if( &texture == &it->second )
        {
            textures.erase( it );
            return;
        }
    }
}

void ImageManager::deleteTexture( const std::string& filename )
{
    std::map<std::string, sf::Texture>::const_iterator it = textures.find( filename );
    if( it != textures.end() )
        textures.erase( it );
}

void ImageManager::addResourceDir( const std::string& directory )
{
    // Check whether the path already exists
    for( std::vector<std::string>::const_iterator it  = resourceDirs.begin();
            it != resourceDirs.end();
            ++it )
    {
        // The path exists. So it isn't necessary to add id once more.
        if( directory == (*it) )
            return;
    }

    // insert the directory
    resourceDirs.push_back( directory );
}

void ImageManager::removeResourceDir( const std::string& directory )
{
    for( std::vector<std::string>::iterator it  = resourceDirs.begin();
            it != resourceDirs.end(); )
    {
        // The path exists. So it isn't necessary to add id once more.
        if( directory == (*it) )
            it = resourceDirs.erase( it);
        else
            ++it;
    }
}
