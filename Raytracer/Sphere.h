#ifndef SPHERE_H
#define SPHERE_H

#include "Mesh.h"
#include "../Cartesian3.h"

namespace ray {
    class Sphere : public Mesh
    {
    public:
        Sphere() = default;
        Sphere(const Cartesian3 & origin, float radius,const std::shared_ptr<Material> & material);

        auto intersects(const std::unique_ptr<Ray> & ray, double minT,double maxT) -> std::shared_ptr<CollisionResult>  override;
        auto getUV(const Cartesian3 & pos) const -> std::pair<float,float> override;

        inline auto getOrigin() const -> const Cartesian3 & { return origin;}
        inline auto getRadius() const -> float { return radius;}


        auto pdf(const Cartesian3 & o,const Cartesian3 & v) -> float override;
        auto random(const Cartesian3 & o) -> Cartesian3 override;


        static auto randomUnitSphere() -> Cartesian3;
        static auto randomInUnitSphere() -> Cartesian3;
        static auto randomCosineDirection() -> Cartesian3;
        static auto randomInHemiSphere(const Cartesian3 & normal) -> Cartesian3;
        static auto randomToSphere(float radius,float disatnceSquared) -> Cartesian3;
    private:
        Cartesian3 origin;
        float radius;
    };
};


#endif // SPHERE_H
