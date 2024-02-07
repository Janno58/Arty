#include "Game.h"
#include <stack>
#include <memory>

////////////////////////////////////////////////////////////////////////////////
constexpr unsigned int DEFAULT_WINDOW_WIDTH = 1024U;
constexpr unsigned int DEFAULT_WINDOW_HEIGHT = 768U;
constexpr          int DEFAULT_ANTI_ALIASING = 8;

////////////////////////////////////////////////////////////////////////////////
int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = DEFAULT_ANTI_ALIASING;

    sf::RenderWindow window(sf::VideoMode(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT), "Arty", sf::Style::Default, settings);

    TextureCache texCache;
    Projectile::SetTexCache(&texCache);

    std::stack<std::unique_ptr<GameState>> states;
    states.push(std::make_unique<MainMenu>(window, texCache, states));

    while(!states.empty()) {
        states.top()->ExecuteFrame();

        if(states.top()->HasStopped()) {
            states.pop();
        }
    }

    return 0;
}
