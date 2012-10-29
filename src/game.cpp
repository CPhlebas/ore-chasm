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

#include <SFML/Graphics.hpp>

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
    /*
     * TODO:
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

    m_app =sf::Window(sf::VideoMode(SCREEN_W, SCREEN_H), "Buildarrhea"); //sf::Style::Fullscreen

    // cap tickrate to FPS
    timer = al_create_timer(1.0 / FPS);


    image = al_load_bitmap("../textures/stone.png");


    done = false;
    tick();
    shutdown();
}

void Game::tick()
{
    sf::Event Event;
    const sf::Input& Input = m_app.GetInput();

    while (m_app.IsOpened())
    {
        while (m_app.GetEvent(Event))
        {

            bool LeftKeyDown = Input.IsKeyDown(sf::Key::Left);
            bool RightButtonDown = Input.IsMouseButtonDown(sf::Mouse::Right);
            unsigned int MouseX = Input.GetMouseX();
            unsigned int MouseY = Input.GetMouseY();

            // Window closed
            if (Event.Type == sf::Event::Closed || ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))) {
                shutdown();
            }
        }

        // always after rendering!
        m_app.Display();
    }
}

void Game::shutdown()
{
    m_app.Close();
}
