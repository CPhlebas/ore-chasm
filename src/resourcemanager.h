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

#ifndef RESORUCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <stdlib.h>
#include <map>
#include <vector>

class ALLEGRO_BITMAP;

class ResourceManager
{
public:
    ResourceManager();
    static ResourceManager* instance();

    ALLEGRO_BITMAP* loadBitmap(const std::string& filename);
    void deleteBitmap(ALLEGRO_BITMAP *bitmap);
    void deleteBitmap(const std::string& filename);

    void addResourceDir(const std::string& directory);
    void removeResourceDir(const std::string& directory);

private:
    ~ResourceManager();

    std::map<std::string, ALLEGRO_BITMAP*> m_bitmaps;
    std::vector<std::string> m_resourceDirs;
};
#endif