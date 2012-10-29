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

#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro5.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"

class Game
{
  public:
    Game();

    void abort_game(const char* message);
    void init();
    void tick();
    void shutdown();

    bool done;

    const float FPS = 60.0;
    const int SCREEN_W = 1600;
    const int SCREEN_H = 900;

    ALLEGRO_EVENT_QUEUE* event_queue = 0;
    ALLEGRO_TIMER* timer = 0;
    ALLEGRO_DISPLAY* display = 0;
    ALLEGRO_BITMAP *image = 0;
};

#endif
