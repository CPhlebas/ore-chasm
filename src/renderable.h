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

#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "imagemanager.h"

#include <SFML/Graphics.hpp>

class Renderable : public sf::Sprite
{
public:
    Renderable();

    /**
     * Small basic wrapper which inherits an sf::Sprite and automagically
     * integrates itself with the RenderManager.
     */
    Renderable(const char* texture);

    /**
     * Pass a string into @p texture and it will automatically ask the ImageManager
     * for an already-loaded version, if possible. If not, it will load it.
     *
     * Internally, calls sf::Sprite::setImage with the texture it obtained.
     */
    void setTexture(const char* texture);

    /**
     * Use this if you generate an image/texture at runtime and need to set it.
     * This method is separate, so that you know that it is in fact, uncached and not
     * ref counted. Unlike ::setTexture(const char*);
     * Just so we don't accidentally start setting uncached ones everywhere
     */
    void setUncachedTexture(const sf::Texture& texture);

    /**
     * important ONLY for debug rendering, so there's less duplication..
     * CALL THIS AFTER m_window->draw(myrenderable)!!
     */
    void render(sf::RenderWindow *window);

private:
    /**
     * Hidden!! So that we don't screw up and start caching stuff outside, and not using
     * this API.
     *
     */
    void setTexture(const sf::Texture texture);

    ImageManager* m_imageManager = nullptr;
};

#endif
