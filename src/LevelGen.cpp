#include "LevelGen.h"
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
std::vector<int> GenerateWave(int length) {
    std::vector<int> result;

    for(int i = 0; i < length; i++) {
        result.emplace_back( 100 * std::sin(i*0.01) + 500) ;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<sf::Uint8> PixelsFromHeights(const std::vector<int>& heights, int levelHeight) {
    std::vector<sf::Uint8> pixels;

    for(int y = 0; y < levelHeight; y++) {
        for(int height : heights) {
            if(y > height) {
                pixels.push_back(150);
                pixels.push_back(75);
                pixels.push_back(0);
                pixels.push_back(255);
            } else {
                pixels.push_back(0);
                pixels.push_back(0);
                pixels.push_back(0);
                pixels.push_back(0);
            }
        }
    }

    return pixels;
}