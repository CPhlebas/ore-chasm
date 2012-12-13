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

#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "block.h"

#include <SFML/Graphics.hpp>

class Player : public Entity
{
public:
    /**
     * Create a player with the starting texture @p texture
     * the ImageLoader is used automatically, so just pass a path
     * to the image to load.
     */
    Player(const char* texture);

    void handleEvent(const sf::Event& event);

    // radius indicating how many pixels out the player can pick
    // blocks
    static constexpr float blockPickingRadius = Block::blockSize * 8.0f;
    static constexpr float movementSpeed = 1000.0f;

    void render(sf::RenderWindow *window);

private:
    unsigned char health = 100;
    float m_inputXDirection = 0.0f;
    float m_inputYDirection = 0.0f;
};


#endif
