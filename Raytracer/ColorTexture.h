#ifndef COLORTEXTURE_H
#define COLORTEXTURE_H

#include "../Renderer/Texture2D.h"

namespace ray
{
    class ColorTexture : public Texture
    {
    public:
        ColorTexture(const Color & color) : color(color) {}
        virtual auto sample(const std::pair<float,float> & texCoord) const -> Color;
    private:
        const Color color;
    };
}


#endif // COLORTEXTURE_H
