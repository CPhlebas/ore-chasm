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

Sky::Sky(sf::RenderWindow *window, sf::View *view, float height) :
    m_window(window),
    m_view(view),
    m_height(height)
{
    m_sunSprite.setTexture("../textures/sun.png");
    m_sunSprite.setOrigin(m_sunSprite.getTextureRect().width * 0.5, m_sunSprite.getTextureRect().height * 0.5);
}

void Sky::update(const float elapsedTime)
{
}

void Sky::render()
{
    m_window->draw(m_sunSprite);
    m_sunSprite.render(m_window);
}
