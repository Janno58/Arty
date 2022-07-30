#include "TextureCache.h"
#include <stdexcept>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
sf::Texture& TextureCache::GetTexture(const std::string& path) {
    if(textures.contains(path)) {
        return textures[path];
    }

    sf::Texture tex;
    if(!tex.loadFromFile(path)) {
        throw std::runtime_error("Error loading texture!");
    }

    std::cout<< "Loaded: " << path << "\n";

    textures.insert({path, std::move(tex)});
    return textures[path];
}