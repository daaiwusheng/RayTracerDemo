#ifndef COLLISIONRESULT_H
#define COLLISIONRESULT_H
#include "../Cartesian3.h"
#include <memory>


namespace ray {
    class Ray;
    class Material;
    class Mesh;
    class CollisionResult
    {
    public:
        CollisionResult();
        auto setFaceNormal(const std::unique_ptr<Ray> & ray, const Cartesian3 & outNormal) -> void;

    public:
        Cartesian3 position;
        Cartesian3 normal;
        double t;
        bool front;
        Mesh * hitObject = nullptr;
        std::shared_ptr<Material> material;
        float u; // texture coords;
        float v; // texture coords;
    };

}

#endif // COLLISIONRESULT_H
