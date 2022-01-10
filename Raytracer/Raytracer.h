#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <memory>
#include "Camera.h"
#include "Scene.h"
#include "../RGBAImage.h"
#include "../Renderer/Color.h"
#include "../TexturedObject.h"
#include "Material.h"


class RenderParameters;
namespace ray
{
    class Ray;

    class Raytracer
    {
    public:

        Raytracer();
        auto loadScene(const std::string & name) -> void;

        auto resize(int32_t w,int32_t h ) -> void;
        auto render(RenderParameters *renderParameters) -> void;
        auto sendRay(float u,float v,RenderParameters *renderParameters) -> RGBAValue;
        auto worldRay(const std::unique_ptr<Ray> & ray,const std::shared_ptr<Mesh> & light,int32_t depth,RenderParameters *renderParameters) -> Color;

        auto gammaCorrect(const Color & color,int32_t samples) -> RGBAValue;

        auto update(RenderParameters *renderParameters) -> void;

        auto blinnPhongShading(const std::unique_ptr<Ray> & ray,const std::shared_ptr<CollisionResult> & result,RenderParameters *renderParameters)-> Color;


        auto simpleLighting(const std::unique_ptr<Ray> & rayIn,const std::shared_ptr<Mesh> & light,const std::shared_ptr<CollisionResult> & result,const std::unique_ptr<ScatterResult> & scatterResult,float & pdf) -> std::unique_ptr<Ray>;

        auto showDefault() -> void;

        std::unique_ptr<Camera> camera;
        Scene scene;
        RGBAImage frameBuffer;
        RGBAImage * texture;

        Color background;

        TexturedObject obj;


        int32_t SAMPLE_COUNT = 100;
        int32_t MAX_DEPTH = 4;

    };
}
#endif // RAYTRACER_H
