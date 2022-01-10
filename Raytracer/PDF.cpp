#include "PDF.h"
#include <cmath>
#include "Sphere.h"
#include "../Randomizer.h"
namespace ray
{


    RandomHemiShperePDF::RandomHemiShperePDF(const Cartesian3 & normal)
        :normal(normal)
    {

    }

    //p(direction) = 1/2PI;
    auto RandomHemiShperePDF::value(const Cartesian3 & direction) const -> float
    {
        return 0.5 / M_PI;
    }

    auto RandomHemiShperePDF::generate() const -> Cartesian3
    {
        return Sphere::randomInHemiSphere(normal).unit();
    }

    CosinePDF::CosinePDF(const Cartesian3 & normal)
        :normal(normal)
    {
         obn.buildFromW(normal);
    }

    //p(directions)=cos(sita)/PI
    auto CosinePDF::value(const Cartesian3 & direction) const -> float
    {
        auto cos = direction.unit().dot(obn[2]);
        return cos <= 0 ? 0 : cos / M_PI;
    }

    //there is a bug here..
    auto CosinePDF::generate() const -> Cartesian3
    {
      return obn.local(Sphere::randomCosineDirection());
    }


    MeshPDF::MeshPDF(const std::shared_ptr<Mesh> & mesh, const Cartesian3 & origin)
        :mesh(mesh),origin(origin)
    {

    }

    auto MeshPDF::value(const Cartesian3 & direction) const -> float
    {
        return mesh->pdf(origin,direction);
    }

    auto MeshPDF::generate() const -> Cartesian3
    {
        return mesh->random(origin);
    }


    // mixtured pdf with two pdfs.
    MixturePDF::MixturePDF(const std::shared_ptr<PDF> & a,const std::shared_ptr<PDF> & b)
    {
        pdfs[0] = a;
        pdfs[1] = b;
    }

    auto MixturePDF::value(const Cartesian3 & direction) const -> float
    {

        return 0.5*(pdfs[0]->value(direction) + pdfs[1]->value(direction));
    }

    auto MixturePDF::generate() const -> Cartesian3
    {
        if(Randomizer::get().nextBool()){
            return pdfs[0]->generate();
        }
        return pdfs[1]->generate();
    }

}
