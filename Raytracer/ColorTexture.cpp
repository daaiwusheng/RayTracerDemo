#include "ColorTexture.h"


namespace ray
{
    auto ColorTexture::sample(const std::pair<float,float> & texCoord) const -> Color
    {
        return color;
    }
}
