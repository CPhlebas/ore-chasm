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

CloudSystem::CloudSystem(sf::RenderWindow *window) : m_window(window)
{
    m_cloud1 = new Renderable("../textures/cloud1.png");
    m_cloud1->setOrigin(m_cloud1->getTextureRect().width * 0.5, m_cloud1->getTextureRect().height * 0.5);
    m_cloud1->setPosition(100.0, 100.0);
}

void CloudSystem::update()
{

}

void CloudSystem::render()
{
    m_window->draw(*m_cloud1);
    m_cloud1->render(m_window);
}

