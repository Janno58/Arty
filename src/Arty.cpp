#include "Physics.h"
#include "Input.h"
#include "Level.h"
#include "Unit.h"
#include "WorldScroll.h"
#include <SFML/Graphics.hpp>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
constexpr unsigned int DEFAULT_WINDOW_WIDTH = 1024;
constexpr unsigned int DEFAULT_WINDOW_HEIGHT = 768;

////////////////////////////////////////////////////////////////////////////////
int main() {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    const auto winSize = sf::Vector2f(static_cast<float>(DEFAULT_WINDOW_WIDTH),
                                      static_cast<float>(DEFAULT_WINDOW_HEIGHT));
    sf::RenderWindow window(sf::VideoMode(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT), "Arty", sf::Style::Default, settings);

    std::cout<< "Anti-aliasing: " << window.getSettings().antialiasingLevel << "\n";

    auto level = Level(2000, 1000);
    auto scroll = WorldScroll(level.GetSize(), winSize);
    window.setView(scroll.GetView());

    sf::Clock frameClock;
    sf::Time frameTimeAccumulator;
    const sf::Time fixedFrameTime = sf::milliseconds(10);

    auto unit = Unit();

    sf::Event event{};
    bool running{true};

    while(running) {
        while(window.pollEvent(event)) {
            if( event.type == sf::Event::Closed ) {
                running = false;
            }

            else if( event.type == sf::Event::Resized ) {
                const auto width = static_cast<float>(event.size.width);
                const auto height = static_cast<float>(event.size.height);

                scroll.SetScreenSize(width, height);
            }

            else if( event.type == sf::Event::MouseMoved ) {
                auto gMousePos = sf::Mouse::getPosition(window);
                auto mousePos  = window.mapPixelToCoords(gMousePos);
                unit.MouseMove(mousePos);
            }
        }

        frameTimeAccumulator += frameClock.restart();
        while( frameTimeAccumulator >= fixedFrameTime ) {
            scroll.Scroll(Input::GetMapScroll(), fixedFrameTime);
            unit.StepPhysics(fixedFrameTime.asSeconds());

            if(Physics::Collides(level, unit)) {
                unit.SetVelocity(sf::Vector2f(0.F, -9.8F));
            }

            frameTimeAccumulator -= fixedFrameTime;
        }

        window.clear(sf::Color::White);
        window.setView(scroll.GetView());
        window.draw(level);
        window.draw(unit);
        window.display();
    }

    return 0;
}
