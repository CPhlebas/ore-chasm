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

Texture::Texture(const char* texture)
{
    m_imageManager = ImageManager::instance();
    Texture::setTexture(texture);

    const Eigen::Vector2f texSize = size();
    m_origin = Eigen::Vector2f(texSize.x() * 0.5, texSize.y() * 0.5);
}

void Texture::setTexture(const char* texture)
{
}

void Texture::draw_bitmap(int flags)
{
    const sf::Vector2u size = sf::Sprite::getTexture()->getSize();
    const Eigen::Vector2f rectSize(size.x, size.y);
    const Eigen::Vector2f pos = sf::Sprite::getPosition();
    const sf::IntRect rect = getTextureRect();
    const Eigen::Vector2f center = Eigen::Vector2f(rect.width * 0.5, rect.height * 0.5);

    al_draw_bitmap(m_bitmap, m_position.x() - m_origin.x(), m_position.y() - m_origin.y(), flags);

    if (DEBUG_RENDERING) {
        renderDebugDrawing();
    }
}

void Texture::renderDebugDrawing()
{
    ALLEGRO_COLOR color = al_map_rgb(0, 255, 0);
    al_draw_rectangle(m_position.x() - m_origin.x(), m_position.y() - m_origin.y(), m_position.x() + size().x() - m_origin.x(), m_position.y() + size().y() - m_origin.y(), color, 1.0f);
}

Eigen::Vector2i Texture::size() const
{
    return Eigen::Vector2i(al_get_bitmap_height(m_bitmap), al_get_bitmap_width(m_bitmap));
}

void Texture::setOrigin(const Eigen::Vector2f origin)
{
    m_origin = origin;
}
