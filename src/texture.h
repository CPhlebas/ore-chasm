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

#ifndef TEXTURE_H
#define TEXTURE_H

#include "imagemanager.h"

#include <Eigen/Core>

class ALLEGRO_DISPLAY;
class ALLEGRO_BITMAP;

class Texture
{
public:
    Texture();

    /**
     * A wrapper around ALLEGRO_BITMAP which automagically handles loading and
     * using the resourcemanager.
     * The default origin is the center of the texture.
     * @see setOrigin origin
     */
    Texture(const char* texture);

    /**
     * Pass a string into @p texture and it will automatically ask the ResourceManager
     * for an already-loaded version, if possible. If not, it will load it.
     *
     */
    void setTexture(const char* texture);

    /**
     * Wrapper of al_draw_bitmap.
     * Draws this texture's internal bitmap to whatever the current target is, at the current position.
     * Fucking magic.
     * NOTE: automatically handles debug drawing around it, if it is enabled by DEBUG_DRAWING global.
     * @p flags same as al_draw_bitmap: ALLEGRO_FLIP_HORIZONTAL and/or ALLEGRO_FLIP_VERTICAL
     */
    virtual void draw_bitmap(int flags = 0);

    /**
     * Returns the width and height of the bitmap of this Texture, as a Vector2i.
     */
    Eigen::Vector2i size() const;

    /**
     * Sets the origin of this texture, to which to offset drawing.
     * Default is the center of the texture.
     */
    void setOrigin(const Eigen::Vector2f origin);

    Eigen::Vector2f origin() const;

private:
    void renderDebugDrawing();

private:
    ImageManager* m_imageManager = nullptr;

    ALLEGRO_BITMAP *m_bitmap = nullptr;

    Eigen::Vector2f m_position = Eigen::Vector2f(0.0f, 0.0f);
    Eigen::Vector2f m_origin;
};

#endif
