#pragma once

#include <memory>
#include "../Cartesian3.h"
#include "../Renderer/Color.h"
#include "ColorTexture.h"


namespace ray
{
    class Ray;
    class CollisionResult;
    class PDF;

    struct ScatterResult
    {
        std::unique_ptr<Ray> specularRay;
        bool specular;
        Color attenuation;
        std::shared_ptr<PDF> pdf;
    };




    class Material
    {
    public:
        enum class MaterialType{
            Lambertian,
            Dielectric,
            Metal,
            DiffuseLight
        };

        Material() = default;
        virtual ~Material() = default;

        virtual auto scatter(const std::unique_ptr<Ray> & rayIn, const std::shared_ptr<CollisionResult> & result) const -> std::unique_ptr<ScatterResult> { return nullptr;}

        virtual auto emitted(const std::unique_ptr<Ray> & rayIn, const std::shared_ptr<CollisionResult> & result,const std::pair<float,float> & uv) -> Color { return {};}

        virtual auto scatterPdf(const std::unique_ptr<Ray> & rayIn, const std::shared_ptr<CollisionResult> & result,const std::unique_ptr<Ray> & scatter) -> float { return 0; }

        virtual auto getType() -> MaterialType = 0;

    };


    class Lambertian : public Material
    {
    public:
        Lambertian(const std::shared_ptr<Texture> & color);
        virtual auto scatter(const std::unique_ptr<Ray> & rayIn, const std::shared_ptr<CollisionResult> & result )const -> std::unique_ptr<ScatterResult> override;
        virtual auto scatterPdf(const std::unique_ptr<Ray> & rayIn, const std::shared_ptr<CollisionResult> & result,const std::unique_ptr<Ray> & scatter) -> float;
        virtual auto getType() -> MaterialType override { return MaterialType::Lambertian;}
    private:
        std::shared_ptr<Texture> albedo;
    };


    class Dielectric : public Material
    {
    public:
        Dielectric(double ratio);
        virtual auto scatter(const std::unique_ptr<Ray> & rayIn, const std::shared_ptr<CollisionResult> & result) const -> std::unique_ptr<ScatterResult> override;
        virtual auto scatterPdf(const std::unique_ptr<Ray> & rayIn, const std::shared_ptr<CollisionResult> & result,const std::unique_ptr<Ray> & scatter) -> float;
        virtual auto getType() -> MaterialType override { return MaterialType::Dielectric;}
    private:
        double ratio;
    };





    class Metal : public Material
    {
    public:
        Metal(const std::shared_ptr<Texture> & initAlbedo,float initFuzz);
        virtual auto scatter(const std::unique_ptr<Ray> & rayIn, const std::shared_ptr<CollisionResult> & result) const -> std::unique_ptr<ScatterResult> override;
         virtual auto getType() -> MaterialType override { return MaterialType::Metal;}
    private:
        std::shared_ptr<Texture> albedo;
        float fuzz;
    };


}
