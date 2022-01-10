#include "Mesh.h"
#include "CollisionResult.h"
#include "../MemoryTools.h"

namespace ray
{
    Mesh::Mesh(const std::shared_ptr<Material> & initMaterial)
        :material(initMaterial)
    {

    }

    auto Mesh::assembleCollisionResult(double t,const std::unique_ptr<Ray> & ray,const Cartesian3 & normal) ->  std::shared_ptr<CollisionResult>
    {
        auto ret = std::makeShared<CollisionResult>();
        ret->t = t;
        ret->position = ray->getPosition(t);
        ret->setFaceNormal(ray, normal);
        ret->material = material;
        ret->hitObject = this;
        auto uv = getUV( ret->normal);
        ret->u = uv.first;
        ret->v = uv.second;
        return ret;
    }

    auto Mesh::update(RenderParameters *renderParameters) -> bool
    {
        if(transform.update(renderParameters)){
            box.min = transform.getTransform() * box.min;
            box.max = transform.getTransform() * box.max;
            return true;
        }
        return false;
    }


    auto Mesh::updateTransform(const Matrix4 & parentTransform) -> void
    {
        transform.setParentTransform(parentTransform);
    }

    auto Mesh::getUV(const Cartesian3 & pos) const -> std::pair<float,float>
    {
        return {};
    }

    auto Mesh::getBoundingBox() -> const BoundingBox&
    {
        return box;
    }
}
