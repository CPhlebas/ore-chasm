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
    sf::CircleShape circle = sf::CircleShape(Player::blockPickingRadius);
    circle.setPosition(getPosition());
    circle.setOrigin(Player::blockPickingRadius, Player::blockPickingRadius);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Red);
    circle.setOutlineThickness(1.0f);
    window->draw(circle);
}
