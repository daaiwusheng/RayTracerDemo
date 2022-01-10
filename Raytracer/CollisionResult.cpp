#include "CollisionResult.h"
#include "Ray.h"
namespace ray {

    CollisionResult::CollisionResult()
    {

    }

    /**
     * @brief CollisionResult::setFaceNormal    set current normal of face
     * @param ray
     * @param outNormal
     */
    auto CollisionResult::setFaceNormal(const std::unique_ptr<Ray> & ray, const Cartesian3 & outNormal) -> void
    {
        //angle between ray and face normal is greater than 90 degree;
        front = ray->getDirection().dot(outNormal) < 0;
        normal = front ? outNormal :-1 * outNormal;
        normal.normalize();
    }
}
