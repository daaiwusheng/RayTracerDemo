#include "Plane.h"
#include "CollisionResult.h"
#include "Ray.h"
#include "../MemoryTools.h"
#include "../Randomizer.h"
#include "Material.h"
#include <limits>

namespace ray
{

    //create plane
    Plane::Plane(const std::pair<float,float>  & initMin,const std::pair<float,float> & initMax, float k, const PlaneType & initType,const std::shared_ptr<Material> & material)
        :Mesh(material),type(initType),k(k)
    {

        switch (type) {
            case PlaneType::XY_PLANE:
                min = {initMin.first,initMin.second,k-0.0001f};
                max = {initMax.first,initMax.second,k+0.0001f};
            break;

            case PlaneType::XZ_PLANE:
                min = {initMin.first,k-0.0001f,initMin.second};
                max = {initMax.first,k+0.0001f,initMax.second};
            break;

            case PlaneType::YZ_PLANE:
                min = {k-0.0001f,initMin.first,initMin.second};
                max = {k+0.0001f,initMax.first,initMax.second};
            break;
        }

    }



    //simple with light
    //                dA * cos(alpha)
    //          dw = ------------------
    //                distance^2(p,q)
    //
    //
    //                      dA * cos(alpha)                  dA
    //   p(direction) *------------------------ = p(q)*dA = ----
    //                      distance^2(p,q)                   A
    //So
    //                      distance^2(p,q)
    //   p(direction) = ------------------------
    //                       A * cos(alpha)
    //
    auto Plane::pdf(const Cartesian3 & o,const Cartesian3 & v) -> float
    {

        auto result = intersects(std::makeUnique<Ray>(o,v),0.0001,std::numeric_limits<double>::infinity());


        if(material != nullptr && material->getType() == Material::MaterialType::Lambertian){
            return 0.f;
        }

        if(result == nullptr){
            return 0;
        }

        float area = 1.f;

        switch (type) {
            case PlaneType::XY_PLANE:
                area = (max.x - min.x )*(max.y-min.y);
            break;

            case PlaneType::XZ_PLANE:
                area = (max.x - min.x )*(max.z-min.z);
            break;

            case PlaneType::YZ_PLANE:
                area = (max.y - min.y )*(max.z-min.z);
            break;
        }


        auto distance = result->t * result->t * v.squared();
        auto cos = std::fabs(v.dot(result->normal)  / v.length());

        return distance / (cos * area);

    }


    auto Plane::random(const Cartesian3 & origin) -> Cartesian3
    {

        if(material != nullptr && material->getType() == Material::MaterialType::Lambertian)
        {
           return Mesh::random(origin);
        }

        Randomizer r;
        Cartesian3 pos;

        switch (type) {
        case PlaneType::XZ_PLANE:

            pos = Cartesian3{
                        r.nextReal(min.x,max.x),
                        k,
                        r.nextReal(min.z,max.z),
                    };

            break;
        case PlaneType::XY_PLANE:

            pos = Cartesian3{
                        r.nextReal(min.x,max.x),
                        r.nextReal(min.y,max.y),
                        k
                       };

            break;
        case PlaneType::YZ_PLANE:

            pos = Cartesian3{
                        k,
                        r.nextReal(min.y,max.y),
                        r.nextReal(min.z,max.z),
                       };

            break;

        }
        return pos- origin;

    }


    //intersects with plane
    auto Plane::intersects(const std::unique_ptr<Ray> & newRay, double minT,double maxT) -> std::shared_ptr<CollisionResult>
    {


        switch (type) {
            case PlaneType::XY_PLANE:
        {
            auto t = (k - newRay->getOrigin().z) / newRay->getDirection().z;
            if (t < minT || t > maxT)
                 return nullptr;
            auto x = newRay->getOrigin().x + t * newRay->getDirection().x;
            auto y = newRay->getOrigin().y + t * newRay->getDirection().y;
            if (x < min.x || x > max.x || y < min.y || y > max.y)
                return nullptr;

            auto result = assembleCollisionResult(t,newRay,{0,0,1});
            result->u = (x-min.x) / (max.x-min.x);
            result->v = (y-min.y) / (max.y-min.y);
            if(flipFace)
                result->front = !result->front;
            return result;
        }
            break;
            case PlaneType::XZ_PLANE:
        {
            auto t = (k - newRay->getOrigin().y) / newRay->getDirection().y;
            if (t < minT || t > maxT)
                 return nullptr;
            auto x = newRay->getOrigin().x + t * newRay->getDirection().x;
            auto z = newRay->getOrigin().z + t * newRay->getDirection().z;
            if (x < min.x || x > max.x || z < min.z || z > max.z)
                return nullptr;

            auto result = assembleCollisionResult(t,newRay,{0,1,0});
            result->u = (x-min.x) / (max.x-min.x);
            result->v = (z-min.z) / (max.z-min.z);
            if(flipFace)
                result->front = !result->front;
            return result;
        }
            break;

            case PlaneType::YZ_PLANE:


        {
            auto t = (k - newRay->getOrigin().x) / newRay->getDirection().x;
            if (t < minT || t > maxT)
                 return nullptr;
            auto y = newRay->getOrigin().y + t * newRay->getDirection().y;
            auto z = newRay->getOrigin().z + t * newRay->getDirection().z;
            if (y < min.y || y > max.y || z < min.z || z > max.z)
                return nullptr;

            auto result = assembleCollisionResult(t,newRay,{1,0,0});
            result->u = (y-min.y) / (max.y-min.y);
            result->v = (z-min.z) / (max.z-min.z);
            if(flipFace)
                result->front = !result->front;
            return result;
        }

            break;
        }
        return nullptr;
    }

}
