#include "TexturePool.h"
#include <fstream>

TexturePool::TexturePool()
{

}


auto TexturePool::load(const std::string & file) -> const RGBAImage*
{
    auto iter = textures.find(file);
    if(iter == textures.end()){
        std::ifstream in(file);
        if(!in.good()){
            std::cout<<"warring : "<<file<<" did not find in your filesystem."<<std::endl;
        }
        return &textures.emplace(std::piecewise_construct,
                         std::forward_as_tuple(file),
                         std::forward_as_tuple(in)
                    ).first->second;
    }
    return &iter->second;
}
