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
#include <assert.h>

#include <allegro5/allegro.h>

#include <Eigen/Core>

Player::Player(const char* texture) : Entity(texture)
{
    const sf::IntRect rect = Texture::getTextureRect();
    const Eigen::Vector2f center = Eigen::Vector2f(rect.width * 0.5, rect.height * 0.5);
    setOrigin(center);
}

void Player::render(ALLEGRO_DISPLAY *display)
{
    Texture::render(window);

    //debug drawing for the radius that is within the player's reach to "pick"
    sf::RectangleShape rect = sf::RectangleShape(Eigen::Vector2f(Player::blockPickingRadius * 2.0f, Player::blockPickingRadius * 2.0f));
    rect.setPosition(getPosition().x - Player::blockPickingRadius, getPosition().y - Player::blockPickingRadius);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(1.0f);
    window->draw(rect);
}

void Player::handleEvent(const ALLEGRO_EVENT& event)
{
    switch (event.type) {
        case ALLEGRO_EVENT_KEY_DOWN:
        if (event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
            m_inputXDirection = 1.f;
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
            m_inputXDirection = -1.f;
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
            m_inputYDirection = 1.f;
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_W || event.keyboard.keycode == ALLEGRO_KEY_UP) {
            m_inputYDirection = -1.f;
        }
        break;

        case ALLEGRO_EVENT_KEY_UP:
        if (event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
            m_inputXDirection = 0.f;
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
            m_inputXDirection = 0.f;
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
            m_inputYDirection = 0.f;
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_W || event.keyboard.keycode == ALLEGRO_KEY_UP) {
            m_inputYDirection = 0.f;
        }
        break;
    }

    Entity::setVelocity(m_inputXDirection, m_inputYDirection);
}
