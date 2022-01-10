#pragma once
#include "../TexturedObject.h"
#include "Mesh.h"


namespace ray
{
    class TriangleMesh : public Mesh
    {
    public:
        TriangleMesh(const TexturedObject & texturObject,const std::shared_ptr<Material> & material);
        auto intersects(const std::unique_ptr<Ray> & ray, double minT,double maxT) -> std::shared_ptr<CollisionResult> override;
        auto update(RenderParameters *renderParameters) -> bool override;
     private:
        TexturedObject texturObject;
        auto intersectsWithTriangle(const Cartesian3&,const Cartesian3&,const Cartesian3&, const std::unique_ptr<Ray> & ray, double minT,double maxT,int32_t index) -> std::shared_ptr<CollisionResult>;
    };
}
