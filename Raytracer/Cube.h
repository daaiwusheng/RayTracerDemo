#pragma once

#include "../Cartesian3.h"
#include "Mesh.h"
#include <vector>


namespace ray
{
    class BVHTree;
    class Cube : public Mesh
    {
    public:
        Cube(const Cartesian3 & lt, const Cartesian3 & rb, const std::shared_ptr<Material> & material);
        auto intersects(const std::unique_ptr<Ray> & ray, double minT,double maxT) -> std::shared_ptr<CollisionResult> override;

    private:
        const Cartesian3 leftTop;
        const Cartesian3 rightBottom;
        std::shared_ptr<BVHTree> tree;
        std::vector<std::shared_ptr<Mesh>> slides;
    };
}
