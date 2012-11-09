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

#ifndef GAME_H
#define GAME_H

#include "world.h"

#include <stdio.h>
#include <stdlib.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>

#include <GLConsole/GLConsole.h>


static bool DEBUG_RENDERING = true;

class Game
{
public:
    Game();

    void abort_game(const char* message);
    void init();
    void tick();
    void shutdown();

    const float FPS = 60.0;
    const int SCREEN_W = 1600;
    const int SCREEN_H = 900;

private:
    World *m_world = nullptr;
    GLConsole *m_console = nullptr;
    sf::RenderWindow *m_app = nullptr;
    sf::View *m_view = nullptr;
    sf::Font *m_font = nullptr;
};


#endif
