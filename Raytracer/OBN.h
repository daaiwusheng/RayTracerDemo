#ifndef OBN_H
#define OBN_H

#include "../Cartesian3.h"

//Orthonormal Basis

namespace ray
{
    class OBN
    {
    public:
        OBN();

        //u.v.w
        inline auto operator[](int32_t i) const -> const Cartesian3 & { return axis[i]; }

        auto local(const Cartesian3 & a) const -> Cartesian3;

        auto buildFromW(const Cartesian3 & a) -> void;

    private:
        Cartesian3 axis[3];

    };
}
#endif // OBN_H
