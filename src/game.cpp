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
#include "imagemanager.h"
#include "entity.h"

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Config.hpp>
#include <SFML/OpenGL.hpp>

#include <Box2D/Box2D.h>

#include <GLConsole/GLConsole.h>
//A CVar version of std::vector
#include <CVars/CVarVectorIO.h>
//A CVar version of std::map
#include <CVars/CVarMapIO.h>


Game::Game()
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
    /*
    TODO:
    unsigned int VideoModesCount = sf::VideoMode::GetModesCount();
    for (unsigned int i = 0; i < VideoModesCount; ++i)
    {
        sf::VideoMode Mode = sf::VideoMode::GetMode(i);

        // Mode is a valid video mode
    }
    // Creating a fullscreen window with the best video mode supported
    App.Create(sf::VideoMode::GetMode(0), "SFML Window", sf::Style::Fullscreen);

    sf::VideoMode DesktopMode = sf::VideoMode::GetDesktopMode();
    */

    //TODO: debug only, on by default
    //    App.SetFramerateLimit(60); // Limit to 60 frames per second

    // sf::WindowSettings Settings = App.GetSettings();

    sf::ContextSettings settings;

    settings.depthBits = 24;
    settings.stencilBits = 8;
//    settings.antialiasingLevel = 4;
//    settings.majorVersion = 3;
//    settings.minorVersion = 0;


    m_app = new sf::RenderWindow(sf::VideoMode(SCREEN_W, SCREEN_H), "Buildarrhea", sf::Style::Resize, settings); //sf::Style::Fullscreen
    m_app->setVerticalSyncEnabled(false);
//    m_app->setFramerateLimit(60);
    // totally useless for a game.
    m_app->setKeyRepeatEnabled(false);

    settings = m_app->getSettings();

    std::cout << "depth bits:" << settings.depthBits << std::endl;
    std::cout << "stencil bits:" << settings.stencilBits << std::endl;
    std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;
    std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;

    m_view = new sf::View(sf::FloatRect(0, 0, SCREEN_W, SCREEN_H));
    //TODO    m_view->SetCenter();
    m_app->setView(*m_view);

    m_font = new sf::Font();
    if (!m_font->loadFromFile("../font/DejaVuSerif.ttf"))
    {
        abort_game("unable to load font");
    }

    ImageManager* manager = ImageManager::instance();
    manager->addResourceDir("../textures");

    m_console = new GLConsole(m_app);
    m_console->Init();

    m_world = new World(m_app, m_view);

    tick();
    shutdown();
}

void Game::tick()
{
    sf::Event event;

//    m_player->setPosition(m_view->getCenter());

    sf::Text text;
    text.setFont(*m_font);
    text.setCharacterSize(12.0);
    text.setColor(sf::Color::Yellow);

    std::stringstream ss;
    std::string str;

    sf::Clock clock;

    int fps = 0;
    int minFps = 0;
    int maxFps = 0;

    const int MAX_BENCH = 300;
    int benchTime = MAX_BENCH;


    float elapsedTime = 0;

    while (m_app->isOpen())
    {
        elapsedTime = clock.restart().asSeconds();
        benchTime -= 1;
        fps = int(1.f / elapsedTime);

        // recheck the max, good amount of time passed
        if (benchTime <= 0) {
            m_console->Printf("bench time elapsed");
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
        ss << "Framerate: " << fps << " Min: " << minFps << " Max: " << maxFps << " elapsedTime: " << elapsedTime;
        str = ss.str();
        text.setString(str);

        while (m_app->pollEvent(event))
        {

            // bool LeftKeyDown = Input.isKeyDown(sf::Key::Left);
            // bool RightButtonDown = Input.isMouseButtonDown(sf::Mouse::Right);
            // unsigned int MouseX = Input.getMouseX();
            // unsigned int MouseY = Input.getMouseY();

            m_console->handleEvent(event);
            m_world->handleEvent(event);
            switch (event.type)
            {
                // window closed
            case sf::Event::Closed:
                shutdown();
                break;

                // key pressed
            case sf::Event::KeyPressed:
                if (!m_console->IsOpen()) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        shutdown();
                    }
                } else {
                }

                // traditional tilde used in every game/console, except without shift ;)
                //HACK FIXME: currently bound to F3 because tilde/accent isn't exposed?
                if (event.key.code == sf::Keyboard::F3) {
                    std::cout << "TAB HIT, TOGGLING CONSOLE" << std::endl;
                    m_console->ToggleConsole();
                }

                if (event.key.code == sf::Keyboard::Escape) {
                    shutdown();
                }
                break;

            case sf::Event::KeyReleased:
                if (!m_console->IsOpen()) {

                } else {
                }
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

        m_world->update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        m_app->pushGLStates();
//        m_view->move(500 * xDir * elapsedTime, 500* yDir * elapsedTime);

        m_app->clear(sf::Color(200, 0, 0));

        m_app->setView(m_app->getDefaultView());

        m_app->draw(text);

        m_app->setView(*m_view);
        m_world->render();

        m_app->popGLStates();

        m_app->setView(m_app->getDefaultView());
        m_console->RenderConsole();
        m_app->setView(*m_view);

        // always after rendering!
        m_app->display();

    }
}

void Game::shutdown()
{
    m_app->close();
}
