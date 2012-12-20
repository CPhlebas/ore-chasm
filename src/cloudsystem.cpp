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

#include "cloudsystem.h"

#include "game.h"

CloudSystem::CloudSystem(sf::RenderWindow *window, sf::View *view) : m_window(window), m_view(view)
{
    for (int i = 0; i < CLOUDS_COUNT; ++i) {
        m_clouds[i] = new Texture("../textures/cloud1.png");
        Texture *cloud = m_clouds[i];
        cloud->setOrigin(cloud->getTextureRect().width * 0.5, cloud->getTextureRect().height * 0.5);
        cloud->setPosition(i * 300.0, 50.0);
    }
}

void CloudSystem::update()
{
    for (int i = 0; i < CLOUDS_COUNT; ++i) {
        Texture *cloud = m_clouds[i];

        if (cloud->getPosition().x + cloud->getTextureRect().width < SCREEN_W) {
            cloud->move(m_windspeed, 0.0f);
        } else {
            cloud->setPosition(-cloud->getTextureRect().width, cloud->getPosition().y);
        }

        const float offset = (SCREEN_H * 0.5) - m_view->getCenter().y ;
        cloud->setPosition(cloud->getPosition().x,  cloud->getTextureRect().height + offset);
    }
}

void CloudSystem::render()
{
    //FIXME: render to a sprite instead of doing a ton of draw clouds for all clouds

    for (int i = 0; i < CLOUDS_COUNT; ++i) {
        Texture *cloud = m_clouds[i];
        m_window->draw(*cloud);
        cloud->render(m_window);
    }
}

