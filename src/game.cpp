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

#include "debug.h"
//#include "entity.h"
//#include "imagemanager.h"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/keyboard.h>
#include <allegro5/mouse.h>

Game::Game()
{
}

Game::~Game()
{
}

void Game::abort_game(const char* message)
{
    printf("%s \n", message);
    shutdown();
    exit(1);
}

void Game::init()
{
    uint32_t version = al_get_allegro_version();
    int major = version >> 24;
    int minor = (version >> 16) & 255;
    int revision = (version >> 8) & 255;
    int release = version & 255;
    Debug::log(Debug::Area::System) << "Using allegro version: " << major << "." << minor << "." << revision << "." << release;

    Debug::fatal(al_init(), Debug::Area::System, "Failure to init allegro");

    al_init_font_addon();
    Debug::fatal(al_init_ttf_addon(), Debug::Area::System, "Failure to init ttf addon");
    Debug::fatal(al_init_image_addon(), Debug::Area::System, "Failure to init image addon");
    Debug::fatal(al_install_keyboard(), Debug::Area::System, "Failure to install keyboard");
    Debug::fatal(al_install_mouse(), Debug::Area::System, "Failure to install mouse");

    m_display = al_create_display(SCREEN_W, SCREEN_H);
    Debug::fatal(m_display, Debug::Area::Graphics, "display creation failed");

    version = al_get_opengl_version();
    major = version >> 24;
    minor = (version >> 16) & 255;
    revision = (version >> 8) & 255;

    std::cout << "\n\n\n\n";
    Debug::log(Debug::Area::Graphics) << "Hardware we're running on...";
    Debug::log(Debug::Area::Graphics) << major << "." << minor << "." << revision;

    int glVariant = al_get_opengl_variant();

    if (glVariant & ALLEGRO_DESKTOP_OPENGL) {
        Debug::log(Debug::Area::Graphics) << "Using desktop OpenGL variant.";
    } else if (glVariant & ALLEGRO_OPENGL_ES) {
        Debug::log(Debug::Area::Graphics) << "Using OpenGL ES OpenGL variant.";
    }

    Debug::log(Debug::Area::Graphics) << "Platform: Driver Vendor: " << glGetString(GL_VENDOR);
    Debug::log(Debug::Area::Graphics) << "Platform: Renderer: " << glGetString(GL_RENDERER);
    Debug::log(Debug::Area::Graphics) << "OpenGL Version: " << glGetString(GL_VERSION);
    Debug::log(Debug::Area::Graphics) << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);

    GLint textureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &textureSize);
    Debug::log(Debug::Area::Graphics) << "Maximum OpenGL texture size allowed: " << textureSize;
    std::cout << "\n\n\n\n";

    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);

    al_set_app_name("ore-chasm");
    al_set_window_title(m_display, "Ore Chasm");

    Debug::fatal(m_events = al_create_event_queue(), Debug::Area::System, "event queue init");

    al_register_event_source(m_events, al_get_display_event_source(m_display));
    al_register_event_source(m_events, al_get_mouse_event_source());
    al_register_event_source(m_events, al_get_keyboard_event_source());

    Debug::fatal(m_font = al_load_ttf_font("../font/Ubuntu-L.ttf", 12, 0), Debug::Area::System, "Failure to load font");

//    ImageManager* manager = ImageManager::instance();
//    manager->addResourceDir("../textures/");

    // World takes ownership of m_view
 //   World::createInstance(m_window, m_view);
//    m_world = World::instance();

    // ----------------------------------------------------- initial render --------------------------------------------------
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_flip_display();

    tick();
    shutdown();
}

void Game::tick()
{
    std::stringstream ss;
    std::string str;

    int fps = 0;

    const int MAX_BENCH = 300;
    int benchTime = MAX_BENCH;

    double oldTime = al_get_time();

    while (m_running) {
        double newTime = al_get_time();
        double delta = newTime - oldTime;
        double fps = 1 / (delta);
        oldTime = newTime;

        ALLEGRO_EVENT event;
        while (al_get_next_event(m_events, &event)) {
            m_world->handleEvent(event);
            switch (event.type) {
                // window closed
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                m_running = false;
                break;

                // key pressed
                case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    Debug::log(Debug::Area::System) << "shutting down...";
                    goto shutdown;
                }
                break;

                case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
                    //FIXME: pause game
                break;

                case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
                    //FIXME: resume game
                break;

                case ALLEGRO_EVENT_DISPLAY_HALT_DRAWING:
                    //FIXME: for android
                    break;

                case ALLEGRO_EVENT_DISPLAY_RESUME_DRAWING:
                    //FIXME: for android
                    break;

                default:
                    break;
            }
        }

            // if there are events to process, lets suspend drawing for a tick
            al_clear_to_color(al_map_rgb(0,0,0));

            ss.str("");
            ss << "FPS: " << fps;
            str = ss.str();
            al_draw_text(m_font, al_map_rgb(255, 255, 0), 0, 0, ALLEGRO_ALIGN_LEFT, str.c_str());

            m_world->update(static_cast<float>(delta));
            m_world->render();
            //rendering always before this
            al_flip_display();
    }

shutdown:
    shutdown();
}

void Game::shutdown()
{
    al_destroy_display(m_display);
    al_destroy_event_queue(m_events);
    exit(0);
}
