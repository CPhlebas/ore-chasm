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

//#include "world.h"

#include <stdio.h>
#include <stdlib.h>

class ALLEGRO_TIMER;
class ALLEGRO_EVENT_QUEUE;
class ALLEGRO_DISPLAY;
//1600
static int SCREEN_W = 1600;
//900
static int SCREEN_H = 900;

static bool DEBUG_RENDERING = true;

class Game
{
public:
    Game();
    ~Game();

    void abort_game(const char* message);
    void init();
    void tick();
    void shutdown();

    const float FPS = 60.0;

private:
    //World *m_world = nullptr;
    ALLEGRO_DISPLAY *m_display = nullptr;
    ALLEGRO_EVENT_QUEUE *m_events = nullptr;
    ALLEGRO_TIMER *m_timer = nullptr;
    bool m_running = true;
};


#endif
