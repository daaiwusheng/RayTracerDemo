#pragma once

#include "Cartesian3.h"
#include <memory>

namespace ray{
    class Ray;
}

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(const Cartesian3 & min,const Cartesian3 & max);
    inline auto getMin() const -> const Cartesian3 & { return min;}
    inline auto getMax() const -> const Cartesian3 & { return max;}


    inline auto getMin()-> Cartesian3 & { return min;}
    inline auto getMax()-> Cartesian3 & { return max;}

    auto intersects(const std::unique_ptr<ray::Ray> & ray, float minT,float maxT) -> bool;

    auto intersects(const BoundingBox & box) -> bool;

    auto merge(const BoundingBox & box) -> void;

    auto operator+=(const Cartesian3 & offset) -> BoundingBox;

    static auto setBox(BoundingBox & box,const std::initializer_list<Cartesian3> list) -> void;



    Cartesian3 min;
    Cartesian3 max;
};

