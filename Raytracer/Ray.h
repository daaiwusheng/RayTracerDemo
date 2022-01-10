#ifndef RAY_H
#define RAY_H

//#include "Cartesian3.h"
#include "../Cartesian3.h"
//create a ray with formula
//𝐏(𝑡)=𝐀+𝑡𝐛
namespace ray {
    class Ray
    {
    public:
        Ray();

        Ray(const Cartesian3 & origin,const Cartesian3 & dir,float time = 0.f);


        inline auto getOrigin() const -> const Cartesian3& { return origin;}
        inline auto getDirection() const -> const Cartesian3& { return dir;}
        inline auto getTime() const -> const float { return time; }


        //𝐏(𝑡)=𝐀+𝑡𝐛
        auto getPosition(double dt) const -> Cartesian3;



    private:
        Cartesian3 origin;
        Cartesian3 dir;
        float time = 0.f;

    };
}





#endif // RAY_H
