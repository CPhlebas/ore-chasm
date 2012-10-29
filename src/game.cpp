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
    if (!al_init()) {
        abort_game("Failed to initialize allegro");
    }

    if (!al_install_keyboard()) {
        abort_game("Failed to install keyboard");
    }

    // cap tickrate to FPS
    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        abort_game("Failed to create main game loop timer");
    }

    if(!al_init_image_addon()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        abort_game("image addon loading failure");
    }

    al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        abort_game("Failed to create display");
    }

    image = al_load_bitmap("../textures/stone.png");

    event_queue = al_create_event_queue();
    if (!event_queue) {
        abort_game("Failed to create event queue");
        al_destroy_display(display);
    }

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));

    done = false;
    tick();
    shutdown();
}

void Game::tick()
{
    bool redraw = true;
    al_start_timer(timer);

    while (!done) {
        ALLEGRO_EVENT event;
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, 0.06);

        bool getEvent = al_wait_for_event_until(event_queue, &event, &timeout);

        if (getEvent && event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

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
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
}
