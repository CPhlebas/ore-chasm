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

#include "resourcemanager.h"

#include <map>
#include <iostream>

#include <allegro5/allegro.h>

static ResourceManager* s_instance = 0;

ResourceManager::ResourceManager() : m_bitmaps(), m_resourceDirs()
{
}

ResourceManager::~ResourceManager()
{
    m_bitmaps.clear();
    m_resourceDirs.clear();
}

ResourceManager* ResourceManager::instance()
{
    if (!s_instance) {
        s_instance = new ResourceManager();
    }

    return s_instance;
}

ALLEGRO_BITMAP* ResourceManager::loadTexture(const std::string& filename)
{
    // Check, whether the image already exists
    for (std::map<std::string, ALLEGRO_BITMAP*>::const_iterator it = m_bitmaps.begin();
            it != m_bitmaps.end();
            ++it) {
        if (filename == it->first) {
            std::cout << "DEBUG_MESSAGE: " << filename << " using existing texture.\n";
            return it->second;
        }
    }

    ALLEGRO_BITMAP *bitmap = nullptr;

    // Search project's main directory
    if (al_load_bitmap(filename.c_str())) {
        m_bitmaps[filename] = bitmap;
        std::cout << "DEBUG_MESSAGE: " << filename << " loading new texture.\n";
        return m_bitmaps[filename];
    }

    // If the image has still not been found, search all registered directories
    for (std::vector<std::string>::iterator it = m_resourceDirs.begin(); it != m_resourceDirs.end(); ++it) {
        std::string fullName;
        fullName << (*it);
        fullName << filename;

        if (al_load_bitmap(fullName.c_str())) {
            m_bitmaps[filename] = bitmap;
            std::cout << "DEBUG_MESSAGE: " << filename << " loading texture.\n";
            return m_bitmaps[filename];
        }
    }

    std::cout << "GAME_ERROR: Texture was not found. It is filled with an empty texture.\n";

    m_bitmaps[filename] = bitmap;
    return m_bitmaps[filename];
}

void ResourceManager::deleteTexture(const sf::Texture& texture)
{
    for (std::map<std::string, ALLEGRO_BITMAP*>::const_iterator it = m_bitmaps.begin();
            it != m_bitmaps.end();
            ++it) {
        if (&texture == &it->second) {
            m_bitmaps.erase(it);
            return;
        }
    }
}

void ResourceManager::deleteTexture(const std::string& filename)
{
    std::map<std::string, ALLEGRO_BITMAP*>::const_iterator it = m_bitmaps.find(filename);
    if (it != m_bitmaps.end())
        m_bitmaps.erase(it);
}

void ResourceManager::addResourceDir(const std::string& directory)
{
    // Check whether the path already exists
    for (std::vector<std::string>::const_iterator it  = m_resourceDirs.begin(); it != m_resourceDirs.end(); ++it) {
        // The path exists. So it isn't necessary to add id once more.
        if (directory == (*it))
            return;
    }

    // insert the directory
    m_resourceDirs.push_back(directory);
}

void ResourceManager::removeResourceDir(const std::string& directory)
{
    for (std::vector<std::string>::iterator it  = m_resourceDirs.begin(); it != m_resourceDirs.end();) {
        // The path exists. So it isn't necessary to add id once more.
        if (directory == (*it)) {
            it = m_resourceDirs.erase(it);
        } else {
            ++it;
        }
    }
}
