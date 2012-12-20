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

#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <stdlib.h>

class ImageManager
{
public:
    ImageManager();
    static ImageManager* instance();

    const sf::Texture& loadTexture(const std::string& filename);
    void deleteTexture(const sf::Texture& texture);
    void deleteTexture(const std::string& filename);
    void addResourceDir(const std::string& directory);
    void removeResourceDir(const std::string& directory);

private:
    ~ImageManager();

    std::map<std::string, sf::Texture> textures;
    std::vector<std::string> resourceDirs;
};
#endif