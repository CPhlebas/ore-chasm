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

ImageManager::ImageManager() : images(), resourceDirs()
{
}

ImageManager::~ImageManager()
{
        images.clear();
        resourceDirs.clear();
}

ImageManager* ImageManager::instance()
{
    if (!s_instance) {
        s_instance = new ImageManager();
    }

    return s_instance;
}

const sf::Image& ImageManager::loadImage( const std::string& filename )
{
        // Check, whether the image already exists
        for( std::map<std::string, sf::Image>::const_iterator it = images.begin();
                 it != images.end();
                 ++it)
        {
                if( filename == it->first )
                {
                        std::cout << "DEBUG_MESSAGE: " << filename << " using existing image.\n";
                        return it->second;
                }
        }

        // The image doesen't exists. Create it and save it.
        sf::Image image;

        // Search project's main directory
        if( image.LoadFromFile( filename ) )
        {
                images[filename] = image;
                std::cout << "DEBUG_MESSAGE: " << filename << " loading image.\n";
                return images[filename];
        }

        // If the image has still not been found, search all registered directories
        for( std::vector<std::string>::iterator it = resourceDirs.begin();
                 it != resourceDirs.end();
                 ++it )
        {
                if( image.LoadFromFile( (*it) + filename ) )
                {
                        images[filename] = image;
                        std::cout << "DEBUG_MESSAGE: " << filename << " loading image.\n";
                        return images[filename];
                }

        }

        std::cout << "GAME_ERROR: Image was not found. It is filled with an empty image.\n";
        images[filename] = image;
        return images[filename];
}

void ImageManager::deleteImage( const sf::Image& image )
{
        for( std::map<std::string, sf::Image>::const_iterator it = images.begin();
                 it != images.end();
                 ++it)
        {
                if( &image == &it->second )
                {
                        images.erase( it );
                        return;
                }
        }
}

void ImageManager::deleteImage( const std::string& filename )
{
        std::map<std::string, sf::Image>::const_iterator it = images.find( filename );
        if( it != images.end() )
                images.erase( it );
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