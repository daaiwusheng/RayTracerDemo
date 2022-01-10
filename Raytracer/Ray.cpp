
#include "Ray.h"

namespace ray
{
    Ray::Ray()
    {

    }

    Ray::Ray(const Cartesian3 & origin,const Cartesian3 & dir,float t)
        :origin(origin),dir(dir),time(t)
    {

    }


    auto Ray::getPosition(double dt) const -> Cartesian3
    {
        return origin + dt * dir;
    }
}
