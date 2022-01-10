#include "Sphere.h"
#include <cmath>
#include "CollisionResult.h"
#include "../Randomizer.h"
#include <limits>
#include "../MemoryTools.h"
#include "OBN.h"
#include "Material.h"

namespace ray {

//create a sphere
    Sphere::Sphere(const Cartesian3 & o, float r,const std::shared_ptr<Material> & material)
        :Mesh(material),origin(o),radius(r)
    {
        box ={
            origin - Cartesian3{r,r,r},
            origin + Cartesian3{r,r,r}
        };
    }

    //dot[(A+t*B-C),(A+t*B-C)] = R^2
    //get the solution of t(unknown)

    //t^2*dot(B,B) + 2 * t * dot(A-C,A-C) + dot(C,C) - R^2 = 0

    auto Sphere::intersects(const std::unique_ptr<Ray> & ray, double minT,double maxT) -> std::shared_ptr<CollisionResult>
    {
        auto rayToSphere = ray->getOrigin() - origin;
        auto a = ray->getDirection().squared();
        auto b = rayToSphere.dot(ray->getDirection());
        auto c = rayToSphere.squared() - radius * radius;

        auto discriminant = b*b - a*c;
        if (discriminant < 0) return nullptr;

        auto sqrtd = std::sqrt(discriminant);


        auto root = (-b - sqrtd) / a;
        if(root < minT || root > maxT){
            root = (-b + sqrtd) / a;
            if(root < minT || root > maxT){
                return nullptr;
            }
        }

        return assembleCollisionResult(root,ray, (ray->getPosition(root) - origin) / radius);
    }


    auto Sphere::pdf(const Cartesian3 & o,const Cartesian3 & v) -> float
    {

        static constexpr auto MAX = std::numeric_limits<double>::infinity();
        static constexpr auto MIN_TOL = 0.0001;

        auto result = intersects(std::makeUnique<Ray>(o,v),MIN_TOL,MAX);

        if(result == nullptr){
            return 0;
        }

        auto cos = std::sqrt(1 - radius * radius / (origin - o).squared());

        auto solidAngle = M_PI_2 * (1 - cos);

        return 1/ solidAngle;

    }

    auto Sphere::random(const Cartesian3 & o) -> Cartesian3
    {


        OBN obn;
        auto dir = origin - o;
        auto dis = dir.squared();
        obn.buildFromW(dir);
        return obn.local(randomToSphere(radius,dis)).unit();
    }

    //calculate UV in a sphere surface
    auto Sphere::getUV(const Cartesian3 & pos) const -> std::pair<float,float>
    {
        auto theta = std::acos(-pos.y);
        auto phi = std::atan2(-pos.z, pos.x) + M_PI;
        return { phi / (M_PI_2), theta / M_PI};
    }



    //utility for generating sphere..
    //random a unit sphere..
    auto Sphere::randomInUnitSphere() -> Cartesian3
    {
       return randomUnitSphere().unit();
    }

    auto Sphere::randomUnitSphere() -> Cartesian3
    {
        while (true) {
           auto p = Cartesian3::randomVector(-1,1);
           if (p.squared() >= 1) continue;

           return p;
        }
    }



     auto Sphere::randomToSphere(float radius,float disatnceSquared) -> Cartesian3
     {

         auto r1 = Randomizer::random();
         auto r2 = Randomizer::random();

         auto z = 1 + r2 * (std::sqrt(1 - radius * radius / disatnceSquared) - 1);

         auto phi = M_PI_2 * r1;
         auto x = std::cos(phi) * std::sqrt(1 - z *z );
         auto y = std::sin(phi) * std::sqrt(1 - z *z );

         return {static_cast<float>(x),static_cast<float>(y),z};
     }

    auto Sphere::randomInHemiSphere(const Cartesian3 & normal) -> Cartesian3
    {
        auto sphere = randomInUnitSphere();
        return sphere.dot(normal) < 0 ? sphere * -1 : sphere;
    }


    auto Sphere::randomCosineDirection() -> Cartesian3
    {
        auto r1 = Randomizer::random();
        auto r2 = Randomizer::random();
        auto z= std::sqrt(1-r2);
        auto phi = M_PI_2 * r1;

        auto x = std::cos(phi) * std::sqrt(r2);
        auto y = std::sin(phi) * std::sqrt(r2);

        return Cartesian3{static_cast<float>(x),static_cast<float>(y),z};
    }
}
