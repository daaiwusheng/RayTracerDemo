#include "Material.h"
#include "Ray.h"
#include "CollisionResult.h"
#include "../Renderer/MathUtils.h"
#include "../MemoryTools.h"
#include "../Randomizer.h"
#include "Sphere.h"
#include "OBN.h"
#include "PDF.h"


namespace ray
{
    //create a Lambertian diffuse material
    Lambertian::Lambertian(const std::shared_ptr<Texture> & color)
        :albedo(color)
    {

    }

    //Lambertian
    auto Lambertian::scatter(const std::unique_ptr<Ray> & rayIn, const std::shared_ptr<CollisionResult> & result) const -> std::unique_ptr<ScatterResult>
    {

        auto scatterResult = std::makeUnique<ScatterResult>();
        scatterResult->specular = false;//this is not specular object.
        //sample color.
        scatterResult->attenuation = albedo->sample({result->u,result->v});
        //create pdf
        scatterResult->pdf = std::makeShared<RandomHemiShperePDF>(result->normal);

        return scatterResult;
    }


    /**
     * MC basics:
     * Integration f(x) = f(r)/p(r)
     * p(direction)=cos(sita)/PI.
     */
    auto Lambertian::scatterPdf(const std::unique_ptr<Ray> & rayIn, const std::shared_ptr<CollisionResult> & result,const std::unique_ptr<Ray> & scatter) -> float
    {
        auto cos = result->normal.dot(scatter->getDirection().unit());
        return cos < 0? 0 : cos / M_PI;
    }




    Dielectric::Dielectric(double ratio)
        :ratio(ratio)
    {

    }


    auto Dielectric::scatter(const std::unique_ptr<Ray> & ray, const std::shared_ptr<CollisionResult> & result) const -> std::unique_ptr<ScatterResult>
    {

        double refractionRatio = result->front ? (1.0/ratio) : ratio;
        auto direction = ray->getDirection();
        direction.normalize();
        double cosTheta = std::fmin((direction * -1).dot(result->normal), 1.0);
        double sinTheta = std::sqrt(1.0 - cosTheta*cosTheta);
        bool reflect = refractionRatio * sinTheta > 1.0;
        if (reflect || MathUtils::reflectance(cosTheta, ratio) > Randomizer::random())
           direction = MathUtils::reflect(direction,result->normal);
        else
           direction = MathUtils::refract(direction,result->normal, refractionRatio);

        auto scatterResult = std::makeUnique<ScatterResult>();
        scatterResult->attenuation =  {1.0, 1.0, 1.0,1.0f};
        scatterResult->specular = true;

        scatterResult->specularRay = std::makeUnique<Ray>(result->position,direction);
        return scatterResult;
    }


    auto Dielectric::scatterPdf(const std::unique_ptr<Ray> & rayIn, const std::shared_ptr<CollisionResult> & result,const std::unique_ptr<Ray> & scatter) -> float
    {
        //auto cos = result->normal.dot(scatter->getDirection().unit());
        //return cos;
        return 0;
    }




    Metal::Metal(const std::shared_ptr<Texture> & initAlbedo,float initFuzz)
        :albedo(initAlbedo),fuzz(std::min(initFuzz,1.f))
    {

    }

    auto Metal::scatter(const std::unique_ptr<Ray> & rayIn, const std::shared_ptr<CollisionResult> & result) const -> std::unique_ptr<ScatterResult>
    {
        auto reflected = MathUtils::reflect(rayIn->getDirection().unit(), result->normal);

        auto scatterResult = std::makeUnique<ScatterResult>();
        scatterResult->attenuation =  albedo->sample({result->u,result->v});
        scatterResult->specular = true;
        scatterResult->specularRay = std::makeUnique<Ray>(result->position,reflected + fuzz * Sphere::randomInUnitSphere(),rayIn->getTime());

        return scatterResult;
    }



}
