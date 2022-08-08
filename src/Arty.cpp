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
constexpr unsigned int LEVEL_WIDTH = 4000;
constexpr unsigned int LEVEL_HEIGHT = 1000;

////////////////////////////////////////////////////////////////////////////////
int main() {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT), "Arty", sf::Style::Default, settings);

    std::cout<< "Anti-aliasing: " << window.getSettings().antialiasingLevel << "\n";

    TextureCache texCache;
    Projectile::SetTexCache(&texCache);

    auto level = Level(LEVEL_WIDTH, LEVEL_HEIGHT);
    auto scroll = WorldScroll(level.GetSize(), {DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT});
    window.setView(scroll.GetView());

    sf::Clock frameClock;
    sf::Time frameTimeAccumulator;
    const sf::Time fixedFrameTime = sf::milliseconds(10);

    //
    // Player tanks
    //
    std::vector<Unit> players;
    players.emplace_back(texCache.GetTexture("tanks_tankGreen_body3.png"),
                         texCache.GetTexture("tanks_turret1.png"),
                         level.GetSpawn(0U));

    players.emplace_back(texCache.GetTexture("tanks_tankNavy_body3.png"),
                         texCache.GetTexture("tanks_turret1.png"),
                         level.GetSpawn(1U));

    std::vector<Unit>::size_type activePlayer = 0;

    std::vector<Projectile> shells;

    sf::Event event{};
    bool running{true};

    while(running) {
        //
        // Events
        //
        while(window.pollEvent(event)) {
            if( event.type == sf::Event::Closed ) {
                running = false;
            }

            else if( event.type == sf::Event::Resized ) {
                scroll.SetScreenSize(event.size.width, event.size.height);
            }

            else if( event.type == sf::Event::MouseButtonReleased &&
                    event.mouseButton.button == sf::Mouse::Left) {
                const auto posBase= players[activePlayer].GetRotationPoint();
                const auto posTip = players[activePlayer].GetMuzzlePos();
                const auto vel = Normalize(posTip - posBase) * 700.F;

                shells.emplace_back(posTip, vel);
                activePlayer++;

                if(activePlayer >= players.size()) {
                    activePlayer = 0U;
                }

            }
        }

        const auto gMousePos = sf::Mouse::getPosition(window);
        const auto mousePos  = window.mapPixelToCoords(gMousePos);

        players[activePlayer].MouseMove(mousePos);

        //
        // Physics
        //
        frameTimeAccumulator += frameClock.restart();
        while( frameTimeAccumulator >= fixedFrameTime ) {
            scroll.Scroll(Input::GetMapScroll(), fixedFrameTime);

            for(auto& tank : players) {
                tank.StepPhysics(fixedFrameTime.asSeconds());

                if(Physics::Collides(level, tank)) {
                    tank.SetVelocity(-GRAVITY);
                }
            }

            for(auto& shell : shells) {
                shell.StepPhysics(fixedFrameTime.asSeconds());

                if(Physics::Collides(level, shell)) {
                    auto pixels = shell.Explode();
                    level.SetPixels(pixels);
                }

                for(auto& tank : players) {
                    if(Physics::Collides(tank, shell)) {
                        auto pixels = shell.Explode();
                        level.SetPixels(pixels);
                    }
                }
            }

            frameTimeAccumulator -= fixedFrameTime;
        }

        //
        // Misc housekeeping
        //
        std::erase_if(shells, [](const auto& shell){ return shell.HasExploded(); });
        std::erase_if(shells, ShellOutsideLevel(LEVEL_WIDTH, LEVEL_HEIGHT));

        //
        // Draw
        //
        window.clear(level.GetBackgroundColor());
        window.setView(scroll.GetView());
        window.draw(level);

        for(auto& tank : players) {
            window.draw(tank);
        }

        for(auto& shell : shells) {
            window.draw(shell);
        }

        window.display();
    }

    return 0;
}
