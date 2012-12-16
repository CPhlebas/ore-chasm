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

#include "player.h"

#include "imagemanager.h"
#include "debug.h"
#include <assert.h>

Player::Player(const char* texture) : Entity(texture)
{
    const sf::IntRect rect = Renderable::getTextureRect();
    const sf::Vector2f center = sf::Vector2f(rect.width * 0.5, rect.height * 0.5);
    setOrigin(center);
}

void Player::render(sf::RenderWindow* window)
{
    Renderable::render(window);

    //debug drawing for the radius that is within the player's reach to "pick"
    sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(Player::blockPickingRadius * 2.0f, Player::blockPickingRadius * 2.0f));
    rect.setPosition(getPosition().x - Player::blockPickingRadius, getPosition().y - Player::blockPickingRadius);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(1.0f);
    window->draw(rect);
}

void Player::handleEvent(const sf::Event& event)
{
    switch (event.type) {
    case sf::Event::KeyPressed:
        checkInput();
        break;

    case sf::Event::KeyReleased:
        checkInput();
        break;
    }

    Entity::setVelocity(sf::Vector2f(m_inputXDirection * 100, m_inputYDirection * 100));
}

void Player::checkInput()
{
    const bool left = (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left));
    const bool right = (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
    const bool up = (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up));
    const bool down = (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down));

    m_inputXDirection = right - left;
    m_inputYDirection = down - up;
}
