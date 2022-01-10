#ifndef MATHUTILS_H
#define MATHUTILS_H

#include "../Homogeneous4.h"
#include "../RGBAValue.h"
#include <cmath>

namespace MathUtils {


    template<typename T>
    inline auto lerp(const T & a,const T & b,float delta) -> T{
         return a * (1-delta)  + b * delta ;
    }
    template<typename T>
    inline auto radians(T angle) -> T{
        return angle / 180.f * M_PI;
    }

    template<typename T>
    inline auto clamp(T x, T min, T max) -> T{
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    inline auto reflect(const Cartesian3 & vec,const Cartesian3 & normal) -> Cartesian3
    {
        float dn = 2 * vec.dot(normal);
        return vec - normal * dn;
    }

    //n⋅sin(a) = n′⋅sin(a)′
    inline auto refract(const Cartesian3 & vec,const Cartesian3 & normal, double angle) -> Cartesian3
    {
        auto cosTheta = std::fmin(normal.dot(vec * -1), 1.0);
        auto perp =  angle * (vec + cosTheta * normal);
        auto parallel = -sqrt(fabs(1.0 - perp.squared())) * normal;
        return perp + parallel;
    }


    inline auto reflectance(double cosine, double ref) -> double
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1-ref) / (1+ref);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine),5);
    }


};


#endif // MATHUTILS_H
