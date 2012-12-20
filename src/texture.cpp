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

#include "texture.h"
#include "imagemanager.h"
#include "game.h"

#include <Eigen/Core>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

Texture::Texture()
{
    m_imageManager = ImageManager::instance();
}

Texture::Texture(const char* texture)
{
    m_imageManager = ImageManager::instance();
    Texture::setTexture(texture);
}

void Texture::setTexture(const char* texture)
{
    sf::Sprite::setTexture(m_imageManager->loadTexture(texture));

    if (DEBUG_RENDERING) {
        // make things transparent so we can see, also tint them magenta
        sf::Color color(255, 0, 255, 180);
//        sf::Sprite::setColor(color);
    }
}

void Texture::draw_bitmap(int flags)
{
    const sf::Vector2u size = sf::Sprite::getTexture()->getSize();
    const Eigen::Vector2f rectSize(size.x, size.y);
    const Eigen::Vector2f pos = sf::Sprite::getPosition();
    const sf::IntRect rect = getTextureRect();
    const Eigen::Vector2f center = Eigen::Vector2f(rect.width * 0.5, rect.height * 0.5);

    al_draw_bitmap(m_bitmap, m_position.x(), m_position.y(), flags);

    renderDebugDrawing();
}

void Texture::renderDebugDrawing()
{
    ALLEGRO_COLOR color = al_map_rgb(0, 255, 0);
    al_draw_rectangle(m_position.x(), m_position.y(), m_position.x() + size().x(), m_position.y() + size().y(), color, 1.0f);
}

Eigen::Vector2i Texture::size() const
{
    return Eigen::Vector2i(al_get_bitmap_height(m_bitmap), al_get_bitmap_width(m_bitmap));
}
