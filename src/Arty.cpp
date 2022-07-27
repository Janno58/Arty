#include "Level.h"
#include "WorldScroll.h"

////////////////////////////////////////////////////////////////////////////////
sf::Vector2f GetMapScroll()
{
    sf::Vector2f vec;

    if( sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::A) ) {
        vec += {-1.F, 0.F};
    }

    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::D) ) {
        vec += {1.F, 0.F};
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
       sf::Keyboard::isKeyPressed(sf::Keyboard::W) ) {
        vec += {0.F, -1.F};
    }

    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::S) ) {
        vec += {0.F, 1.F};
    }

    return vec;
}

////////////////////////////////////////////////////////////////////////////////
constexpr unsigned int DEFAULT_WINDOW_WIDTH = 1024;
constexpr unsigned int DEFAULT_WINDOW_HEIGHT = 768;

////////////////////////////////////////////////////////////////////////////////
int main() {
    const auto winSize = sf::Vector2f(static_cast<float>(DEFAULT_WINDOW_WIDTH),
                                      static_cast<float>(DEFAULT_WINDOW_HEIGHT));
    sf::RenderWindow window(sf::VideoMode(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT), "Arty");

    auto level = Level(2000, 1000);
    auto scroll = WorldScroll(level.GetSize(), winSize);
    window.setView(scroll.GetView());

    sf::Clock frameClock;
    sf::Time frameTimeAccumulator;
    const sf::Time fixedFrameTime = sf::milliseconds(10);

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
        }

        frameTimeAccumulator += frameClock.restart();
        while( frameTimeAccumulator >= fixedFrameTime ) {
            scroll.Scroll(GetMapScroll(), fixedFrameTime);
            frameTimeAccumulator -= fixedFrameTime;
        }

        window.clear(sf::Color::White);
        window.setView(scroll.GetView());
        window.draw(level);
        window.display();
    }

    return 0;
}
