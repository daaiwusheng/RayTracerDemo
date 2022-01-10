#include "Texture2D.h"
#include "MathUtils.h"
#include <cmath>


Texture2D::Texture2D(const RGBAImage * initImage)
    :image(initImage)
{

}

auto Texture2D::sample(const std::pair<float,float> & texCoord) const -> Color
{
    if(image == nullptr){
        return {};
    }

    if(image->width * image->height == 0){
       return {};
    }
//in here,
//we can use bilinear interpolation.
    int32_t u = std::min(static_cast<long>(texCoord.first * (image->width - 1)),image->width- 1);
    int32_t v = std::min(static_cast<long>((texCoord.second) * (image->height - 1)),image->height- 1);
    auto c = (*image)[v][u];
    return {c.red,c.green,c.blue,c.alpha};
}

auto Texture2D::setImage(const RGBAImage * image) -> void
{
    this->image = image;
}
