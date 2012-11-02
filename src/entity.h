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

#ifndef ENTITY_H
#define ENTITY_H

#include "renderable.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderStates.hpp>

const float gravity = -9.8f;

class Entity : public Renderable
{
public:
    /**
     * Create an entity with the given texture @p texture
     * texture loading is handled automatically via textureManager.
     */
    Entity(const char* texture);

    virtual void update();

    //FIXME: isolate and move further up the chain, and make Renderable privately inherited?
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

    /**
     * Use only to reset the entities position to some other place.
     * ALL OTHER CASES USE VELOCITY. 
     * Upon calling update, it will move it by that vector, as well as integrate
     * gravity.
     * @see setVelocity
     * @see update
     */
    void setPosition(float x, float y);
    void setPosition(sf::Vector2f vect) { };

    /**
     * Sets the velocity that this entity should have. It is affected by gravity,
     * and will modify the position automatically when Entity::update() is called.
     */
    void setVelocity(const sf::Vector2f& velocity);

private:

    sf::Vector2f m_velocity;
};

#endif
