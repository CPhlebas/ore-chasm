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

Entity::Entity(const char* texture) : Renderable(texture)
{
}

//Entity::Entity(): Renderable("../textures/error.png")
//{
//}

void Entity::setVelocity(const sf::Vector2f& velocity)
{
    m_velocity = velocity;
}

void Entity::update(const float elapsedTime)
{
//    m_velocity.y += GRAVITY;

    // where we want to move
    sf::Vector2f dest = sf::Vector2f(m_velocity.x * elapsedTime, m_velocity.y * elapsedTime);
    dest.x += getPosition().x;
    dest.y += getPosition().y;

    // all tiles to the left, then right, top, bottom of entity
    // if a single one is solid, we cannot move in said direction
//    rect.intersects()

    const sf::Vector2f position = getPosition();

    //consider block map as starting at player pos == 0,0 and going down and to the right-ward
    //tilesBefore{X,Y} is only at the center of the view though..find the whole screen real estate
    // which is why startRow etc add and subtract half the screen
    //column
    int tilesBeforeX = position.x / Block::blockSize;
    //row
    int tilesBeforeY = position.y / Block::blockSize;

    const sf::IntRect textureRect = getTextureRect();

    const sf::Vector2f offset = World::instance()->tileOffset();

    const int startRow = tilesBeforeY - ((textureRect.height * 0.5 - offset.x) / Block::blockSize) - 1;
    const int endRow = tilesBeforeY + ((textureRect.height * 0.5) / Block::blockSize);

    //columns are our X value, rows the Y
    const int startColumn = tilesBeforeX - ((textureRect.width * 0.5 + offset.x) / Block::blockSize) - 1;
    const int endColumn = tilesBeforeX + ((textureRect.width * 0.5) / Block::blockSize);

    int currentRow = startRow;
    int currentColumn = startColumn;

    //left, const column, iterate over that row's cells
    for (; currentRow < endRow; ++currentRow) {
        if (World::instance()->isTileSolid(dest)) {
            //the tileposition, that is the top left of the tile. in theory.
            const float tileX = currentColumn * Block::blockSize;
            const float tileY = currentRow * Block::blockSize;

            sf::FloatRect tile;
            tile.left = tileX;
            tile.top = tileY;
            tile.height = Block::blockSize;
            tile.width = Block::blockSize;

            bool intersect = tile.intersects(getGlobalBounds());
            bool isSolid = World::instance()->isTileSolid(currentRow, currentColumn);

            Debug::log() << "intersects? : " << intersect << " isSolid? :" << isSolid << " offset x: " << offset.x << " offset y: " << offset.y;
        }
    }

//    if (dest.x != std::abs(dest.x))

    Renderable::setPosition(dest);
}

/*
 s *f::FloatRect tile_rect(0, 0, 16, 16);
 sf::FloatRect intersection_rect;
 std::for_each(tiles.begin(), tiles.end(), [&tile_rect, &player, &intersection_rect ]( const Tile& aTile ) {
     tile_rect.left = aTile.position.x;
     tile_rect.top = aTile.position.y;
     if( tile_rect.intersects( player.getRect(), intersection_rect ) == true )
     {
         // Probably also add here so it takes into account the player's size
         player.setPosition( findPosition( tile_rect, intersection_rect ) );
 }
 } );

 sf::Vector2f findPosition( sf::FloatRect aSource, sf::FloatRect anIntersection )
 {
     // Now we solve the position where to put the player
     // Which we will do by finding where on the rect we are hitting.
     if( aSource.left <= anIntersection.left )
         return sf::Vector2f( aSource.left, anIntersection.top );

     Etc, etc
 }
*/

void Entity::setPosition(float x, float y)
{
    Renderable::setPosition(x, y);
}

void Entity::setPosition(sf::Vector2f vect)
{
    Renderable::setPosition(vect);
}
