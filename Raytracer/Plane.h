#pragma once

#include "Mesh.h"


namespace ray
{

    enum class PlaneType
    {
        XY_PLANE,
        XZ_PLANE,
        YZ_PLANE
    };

    class Plane : public Mesh
    {
    public:
        Plane(const std::pair<float,float> & min,const std::pair<float,float> & max,float k, const PlaneType & initType,const std::shared_ptr<Material> & material);
        auto intersects(const std::unique_ptr<Ray> & ray, double minT,double maxT) -> std::shared_ptr<CollisionResult> override;
        inline auto setFlipFace(bool flip) -> void { this->flipFace = flip;}
    private:

        auto pdf(const Cartesian3 & o,const Cartesian3 & v) -> float override;
        auto random(const Cartesian3 & o) -> Cartesian3 override;

        Cartesian3 min;
        Cartesian3 max;
        PlaneType type;
        float k;
        bool flipFace = false;
    };
}
