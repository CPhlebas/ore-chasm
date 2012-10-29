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
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Config.hpp>

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
    //    m_.UseVerticalSync(false);
    //    App.SetFramerateLimit(60); // Limit to 60 frames per second

    // sf::WindowSettings Settings = App.GetSettings();

    sf::WindowSettings settings;
    settings.DepthBits = 24; // Request a 24 bits depth buffer
    settings.StencilBits = 8;  // Request a 8 bits stencil buffer
    settings.AntialiasingLevel = 2;  // Request 2 levels of antialiasing

    m_app = new sf::RenderWindow(sf::VideoMode(SCREEN_W, SCREEN_H), "Buildarrhea", sf::Style::Resize, settings); //sf::Style::Fullscreen

    done = false;
    tick();
    shutdown();
}

void Game::tick()
{
    sf::Event Event;
    const sf::Input& Input = m_app->GetInput();

    float Left = 0.f;
    float Top  = 0.f;

    sf::Image image;
    if (!image.LoadFromFile("../textures/stone.png")) {
        abort_game("unable to load texture");
    }

    sf::Sprite sprite;
    sprite.SetColor(sf::Color(0, 255, 255, 128));
    sprite.SetX(200.f);
    sprite.SetY(100.f);
    sprite.SetPosition(200.f, 100.f);
    sprite.SetRotation(30.f);
    sprite.SetCenter(0, 0);
    sprite.SetScaleX(2.f);
    sprite.SetScaleY(0.5f);
    sprite.SetScale(2.f, 0.5f);
    sprite.SetBlendMode(sf::Blend::Multiply);
    sprite.SetImage(image);

    sprite.Move(10, 5);
    sprite.Rotate(90);
    sprite.Scale(1.5f, 1.5f);

    //  "../textures/stone.png"
    while (m_app->IsOpened())
    {
        float elapsedTime = m_app->GetFrameTime();
        float fps = 1.f / elapsedTime;
        printf("Framerate: %f \n", fps);

        while (m_app->GetEvent(Event))
        {

            bool LeftKeyDown = Input.IsKeyDown(sf::Key::Left);
            bool RightButtonDown = Input.IsMouseButtonDown(sf::Mouse::Right);
            unsigned int MouseX = Input.GetMouseX();
            unsigned int MouseY = Input.GetMouseY();

            // Move the sprite
            if (m_app->GetInput().IsKeyDown(sf::Key::Left))  sprite.Move(-100 * elapsedTime, 0);
            if (m_app->GetInput().IsKeyDown(sf::Key::Right)) sprite.Move( 100 * elapsedTime, 0);
            if (m_app->GetInput().IsKeyDown(sf::Key::Up))    sprite.Move(0, -100 * elapsedTime);
            if (m_app->GetInput().IsKeyDown(sf::Key::Down))  sprite.Move(0,  100 * elapsedTime);

            // Window closed
            if (Event.Type == sf::Event::Closed || ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))) {
                shutdown();
            }
        }

        m_app->Clear(sf::Color(200, 0, 0));
        m_app->Draw(sprite);

        // always after rendering!
        m_app->Display();
    }
}

void Game::shutdown()
{
    m_app->Close();
}
