#include "GuiFunctions.h"

////////////////////////////////////////////////////////////////////////////////
namespace GUI {

////////////////////////////////////////////////////////////////////////////////
std::vector<sf::Vertex> CreateQuad(sf::Vector2f VertStart, sf::Vector2f VertEnd,
                                   sf::Vector2f TexStart, sf::Vector2f TexEnd);

////////////////////////////////////////////////////////////////////////////////
std::vector<sf::Vertex> CreateWindowVertices(sf::Vector2f WindowSize,
                                             sf::Vector2u TextureSize)
{
    const auto TexX = static_cast<float>(TextureSize.x);
    const auto TexY = static_cast<float>(TextureSize.y);

    const auto OneThirdX = TexX / 3.F;
    const auto OneThirdY = TexY / 3.F;

    // Top left
    const auto topLeft = CreateQuad( sf::Vector2f(0.F, 0.F),
                                     sf::Vector2f(OneThirdX, OneThirdY),
                                     sf::Vector2f(0.F, 0.F),
                                     sf::Vector2f(OneThirdX, OneThirdY));

    // Top center
    const auto topCenter = CreateQuad(sf::Vector2f(OneThirdX, 0.F),
                                      sf::Vector2f(WindowSize.x - OneThirdX,OneThirdY),
                                      sf::Vector2f(OneThirdX, 0.F),
                                      sf::Vector2f(OneThirdX * 2, OneThirdY));

    // Top right
    const auto topRight = CreateQuad(sf::Vector2f(WindowSize.x - OneThirdX, 0.F),
                                     sf::Vector2f(WindowSize.x, OneThirdY),
                                     sf::Vector2f(OneThirdX * 2, 0.F),
                                     sf::Vector2f(OneThirdX * 3, OneThirdY));

    // Middle left
    const auto midLeft = CreateQuad( sf::Vector2f(0.F, OneThirdY),
                                     sf::Vector2f(OneThirdX, WindowSize.y - OneThirdY),
                                     sf::Vector2f(0.F, OneThirdY),
                                     sf::Vector2f(OneThirdX, OneThirdY*2));

    // Middle center
    const auto midCenter = CreateQuad(sf::Vector2f(OneThirdX, OneThirdY),
                                      sf::Vector2f(WindowSize.x - OneThirdX,WindowSize.y - OneThirdY),
                                      sf::Vector2f(OneThirdX, OneThirdY),
                                      sf::Vector2f(OneThirdX * 2, OneThirdY * 2));

    // Middle right
    const auto midRight = CreateQuad(sf::Vector2f(WindowSize.x - OneThirdX, OneThirdY),
                                     sf::Vector2f(WindowSize.x, WindowSize.y - OneThirdY),
                                     sf::Vector2f(OneThirdX * 2, OneThirdY),
                                     sf::Vector2f(TexX, OneThirdY * 2));

    // Bottom left
    const auto botLeft = CreateQuad( sf::Vector2f(0.F, WindowSize.y - OneThirdY),
                                     sf::Vector2f(OneThirdX, WindowSize.y),
                                     sf::Vector2f(0.F, OneThirdY * 2),
                                     sf::Vector2f(OneThirdX, TexY));

    // Bottom center
    const auto botCenter = CreateQuad(sf::Vector2f(OneThirdX, WindowSize.y - OneThirdY),
                                      sf::Vector2f(WindowSize.x - OneThirdX, WindowSize.y),
                                      sf::Vector2f(OneThirdX, OneThirdY * 2),
                                      sf::Vector2f(OneThirdX * 2, TexY));

    // Bottom right
    const auto botRight = CreateQuad( sf::Vector2f(WindowSize.x - OneThirdX, WindowSize.y - OneThirdY),
                                      WindowSize,
                                      sf::Vector2f(OneThirdX*2, OneThirdY*2),
                                      sf::Vector2f(TexX, TexY));


    std::vector<sf::Vertex> ret;

    ret.insert(ret.end(), topLeft.begin(), topLeft.end());
    ret.insert(ret.end(), topCenter.begin(), topCenter.end());
    ret.insert(ret.end(), topRight.begin(), topRight.end());

    ret.insert(ret.end(), midLeft.begin(), midLeft.end());
    ret.insert(ret.end(), midCenter.begin(), midCenter.end());
    ret.insert(ret.end(), midRight.begin(), midRight.end());

    ret.insert(ret.end(), botLeft.begin(), botLeft.end());
    ret.insert(ret.end(), botCenter.begin(), botCenter.end());
    ret.insert(ret.end(), botRight.begin(), botRight.end());

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
std::vector<sf::Vertex> CreateQuad(sf::Vector2f VertStart, sf::Vector2f VertEnd,
                                   sf::Vector2f TexStart, sf::Vector2f TexEnd) {
    const auto Vertex1 = sf::Vertex(VertStart, TexStart);
    const auto Vertex2 = sf::Vertex(sf::Vector2f( VertEnd.x, VertStart.y ),
                                    sf::Vector2f(TexEnd.x, TexStart.y));
    const auto Vertex3 = sf::Vertex(VertEnd,TexEnd);
    const auto Vertex4 = sf::Vertex(sf::Vector2f(VertStart.x, VertEnd.y),
                                    sf::Vector2f(TexStart.x, TexEnd.y));

    return {Vertex1, Vertex2, Vertex3, Vertex3, Vertex4, Vertex1};
}

} /* ns GUI */