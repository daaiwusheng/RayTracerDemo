#ifndef LIGHTS_H
#define LIGHTS_H

#include "Mesh.h"
#include <vector>
namespace ray
{
    class Lights : public Mesh
    {
    public:
        Lights();

        auto pdf(const Cartesian3 & o,const Cartesian3 & v) -> float;
        auto random(const Cartesian3 & o) -> Cartesian3;

        auto addLight(const std::shared_ptr<Mesh> & ) -> void;
    private:
        std::vector<std::shared_ptr<Mesh>> lights;
    };
}


#endif // LIGHTS_H
