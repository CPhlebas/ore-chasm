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

#ifndef CLOUDSYSTEM_H
#define CLOUDSYSTEM_H

#include "renderable.h"

const int CLOUDS_COUNT = 5;

class CloudSystem
{
public:
    CloudSystem(sf::RenderWindow *window, sf::View *view);

    void update();
    void render();


private:
    sf::RenderWindow *m_window;
    sf::View *m_view;

    Texture *m_clouds[CLOUDS_COUNT];
    const float m_windspeed = 1.f;//0.0005f;
};

#endif
