#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../Cartesian3.h"
#include "Mesh.h"

namespace ray
{
    class Triangle : public Mesh
    {
    public:
        Triangle(const Cartesian3 & v1,const Cartesian3 & v2,const Cartesian3 & v3,const std::shared_ptr<Material> & material);
        auto intersects(const std::unique_ptr<Ray> & ray, double minT,double maxT) -> std::shared_ptr<CollisionResult> override;
        auto setTexCoord(const Cartesian3 & v1,const Cartesian3 & v2,const Cartesian3 & v3) -> void;
    private:
        Cartesian3  v1;
        Cartesian3  v2;
        Cartesian3  v3;
        Cartesian3 uv;
        Cartesian3 uv1;
        Cartesian3 uv2;
    };
}


#endif // TRIANGLE_H
