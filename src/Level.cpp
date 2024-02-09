#include "Level.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <cstring>

////////////////////////////////////////////////////////////////////////////////
namespace fs = std::filesystem;

////////////////////////////////////////////////////////////////////////////////
std::vector<sf::Vector2f> LoadSpawnPoints(const fs::path& path);

////////////////////////////////////////////////////////////////////////////////
Level::Level(const std::string& levelName) : vertices(sf::Quads, 4) {
    fs::path level{levelName};
    fs::path png = fs::path("levels") / level / fs::path("level.png");
    fs::path spw = fs::path("levels") / level / fs::path("spawns");

    spawns = LoadSpawnPoints(spw);

    sf::Image img;
    if(!img.loadFromFile(png.relative_path())) {
        throw std::runtime_error("Error loading level image!");
    }

    width = static_cast<float>(img.getSize().x);
    height = static_cast<float>(img.getSize().y);

    vertices[0] = sf::Vector2f(0.F, 0.F);
    vertices[1] = sf::Vector2f(width, 0.F);
    vertices[2] = sf::Vector2f(width, height);
    vertices[3] = sf::Vector2f(0.F, height);

    vertices[0].texCoords = sf::Vector2f(0.F, 0.F);
    vertices[1].texCoords = sf::Vector2f(width, 0.F);
    vertices[2].texCoords = sf::Vector2f(width, height);
    vertices[3].texCoords = sf::Vector2f(0.F, height);

    texture.create(img.getSize().x, img.getSize().y);
    texture.update(img.getPixelsPtr());

    pixels.reserve(static_cast<unsigned int>(img.getSize().x * img.getSize().y *4));
    std::memcpy(pixels.data(), img.getPixelsPtr(), static_cast<unsigned int>(img.getSize().x * img.getSize().y *4));

    background.Create(width, height);
}

////////////////////////////////////////////////////////////////////////////////
sf::Color Level::GetBackgroundColor() const {
    return background.Color;
}

////////////////////////////////////////////////////////////////////////////////
sf::Vector2f Level::GetSize() const {
    return {width, height};
}

////////////////////////////////////////////////////////////////////////////////
sf::Color Level::GetPixel(sf::Vector2f pos) const {
    auto xPos = static_cast<unsigned long>(pos.x);
    auto yPos = static_cast<unsigned long>(pos.y);
    auto index = (xPos + yPos * static_cast<unsigned long>(width)) * 4;

    if(index > static_cast<unsigned long>(width * height) * 4) {
        return {0,0,0,0};
    }

    sf::Color color;
    color.r = pixels[index];
    color.g = pixels[index+1];
    color.b = pixels[index+2];
    color.a = pixels[index+3];

    return color;
}

////////////////////////////////////////////////////////////////////////////////
sf::Vector2f Level::GetSpawn(std::vector<sf::Vector2f>::size_type index) const {
    return spawns.at(index);
}

////////////////////////////////////////////////////////////////////////////////
void Level::SetPixels(const std::vector<Pixel>& newPixels)
{
    for(const auto& pixel : newPixels) {
        if(pixel.x < 0 || pixel.y < 0 || pixel.x >= static_cast<int>(width) || pixel.y >= static_cast<int>(height)) { continue; }

        auto xPos = pixel.x;
        auto yPos = pixel.y;
        size_t index = (xPos + yPos * static_cast<size_t>(width)) * 4;

        pixels[index++] = pixel.color.r;
        pixels[index++] = pixel.color.g;
        pixels[index++] = pixel.color.b;
        pixels[index++] = pixel.color.a;
    }

    texture.update(pixels.data());
}

////////////////////////////////////////////////////////////////////////////////
void Level::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(background, states);
    states.texture = &texture;
    target.draw(vertices, states);
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> EnumerateLevels() {
    std::vector<std::string> levels;

    fs::path levelsDir("levels/");

    if(!fs::exists(levelsDir) && !fs::is_directory(levelsDir)) {
        std::cerr<< "No levels directory! \n";
    }

    for(auto it = fs::directory_iterator(levelsDir); it != fs::directory_iterator(); ++it) {
        if(it->is_directory()) {
            bool levelPNG = false;
            bool spawns = false;

            for(auto subIter = fs::directory_iterator(it->path()); subIter != fs::directory_iterator(); ++subIter) {
                if(subIter->is_regular_file() && subIter->path().filename() == "level.png") {
                    levelPNG = true;
                }

                if(subIter->is_regular_file() && subIter->path().filename() == "spawns") {
                    spawns = true;
                }
            }

            if(levelPNG && spawns) {
                levels.push_back(it->path().filename());
            }
        }
    }

    return levels;
}

////////////////////////////////////////////////////////////////////////////////
int GetMaxPlayers(const std::string& levelName) {
    fs::path levelsDir("levels/");
    fs::path spawnsFile = levelsDir / levelName / "spawns";

    if(!fs::exists(spawnsFile) && !fs::is_regular_file(spawnsFile)) {
        throw std::runtime_error("spawns file does not exist!");
    }

    auto vec = LoadSpawnPoints(spawnsFile);
    return static_cast<int>(vec.size());
}

////////////////////////////////////////////////////////////////////////////////
std::vector<sf::Vector2f> LoadSpawnPoints(const fs::path& path) {
    if(!fs::exists(path) && !fs::is_regular_file(path)) {
        throw std::runtime_error("spawns file does not exist!");
    }

    std::vector<sf::Vector2f> spawns;

    std::ifstream ifs(path);
    if(ifs.is_open() && ifs.good()) {
        std::string line;
        float one = 0.F;
        float two = 0.F;

        while(std::getline(ifs, line)) {
            std::stringstream sstream(line);
            sstream >> one >> two;
            spawns.emplace_back(one, two);
        }
    } else {
        throw std::runtime_error("Error opening spawns file!");
    }

    ifs.close();
    return spawns;
}