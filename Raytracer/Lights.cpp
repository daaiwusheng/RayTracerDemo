#include "Lights.h"
#include "../Randomizer.h"
namespace ray
{

    Lights::Lights()
    {

    }

    auto Lights::pdf(const Cartesian3 & o,const Cartesian3 & v) -> float
    {
        auto weight = 1.f / lights.size();
        auto sum = 0.f;
        for(auto & obj : lights){
            sum += weight * obj->pdf(o,v);
        }
        return sum;
    }

    auto Lights::random(const Cartesian3 & o) -> Cartesian3
    {
        auto index = Randomizer::get().nextInt(0,(int32_t)lights.size());
        return lights[index]->random(o);
    }

    auto Lights::addLight(const std::shared_ptr<Mesh> & m) -> void
    {
        lights.emplace_back(m);
    }

}

