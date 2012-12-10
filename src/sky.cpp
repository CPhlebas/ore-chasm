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
#include "time.h"
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

    const unsigned char hour = Time::instance()->currentHour();
    const unsigned char minute = Time::instance()->currentMinute();
    std::cout << "MINUTE! " << (short) minute << "\n";

    m_timeAngle = (hour * (180 / 12)) + (minute * (180 / 12) / 60 );

    const double angle = (m_timeAngle + 180) * (M_PI / 180);
    const float newX = _viewportCenter.x + cos(angle) * 400;
    const float newY = _viewportCenter.y + sin(angle) * 400;
    m_sunMoonPosition = sf::Vector2f(newX, newY);
    m_sunSprite.setPosition(m_sunMoonPosition);
}

void Sky::render()
{
    sf::VertexArray line(sf::Lines, 2);
    sf::Vector2f screen = sf::Vector2f(SCREEN_W/2, SCREEN_H/2);
    line.append(sf::Vertex(screen));
    line.append(sf::Vertex(m_sunMoonPosition));
    m_window->draw(line);

    // sunrise at 7 am, sunset at 7am
    const unsigned char hour = Time::instance()->currentHour();

    // 7am, 7pm
    if (hour >= 7 && hour <= 19) {
        m_window->draw(m_sunSprite);
        m_sunSprite.render(m_window);
    }
}
