#ifndef TEXTUREPOOL_H
#define TEXTUREPOOL_H

#include <unordered_map>
#include "../TexturedObject.h"

class TexturePool
{
public:

    static auto get() -> TexturePool & { static TexturePool pool; return pool; }
    auto load(const std::string & file) -> const RGBAImage* ;

private:
    TexturePool();
    std::unordered_map<std::string,RGBAImage> textures;
};

#endif // TEXTUREPOOL_H
