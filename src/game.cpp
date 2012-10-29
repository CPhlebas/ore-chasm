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

#include "game.h"

Game::Game()
{
}

void Game::abort_game(const char* message)
{
    printf("%s \n", message);
    exit(1);
}

void Game::init()
{


    // cap tickrate to FPS
    timer = al_create_timer(1.0 / FPS);


    image = al_load_bitmap("../textures/stone.png");


    done = false;
    tick();
    shutdown();
}

void Game::tick()
{
    bool redraw = true;
    al_start_timer(timer);

    while (!done) {

        if (event.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
            //update_logic();
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                done = true;
            }
            //get_user_input();
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(255, 0, 0));
            al_draw_bitmap(image, 200, 200, 0);
            //update_graphics();
            al_flip_display();
        }
    }
}

void Game::shutdown()
{

}
