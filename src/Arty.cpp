#include "Config.h"
#include "Physics.h"
#include "Input.h"
#include "Level.h"
#include "Unit.h"
#include "WorldScroll.h"
#include "Projectile.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
constexpr unsigned int DEFAULT_WINDOW_WIDTH = 1024U;
constexpr unsigned int DEFAULT_WINDOW_HEIGHT = 768U;

////////////////////////////////////////////////////////////////////////////////
int main() {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT), "Arty", sf::Style::Default, settings);

    std::cout<< "Anti-aliasing: " << window.getSettings().antialiasingLevel << "\n";

    TextureCache texCache;
    Projectile::SetTexCache(&texCache);

    auto level = Level(4000, 1000);
    auto scroll = WorldScroll(level.GetSize(), {DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT});
    window.setView(scroll.GetView());

    sf::Clock frameClock;
    sf::Time frameTimeAccumulator;
    const sf::Time fixedFrameTime = sf::milliseconds(10);

    auto unit = Unit();

    std::vector<Projectile> shells;

    sf::Event event{};
    bool running{true};

    while(running) {
        while(window.pollEvent(event)) {
            if( event.type == sf::Event::Closed ) {
                running = false;
            }

            else if( event.type == sf::Event::Resized ) {
                scroll.SetScreenSize(event.size.width, event.size.height);
            }

            else if( event.type == sf::Event::MouseMoved ) {
                auto gMousePos = sf::Mouse::getPosition(window);
                auto mousePos  = window.mapPixelToCoords(gMousePos);
                unit.MouseMove(mousePos);
            }

            else if( event.type == sf::Event::MouseButtonReleased &&
                    event.mouseButton.button == sf::Mouse::Left) {

                const auto posBase= unit.GetRotationPoint();
                const auto posTip = unit.GetMuzzlePos();
                const auto vel = Normalize(posTip - posBase) * 400.F;

                shells.emplace_back(posTip, vel);
            }
        }

        frameTimeAccumulator += frameClock.restart();
        while( frameTimeAccumulator >= fixedFrameTime ) {
            scroll.Scroll(Input::GetMapScroll(), fixedFrameTime);
            unit.StepPhysics(fixedFrameTime.asSeconds());

            if(Physics::Collides(level, unit)) {
                unit.SetVelocity(-GRAVITY);
            }

            for(auto& shell : shells) {
                shell.StepPhysics(fixedFrameTime.asSeconds());

                if(Physics::Collides(level, shell)) {
                    auto pixels = shell.Explode();
                    level.SetPixels(pixels);
                }
            }

            frameTimeAccumulator -= fixedFrameTime;
        }

        shells.erase( std::remove_if(shells.begin(), shells.end(), [](const auto& shell){ return shell.HasExploded(); }), shells.end() );

        window.clear(sf::Color::White);
        window.setView(scroll.GetView());
        window.draw(level);
        window.draw(unit);

        for(auto& shell : shells) {
            window.draw(shell);
        }

        window.display();
    }

    return 0;
}
