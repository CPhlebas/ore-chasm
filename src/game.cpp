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
    al_init_ttf_addon();
    al_init_image_addon();

    m_display = al_create_display(SCREEN_W, SCREEN_H);
    Debug::fatal(m_display, Debug::Area::Graphics, "display creation failed");

    al_set_app_name("ore-chasm");
    al_set_window_title(m_display, "Ore Chasm");

    Debug::fatal(al_create_event_queue(), Debug::Area::System, "event queue init");

    al_register_event_source(m_events, al_get_display_event_source(m_display));

    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_flip_display();

    std::cout << "\n\n\n\n";
    std::cout << "=======================================================================" << "\n";
    std::cout << "Hardware support information" << "\n";
    std::cout << "OpenGL Version: " << settings.majorVersion << "." << settings.minorVersion << "\n";
    std::cout << "Depth bits: " << settings.depthBits << "\n";
    std::cout << "Stencil bits:" << settings.stencilBits << "\n";
    std::cout << "Antialiasing Level: " << settings.antialiasingLevel << "\n";
    std::cout << "Maximum Texture Size: " << sf::Texture::getMaximumSize() << " pixels \n";
    std::cout << "=======================================================================" << "\n";
    std::cout << "\n\n\n\n";

//    ImageManager* manager = ImageManager::instance();
//    manager->addResourceDir("../textures/");

    // World takes ownership of m_view
 //   World::createInstance(m_window, m_view);
//    m_world = World::instance();

    tick();
    shutdown();
}

void Game::tick()
{
    std::stringstream ss;
    std::string str;

    sf::Clock clock;

    int fps = 0;
    int minFps = 0;
    int maxFps = 0;

    const int MAX_BENCH = 300;
    int benchTime = MAX_BENCH;

    float elapsedTime = 0;

    while (m_window->isOpen()) {
        elapsedTime = clock.restart().asSeconds();
        benchTime -= 1;
        fps = int(1.f / elapsedTime);

        // recheck the max, good amount of time passed
        if (benchTime <= 0) {
            maxFps = fps;
            minFps = maxFps;
            benchTime = MAX_BENCH;
        }

        if (fps < minFps) {
            minFps = fps;
        }

        if (fps > maxFps) {
            maxFps = fps;
        }

        ss.str("");
        ss << "Framerate: " << fps << " Min: " << minFps << " Max: " << maxFps << " elapsedTime: " << elapsedTime << "";
        str = ss.str();
        text.setString(str);

        while (m_window->pollEvent(event)) {

            // bool LeftKeyDown = Input.isKeyDown(sf::Key::Left);
            // bool RightButtonDown = Input.isMouseButtonDown(sf::Mouse::Right);
            // unsigned int MouseX = Input.getMouseX();
            // unsigned int MouseY = Input.getMouseY();

            m_world->handleEvent(event);
            switch (event.type) {
                // window closed
            case sf::Event::Closed:
                goto shutdown;
                break;

                // key pressed
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    goto shutdown;
                }
                break;

            case sf::Event::KeyReleased:
                break;

            case sf::Event::MouseMoved:
                // std::cout << "new mouse x: " << event.mouseMove.x << std::endl;
                // std::cout << "new mouse y: " << event.mouseMove.y << std::endl;
                break;

            case sf::Event::GainedFocus:
                break;

            case sf::Event::LostFocus:
                break;

            case sf::Event::MouseButtonPressed:
                break;

            case sf::Event::MouseButtonReleased:
                break;

            default:
                break;
            }

            //sf::event::LostFocus
            //sf::event::GainedFocus

            // Window closed
//            if (event.type == sf::Event::Closed || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Key::Escape))) {
//               shutdown();
            //          }
        }


 //       m_world->update(elapsedTime);
        // render methods *must* exit by setting back to the default view
        // (if they set it to a different view at the beginning of the call)
//        m_world->render();

    }

shutdown:
    shutdown();
}

void Game::shutdown()
{
    exit(0);
}
