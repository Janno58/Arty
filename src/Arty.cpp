#include <SFML/Graphics.hpp>

////////////////////////////////////////////////////////////////////////////////
constexpr unsigned int DEFAULT_WINDOW_WIDTH = 1024;
constexpr unsigned int DEFAULT_WINDOW_HEIGHT = 768;

int main() {

    sf::RenderWindow window(sf::VideoMode(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT), "Arty");
    bool running{true};
    sf::Event event{};

    while(running) {
        while(window.pollEvent(event)) {
            if( event.type == sf::Event::Closed ) {
                running = false;
            }
        }

        window.clear(sf::Color::White);
        window.display();
    }

    return 0;
}