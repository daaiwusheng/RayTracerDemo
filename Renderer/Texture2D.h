#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include <cstdint>
#include "../RGBAValue.h"
#include "../RGBAImage.h"
#include "Color.h"

class Texture
{
public:
    virtual ~Texture() = default;
    virtual auto sample(const std::pair<float,float> & texCoord) const -> Color = 0;
};



class Texture2D : public Texture
{
public:
    Texture2D() = default;
    Texture2D(const RGBAImage * image);
    auto sample(const std::pair<float,float> & texCoord) const -> Color override;
    auto setImage(const RGBAImage * image) -> void;
    inline auto getImage() const -> const RGBAImage* { return image;}
private:
    //default as RGBA8888
    const RGBAImage * image = nullptr;
};



#endif // TEXTURE2D_H
