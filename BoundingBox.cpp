
#include "BoundingBox.h"
#include "Raytracer/Ray.h"
#include <cmath>

BoundingBox::BoundingBox()
    :BoundingBox({0,0,0.f},{0,0,0.f})
{}

BoundingBox::BoundingBox(const Cartesian3 & min,const Cartesian3 & max)
    :min(min),max(max)
{

}



/**
  *x = ax + t*bx  -> bx is the direction
  *t = (x - ax) / bx
  *t0 = (x0 - ax) / bx
  *t1 = (x1 - ax) / bx
  *
  * extend to x,y,z direction ......
  *
 */
auto BoundingBox::intersects(const std::unique_ptr<ray::Ray> & ray, float minT,float maxT) -> bool
{
    //in x,y,z dir
    for (auto i = 0; i < 3; i++) {
       auto invD = 1.0f / ray->getDirection()[i];
       auto t0 = (min[i] - ray->getOrigin()[i]) * invD;
       auto t1 = (max[i] - ray->getOrigin()[i]) * invD;
       if (invD < 0.0f)
           std::swap(t0, t1);
       minT = std::min(t0,minT);
       maxT = std::max(t1,maxT);
       if (maxT <= minT)
           return false;
    }
    return true;
}


auto BoundingBox::intersects(const BoundingBox & aabb) -> bool
{
    for (std::size_t i = 0; i < 3; ++i)
        if (aabb.min[i] > max[i]) return false;
    for (std::size_t i = 0; i < 3; ++i)
        if (aabb.max[i] < min[i]) return false;
    return true;
}

auto BoundingBox::merge(const BoundingBox & box) -> void
{
    for (auto i = 0; i < 3; ++i)
        min[i] = std::min(min[i], box.min[i]);
    for (auto i = 0; i < 3; ++i)
        max[i] = std::max(max[i], box.max[i]);
}


auto BoundingBox::operator+=(const Cartesian3 & offset) -> BoundingBox
{
    return {min+offset, max+offset};
}


auto BoundingBox::setBox(BoundingBox & box,const std::initializer_list<Cartesian3> list) -> void
{
    for(auto it = list.begin();it!=list.end();it++)
    {
        for(int32_t i = 0;i<3;i++){
            if ((*it)[i] < box.getMin()[i]) box.getMin()[i] = (*it)[i] ;
        }
        for(int32_t i = 0;i<3;i++){
            if ((*it)[i]  > box.getMax()[i]) box.getMax()[i] = (*it)[i] ;
        }
    }
}


