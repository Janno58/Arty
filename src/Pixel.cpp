#include <Pixel.h>

////////////////////////////////////////////////////////////////////////////////
Pixel::Pixel(int xPos, int yPos, sf::Color col) : x(xPos), y(yPos), color(col){
}

////////////////////////////////////////////////////////////////////////////////
std::vector<Pixel> CreateCircle(int centerX, int centerY, int radius) {
    std::vector<Pixel> pixels;

    for( int yPos = -radius; yPos <= radius; yPos++) {
        for( int xPos = -radius; xPos <= radius; xPos++) {
            if( xPos * xPos + yPos * yPos <= radius * radius) {
                pixels.emplace_back(centerX + xPos, centerY + yPos, sf::Color(0,0,0,0));
            }
        }
    }

    return pixels;
}
