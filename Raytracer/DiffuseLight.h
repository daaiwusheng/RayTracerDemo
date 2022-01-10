#pragma once

#include "Material.h"

namespace ray
{
    class DiffuseLight : public Material
    {
    public:
        DiffuseLight(const std::shared_ptr<Texture> & texture);
        auto scatter(const std::unique_ptr<Ray> & rayIn, const std::shared_ptr<CollisionResult> & result) const -> std::unique_ptr<ScatterResult> override;
        auto emitted(const std::unique_ptr<Ray> & rayIn, const std::shared_ptr<CollisionResult> & result,const std::pair<float,float> & texCoord) -> Color override;
             virtual auto getType() -> MaterialType override { return MaterialType::DiffuseLight;}
    private:
        std::shared_ptr<Texture>  texture;
    };
}

