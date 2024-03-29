#pragma once
////////////////////////////////////////////////////////////////////////////////
#include <SFML/Graphics/Texture.hpp>
#include <list>

////////////////////////////////////////////////////////////////////////////////
class TextureCache {
public:

    ////////////////////////////////////////////////////////////////////////////
    sf::Texture& GetTexture(const std::string& path);

private:

    ////////////////////////////////////////////////////////////////////////////
    std::list<std::pair<std::string, sf::Texture>> cache;

};
