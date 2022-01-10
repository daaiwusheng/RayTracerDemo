#ifndef PDF_H
#define PDF_H


#include "../Cartesian3.h"
#include "OBN.h"

#include <memory>

namespace ray
{
    class Mesh;


    //probability density function
    class PDF
    {
    public:
        virtual ~PDF() = default;

        virtual auto value(const Cartesian3 & direction) const -> float = 0;
        virtual auto generate() const -> Cartesian3 = 0;
    };

    //Random Hemisphere Sampling
    //p(direction) = 1/2PI;
    class RandomHemiShperePDF : public PDF
    {
    public:
        RandomHemiShperePDF(const Cartesian3 & normal);
        virtual auto value(const Cartesian3 & direction) const -> float override ;
        virtual auto generate() const -> Cartesian3 override ;
    private:
        Cartesian3  normal;
    };


    class CosinePDF : public PDF
    {
    public:
        CosinePDF(const Cartesian3 & normal);
        virtual auto value(const Cartesian3 & direction) const -> float override ;
        virtual auto generate() const -> Cartesian3 override ;
    private:
        OBN obn;
        Cartesian3 normal;
    };

    class MeshPDF : public PDF
    {
    public:
        MeshPDF(const std::shared_ptr<Mesh> & mesh, const Cartesian3 & origin);
        virtual auto value(const Cartesian3 & direction) const -> float override ;
        virtual auto generate() const -> Cartesian3 override ;
    private:
        std::shared_ptr<Mesh> mesh;
        Cartesian3 origin;
    };

    class MixturePDF : public PDF
    {
    public:
        MixturePDF(const std::shared_ptr<PDF> & a,const std::shared_ptr<PDF> & b);
        virtual auto value(const Cartesian3 & direction) const -> float override ;
        virtual auto generate() const -> Cartesian3 override ;
    private:
        std::shared_ptr<PDF> pdfs[2];
    };

}


#endif // PDF_H
