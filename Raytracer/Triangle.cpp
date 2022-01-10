

#include "Triangle.h"
#include "Ray.h"
#include <cmath>
#include "CollisionResult.h"
namespace ray
{
    Triangle::Triangle(const Cartesian3 & v1,const Cartesian3 & v2,const Cartesian3 & v3,const std::shared_ptr<Material> & material)
        :Mesh(material),v1(v1),v2(v2),v3(v3)
    {
        box = {{INFINITY,INFINITY,INFINITY},{-INFINITY,-INFINITY,-INFINITY}};

        BoundingBox::setBox(box,{v1,v2,v3});

    }

    //o + td = aA + uB + vC;
    //o + td = (1-u-v)A + + uB + vC;

    //intersects with a triangle
    auto Triangle::intersects(const std::unique_ptr<Ray> & ray, double minT,double maxT) -> std::shared_ptr<CollisionResult>
    {

        auto & trans = transform.getTransform();

        auto v1 = trans * this->v1;
        auto v2 = trans * this->v2;
        auto v3 = trans * this->v3;

        //calculate the position after transformation

        auto edge1 = v2 - v1;
        auto edge2 = v3 - v1;

        auto vec = ray->getDirection().cross(edge2);
        float delta = edge1.dot(vec);

        if(std::fabs(delta) < 1e-8){//near zero
            return nullptr;
        }

        float inverse = 1.f / delta;

        auto v1ToRay = ray->getOrigin() - v1;

        auto u = v1ToRay.dot(vec) * inverse;
        if( u < 0 || u > 1){
            return nullptr;
        }


        auto qvec = v1ToRay.cross(edge1);
        auto v = ray->getDirection().dot(qvec) * inverse;
        if( v < 0 || u + v> 1){
            return nullptr;
        }

        auto t = edge2.dot(qvec) * inverse;

        //
        auto ret = assembleCollisionResult(t,ray,edge1.cross(edge2));
        //sample in texture...
        //so  calculate the uv with the uv;
        //barycentric interpolation
        Cartesian3 pos =  u * uv + v * uv1 + (1-u-v)*uv2;
        ret->u = pos.x;
        ret->v = pos.y;
        return ret;
    }

    //set the three tex coords
    auto Triangle::setTexCoord(const Cartesian3 & v1,const Cartesian3 & v2,const Cartesian3 & v3) -> void
    {
         uv = v1;
         uv1 = v2;
         uv2 = v3;
    }
}

