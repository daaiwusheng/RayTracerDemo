#include "OBN.h"

#include <cmath>

namespace ray
{

    OBN::OBN()
    {

    }



    //Random vector=xs+yt+zn
    auto OBN::local(const Cartesian3 & a) const -> Cartesian3
    {
        Cartesian3 local = {0,0,0};
        for(auto i = 0;i<3;i++){
            local+= a[i] * axis[i];
        }
        return local;
    }


    auto OBN::buildFromW(const Cartesian3 & a) -> void
    {
        axis[2] = a.unit();
        Cartesian3 v =(std::fabs(axis[2].x) > 0.9) ? Cartesian3{0,1,0} : Cartesian3{1,0,0};
        axis[1] =  axis[2].cross(v).unit();
        axis[0] =  axis[2].cross(axis[1]);
    }

}
