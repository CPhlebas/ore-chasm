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

#include "renderable.h"
#include "imagemanager.h"
#include "game.h"

#include <SFML/System/Vector2.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Shader.hpp>

Renderable::Renderable(const char* texture)
{
    m_imageManager = ImageManager::instance();
    Renderable::setTexture(texture);
}

void Renderable::setTexture(const char* texture)
{
    sf::Sprite::setTexture(m_imageManager->loadTexture(texture));

    if (DEBUG_RENDERING) {
        // make things transparent so we can see, also tint them magenta
        sf::Color color(255, 0, 255, 180);
//        sf::Sprite::setColor(color);
    }
}

void Renderable::render(sf::RenderWindow* window)
{
    const sf::Vector2u size = sf::Sprite::getTexture()->getSize();
    const sf::Vector2f rectSize(size.x, size.y);
    const sf::Vector2f pos = sf::Sprite::getPosition();
    const sf::IntRect rect = getTextureRect();
    const sf::Vector2f center = sf::Vector2f(rect.width * 0.5, rect.height * 0.5);

    sf::RectangleShape outline;
    outline.setSize(rectSize);
    outline.setOutlineColor(sf::Color::Green);
    outline.setOutlineThickness(1.0f);
    outline.setFillColor(sf::Color(0, 0, 0, 0));
    outline.setPosition(pos);
    outline.setOrigin(center);

    window->draw(outline);
}
