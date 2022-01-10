#pragma once

#include "Ray.h"
#include "../BoundingBox.h"
#include <memory>
#include "../Transform.h"
#include "../RenderParameters.h"

namespace ray
{
    class CollisionResult;
    class Material;

    enum class ShadingType{
       BlinnPhong,
       Diffuse,
    };
    class Mesh
    {
    public:
        Mesh() = default;
        Mesh(const std::shared_ptr<Material> & material);
        virtual auto intersects(const std::unique_ptr<Ray> & ray, double minT,double maxT) -> std::shared_ptr<CollisionResult> { return nullptr;}

        virtual auto getBoundingBox() -> const BoundingBox&;

        virtual auto getUV(const Cartesian3 & pos) const -> std::pair<float,float>;

        virtual auto update(RenderParameters *renderParameters) -> bool;

        inline auto getTransform() -> Transform& { return transform;}
        
        auto updateTransform(const Matrix4 & parentTransform) -> void;


        virtual auto pdf(const Cartesian3 & o,const Cartesian3 & v) -> float{ return 0.0f;}
        virtual auto random(const Cartesian3 & o) -> Cartesian3{ return Cartesian3{1,0,0};}




        inline auto getShadingType() const -> ShadingType { return type; }
        inline auto setShadingType(ShadingType type) -> void { this->type = type; }

        
        inline auto getMaterial() const -> const std::shared_ptr<Material>& { return material;}


    protected:
        auto assembleCollisionResult(double t,const std::unique_ptr<Ray> & ray,const Cartesian3 & normal) ->  std::shared_ptr<CollisionResult> ;

        std::shared_ptr<Material> material;

        BoundingBox box;

        Transform transform;

        bool transformDirty = false;

        ShadingType type = ShadingType::Diffuse;


    };
}
