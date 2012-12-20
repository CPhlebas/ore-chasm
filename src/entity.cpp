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

#include "entity.h"

#include "world.h"
#include "debug.h"

#include <Eigen/Core>

Entity::Entity(const char* texture) : Renderable(texture)
{
}

//Entity::Entity(): Renderable("../textures/error.png")
//{
//}

void Entity::setVelocity(const Eigen::Vector2f& velocity)
{
    m_velocity = velocity;
}

void Entity::update(const float elapsedTime)
{
//    m_velocity.y += GRAVITY;

    Eigen::Vector2f dest = Eigen::Vector2f(m_velocity.x() * elapsedTime, m_velocity.y() * elapsedTime);
    dest.x() += getPosition().x();
    dest.y() += getPosition().y();

    if (!World::instance()->isTileSolid(dest)) {
        Renderable::setPosition(dest);
    }
}

void Entity::setPosition(float x, float y)
{
    Renderable::setPosition(x, y);
}

void Entity::setPosition(const Eigen::Vector2f& vect)
{
    Renderable::setPosition(vect);
}

