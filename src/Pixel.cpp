#include <Pixel.h>
#include <cassert>

////////////////////////////////////////////////////////////////////////////////
Pixel::Pixel(int xPos, int yPos, Color col) : x(xPos), y(yPos), color(col){
    static_assert(sizeof(Color) == 4);
}

////////////////////////////////////////////////////////////////////////////////
std::vector<Pixel> CreateCircle(int centerX, int centerY, int radius) {
    std::vector<Pixel> pixels;

    for( int yPos = -radius; yPos <= radius; yPos++) {
        for( int xPos = -radius; xPos <= radius; xPos++) {
            if( xPos * xPos + yPos * yPos <= radius * radius) {
                pixels.emplace_back(centerX + xPos, centerY + yPos, Color(0,0,0,0));
            }
        }
    }

    return pixels;
}

////////////////////////////////////////////////////////////////////////////////
bool Overlaps(Vec2f pos1, Vec2u size1, const std::vector<std::uint8_t>& pixels1,
              Vec2f pos2, Vec2u size2, const std::vector<std::uint8_t>& pixels2) {
    const int pos1x = static_cast<int>(pos1.x);
    const int pos1y = static_cast<int>(pos1.y);
    const int pos2x = static_cast<int>(pos2.x);
    const int pos2y = static_cast<int>(pos2.y);

    const auto xMin = std::max(pos1x, pos2x);
    const auto yMin = std::max(pos1y, pos2y);
    const auto xMax = std::min(pos1x + static_cast<int>(size1.x), pos2x + static_cast<int>(size2.x));
    const auto yMax = std::min(pos1y + static_cast<int>(size1.y), pos2y + static_cast<int>(size2.y));

    if (xMin < xMax && yMin < yMax) {
        const unsigned long width = xMax - xMin;
        const auto height = yMax - yMin;

        for (auto y_index = 0; y_index < height; y_index++) {
            const auto pix1_index = ((xMin - pos1x) + (yMin - pos1y + y_index) * size1.x) * 4;
            const auto pix2_index = ((xMin - pos2x) + (yMin - pos2y + y_index) * size2.x) * 4;

            assert(pix1_index < pixels1.size());
            assert(pix2_index < pixels2.size());

            for (auto byte_index = 0UL; byte_index < width * 4; byte_index++) {
                if (pixels1[pix1_index + byte_index] > 0 && pixels2[pix2_index + byte_index] > 0) {
                    return true;
                }
            }
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool Overlaps(Vec2f pos1, Vec2f pos2, Vec2u size2, const std::vector<std::uint8_t>& pixels2) {
    if (pos1.x < pos2.x || pos1.x > pos2.x + static_cast<float>(size2.x) ||
        pos1.y < pos2.y || pos1.y > pos2.y + static_cast<float>(size2.y)) {
        return false;
    }

    const auto x_pos = static_cast<size_t>(pos1.x - pos2.x);
    const auto y_pos = static_cast<size_t>(pos1.y - pos2.y);

    auto body2Index = (x_pos + y_pos * size2.x) * 4;
    assert(body2Index < pixels2.size());

    for(auto offset = 0UL; offset < 4; offset++) {
        if(pixels2[body2Index + offset] > 0) {
            return true;
        }
    }

    return false;
}