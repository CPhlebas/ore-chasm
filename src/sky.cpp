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

#include "sky.h"

#include "cloudsystem.h"
#include "game.h"
#include <iostream>
#include <stdio.h>
#include <cmath>

Sky::Sky(sf::RenderWindow *window, sf::View *view, float height) :
m_window(window),
m_view(view),
m_height(height)
{
    m_sunSprite.setTexture("../textures/sun.png");
    m_sunSprite.setOrigin(m_sunSprite.getTextureRect().width * 0.5, m_sunSprite.getTextureRect().height * 0.5);
}

void Sky::update()
{
    sf::Vector2f _viewportCenter;
    _viewportCenter.x = SCREEN_W / 2;
    _viewportCenter.y = SCREEN_H / 2;

    sf::Vector2f diffVect;
    diffVect.x = _viewportCenter.x;
    diffVect.y = _viewportCenter.y;

    m_timeAngle = m_hour * 15;

//    const double angle = atan2(diffVect.y, diffVect.x);
//    const double angle = (m_timeAngle) * (M_PI / 180);
    const double angle = (m_timeAngle) * (M_PI / 180);
    std::cout << "angle: " << angle << " timeangle: " << m_timeAngle << "\n";
    const float newX = _viewportCenter.x + cos(angle) * 200;
    const float newY = _viewportCenter.y  + sin(angle) * 200;
    m_sunMoonPosition = sf::Vector2f(newX, newY);
    std::cout << "x: " << newX << " y: " << newY << " \n";

    m_hour += 1;
    if (m_hour > 12) {
        m_hour = 0;
    }
//    m_timeAngle += 0.05;
//    if (m_timeAngle > 180) {
//        m_timeAngle = 0;
//    }
}

void Sky::render()
{
    sf::VertexArray line(sf::Lines, 2);
    sf::Vector2f screen = sf::Vector2f(SCREEN_W/2, SCREEN_H/2);
    line.append(sf::Vertex(screen));
    line.append(sf::Vertex(m_sunMoonPosition));
    m_window->draw(line);

    m_window->draw(m_sunSprite);
    m_sunSprite.render(m_window);
}
