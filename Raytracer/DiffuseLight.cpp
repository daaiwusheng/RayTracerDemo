#include "DiffuseLight.h"
#include "Ray.h"
#include "CollisionResult.h"

namespace ray
{
    DiffuseLight::DiffuseLight(const std::shared_ptr<Texture> & initTexture)
        :texture(initTexture)
    {

    }

    auto DiffuseLight::scatter(const std::unique_ptr<Ray> & rayIn, const std::shared_ptr<CollisionResult> & result) const -> std::unique_ptr<ScatterResult>
    {
        return nullptr;
    }

    auto DiffuseLight::emitted(const std::unique_ptr<Ray> & rayIn, const std::shared_ptr<CollisionResult> & result,const std::pair<float,float> & texCoord) -> Color
    { 
       return  texture->sample(texCoord);;
    }
}
