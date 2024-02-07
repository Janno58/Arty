#include "TextureCache.h"
#include <stdexcept>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
sf::Texture& TextureCache::GetTexture(const std::string& path) {
    for(auto& pair : cache) {
        if(pair.first == path) {
            return pair.second;
        }
    }

    sf::Texture tex;
    if(!tex.loadFromFile(path)) {
        throw std::runtime_error("Error loading texture!");
    }

    std::cout<< "Loaded: " << path << "\n";

    cache.emplace_back(path, std::move(tex));

    return GetTexture(path);
}