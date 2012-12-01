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

class Block
{
public:
    Block();

    /**
     * Function is called ONLY when this block is considered NOT paused, aka alive.
     * So  only when this block is onscreen or close to it will this function be called.
     */
    void update();

    // height is the same as width (they're square)
    static constexpr unsigned char blockSize = 16;

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
