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

#ifndef BLOCK_H
#define BLOCK_H

#include <map>
#include <string>

class Block
{
public:
    Block();

    /**
     * Function is called ONLY when this block is considered NOT paused, aka alive.
     * So  only when this block is onscreen or close to it will this function be called.
     */
    void update();

    /**
     * Determines the health and texture of the Block.
     * NOTE: MUST be in sync with index of m_blockTextures
     */
    enum class BlockType {
        Null = 0,
        Dirt,
        Stone,
        Grass
    };

    // height is the same as width (they're square)
    static constexpr unsigned char blockSize = 16;

    struct BlockStruct {
        BlockStruct(const char *_texture, bool _collides) { texture = _texture; collides = _collides; };
        const char* texture;

        // I thought about using flags..but this seems better, save for the might-be-fucking-huge-constructor
        // this will be useful for TODO: blocks that hurt or help the player's health, etc. (lava), liquids of types, etc.
        bool collides : 1;

        //TODO: animations..array of textures for animation..for destroying and other shit
    };

    static std::map<BlockType, BlockStruct> blockTypeMap;

    /**
     * A lookup table to represent the 0-255 possibilities for a tile being surrounded,
     * as 47 possible different sprites.
     * for more info, see http://forums.tigsource.com/index.php?topic=9859.15
     * http://www.saltgames.com/2010/a-bitwise-method-for-applying-tilemaps/
     * http://www.gamedev.net/page/resources/_/technical/game-programming/tilemap-based-game-techniques-handling-terrai-r934
     * http://www.angryfishstudios.com/2011/04/adventures-in-bitmasking/
     */
    static std::map<unsigned char, unsigned char> tileMeshingTable;

    /**
     * 0-255, 0 obviously meaning this block is marked as to be destroyed.
     * higher values are for more "difficult" to break block types. e.g. sand has a very low number.
     */
    unsigned char health = 255;

    /**
     * The type of tile this is, 0-255 is valid and can be compared with the world's definition of tile types
     * (an enum)
     */
    unsigned char type = 0;
};

#endif
