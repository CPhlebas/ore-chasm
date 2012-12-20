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

class ALLEGRO_DISPLAY;

class Texture
{
public:
    Texture();

    /**
     * A wrapper around ALLEGRO_BITMAP which automagically handles loading and
     * using the resourcemanager.
     */
    Texture(const char* texture);

    /**
     * Pass a string into @p texture and it will automatically ask the ResourceManager
     * for an already-loaded version, if possible. If not, it will load it.
     *
     */
    void setTexture(const char* texture);

    /**
     * important ONLY for debug rendering, so there's less duplication..
     * CALL THIS AFTER al_draw* methods...
     */
    virtual void render(ALLEGRO_DISPLAY *display);

private:
    ImageManager* m_imageManager = nullptr;
};

#endif
