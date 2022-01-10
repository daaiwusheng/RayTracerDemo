#include "Raytracer.h"

#include "../Renderer/MathUtils.h"

#include "../RenderParameters.h"
#include "CollisionResult.h"
#include "Sphere.h"
#include <limits>
#include "../Randomizer.h"
#include "Material.h"
#include "../MemoryTools.h"
#include "Plane.h"
#include "Triangle.h"
#include "DiffuseLight.h"
#include "Cube.h"
#include "TriangleMesh.h"
#include "Serialization.h"
#include "PDF.h"
#include "TexturePool.h"


namespace ray
{
    Raytracer::Raytracer()
    {
        SAMPLE_COUNT = 20;
        MAX_DEPTH = 3;
    }



    auto Raytracer::loadScene(const std::string & name) -> void
    {

        std::ifstream in(name);

        if(in.good()){
            std::string type;
            in >> type;

            if(type == "SAMPLE_COUNT"){
                in >> SAMPLE_COUNT;
            }

            in >> type;
            if(type == "MAX_DEPTH"){
                in >> MAX_DEPTH;
            }
            in >> type;
            if(type == "Camera"){
                Cartesian3 position;
                Cartesian3 lookAt;
                Cartesian3 up;
                float fov;
                float aspectRatio;

                in >> position >> lookAt >> up >> fov >> aspectRatio;

                camera = std::makeUnique<Camera>(position,lookAt,up,fov,aspectRatio);
                camera->recalculate();
            }

            in >> type;
            if(type == "BACKGROUND")
            {
                float r,g,b,a;
                in >> r >> g >> b>> a;
                background = {r,g,b,a};
            }

            in >> scene;
            in.close();
        }
        else
        {
            std::cout<<"warring : no file load or file is invalid. the default scene will be loaded"<<std::endl;
            showDefault();
        }
        scene.constructBVHTree();
    }




    auto Raytracer::resize(int32_t w,int32_t h ) -> void
    {
        //resize frame buffer and recalculate the camera
        frameBuffer.Resize(w,h);
        if(camera != nullptr){
            camera->setAspectRatio(w/(float)h);
            camera->recalculate();
        }
    }

    auto Raytracer::render(RenderParameters *renderParameters) -> void
    {
        update(renderParameters);

        //in order to avoid multithread problems
        auto copyParam =  *renderParameters;
    #pragma omp parallel for
        for (auto j = frameBuffer.height - 1; j >= 0; --j) {
            for (auto i = 0; i < frameBuffer.width; ++i) {
                auto u = double(i) / (frameBuffer.width-1);
                auto v = double(j) / (frameBuffer.height-1);
                frameBuffer[j][i] = sendRay(u,v,&copyParam);
            }
        }
    }

    auto Raytracer::update(RenderParameters *renderParameters) -> void
    {
        scene.update(renderParameters);
    }

    auto Raytracer::sendRay(float u,float v,RenderParameters *renderParameters) -> RGBAValue
    {

        Color color{0,0,0,1.f};
        if(camera != nullptr)
            for(auto i = 0;i<SAMPLE_COUNT;i++){
                color = color + worldRay(camera->castRay(u,v),scene.getLights(),MAX_DEPTH,renderParameters);
            }
        if(renderParameters->showGamma)
            return gammaCorrect(color,SAMPLE_COUNT);

        auto c = color/SAMPLE_COUNT;

        return {c.r() * 255.f,c.g()* 255.f,c.b()* 255.f,255.f};;

    }


    //gamma correct
    auto Raytracer::gammaCorrect(const Color & color,int32_t samples) -> RGBAValue
    {

        float r = color.r();
        float g = color.g();
        float b = color.b();

        if(r !=r ){
            r = 0.0f;
        }
        if(g !=g ){
            g = 0.0f;
        }

        if(b !=b ){
            b = 0.0f;
        }

        auto scale = 1.0 / samples;
        r = std::sqrt(scale * r);
        g = std::sqrt(scale * g);
        b = std::sqrt(scale * b);
        return {r * 255.f,g* 255.f,b* 255.f,255.f};
    }


    auto Raytracer::worldRay(const std::unique_ptr<Ray> & ray,const std::shared_ptr<Mesh> & light,int32_t depth,RenderParameters *renderParameters) -> Color
    {
        static constexpr auto MAX = std::numeric_limits<double>::infinity();
        static constexpr auto MIN_TOL = 0.0001;

        if (depth <= 0)
            return background;

        //send ray to the scene.
        auto result = scene.intersects(ray,MIN_TOL,MAX);


        if(result != nullptr ){

            //shading with blinnPhone mode.
            if(result->hitObject->getShadingType() == ShadingType::BlinnPhong)
            {
                return blinnPhongShading(ray,result,renderParameters);
            }
            else
            {

                auto scatterResult = result->material->scatter(ray,result);
                auto emitted = result->material->emitted(ray,result,{result->u,result->v});

                if(scatterResult == nullptr){
                    return emitted;
                }


                if(scatterResult->specular){
                    return scatterResult->attenuation * worldRay(scatterResult->specularRay,light,depth - 1,renderParameters);
                }

                float pdf = 1.f;
                auto scatterRay = simpleLighting(ray,light,result,scatterResult,pdf);


                //pdf value is used to monte carlo integration
                pdf = 1.f / pdf;


                return emitted + scatterResult->attenuation * worldRay(scatterRay,light,depth - 1,renderParameters)* result->material->scatterPdf(ray,result,scatterRay) *  pdf ;


                      ;
            }

        }

        return background;
    }


    auto Raytracer::blinnPhongShading(const std::unique_ptr<Ray> & ray,const std::shared_ptr<CollisionResult> & result,RenderParameters *renderParameters)-> Color
    {

        std::shared_ptr<CollisionResult> c = result;

        //material scattered
        auto scattered = result->material->scatter(ray,result);

        //the material emitted color
        auto emitted = result->material->emitted(ray,result, std::pair<float,float> {result->u,result->v} );

        if(scattered == nullptr){
            return emitted;
        }

        if(!renderParameters->phongShadingOn){

            if(renderParameters->mapUVWToRGB && !renderParameters->texturedRendering ){
                return Color{c->u,c->v,0.f,1.f};
            }

            if(!renderParameters->texturedRendering)
            {
                  return Color{0.2f,0.2f,0.2f,1.f};
            }
            return scattered->attenuation;
        }

        //light postion
        auto pos = Cartesian3{0,1,0};
        //calculate the new pos with matrix;
        pos = renderParameters->lightMatrix * pos;
        //the direction of lighting
        auto lightDir = (pos - result->position).unit();


        //cos(sita) between light and normal
        auto cos = std::max(0.f,result->normal.dot(lightDir));
        //diffuse color
        auto diff = scattered->attenuation ;

        if(!renderParameters->texturedRendering)
        {
            if(renderParameters->mapUVWToRGB){
                diff = Color{c->u,c->v,0.f,1.f}* cos;
            }else{
                diff = Color{0.2f,0.2f,0.2f,1.f} * cos;
            }
        }

        auto viewDir = (camera->getOrigin() - result->position).unit();

        //blinn specular variation
        auto halfDir = (lightDir + viewDir).unit();

        //the specular
        auto specular = std::pow(std::max(0.f, result->normal.dot(halfDir)), renderParameters->specularExponent);

        auto specularLight = Color{
            renderParameters->specularLight,
            renderParameters->specularLight,
            renderParameters->specularLight,
            1.f
        };
        auto ambientLight = Color{
                renderParameters->ambientLight,
                renderParameters->ambientLight,
                renderParameters->ambientLight,
            1.f
        };


        return diff * renderParameters->diffuseLight + specularLight * specular + ambientLight;
    }


    //show the default scene
    //add a triangle mesh into scene with BlinnPhong
    auto Raytracer::showDefault() -> void
    {
        auto mesh = scene.addObject<TriangleMesh>(obj,std::makeShared<Lambertian>(std::makeShared<Texture2D>(texture)));

        //shading type
        mesh->setShadingType(ShadingType::BlinnPhong);


        camera = std::makeUnique<Camera>(
                    Cartesian3{0,0,2.5},
                    Cartesian3{0,0,0},
                    Cartesian3{0,1,0},
                    90,
                    4/3.f
                    );
        camera->recalculate();


        background = {0.8, 0.8, 0.6,1.f};

        SAMPLE_COUNT = 1;
        MAX_DEPTH = 1;
    }





    //simple lighting (just for one light)
    //
    auto Raytracer::simpleLighting(const std::unique_ptr<Ray> & rayIn,const std::shared_ptr<Mesh> & light,const std::shared_ptr<CollisionResult> & result,const std::unique_ptr<ScatterResult> & scatterResult,float & pdf) -> std::unique_ptr<Ray>
    {
       // if(light == nullptr){
            auto ray = std::makeUnique<Ray>(result-> position,scatterResult->pdf->generate(),rayIn->getTime());
            pdf = scatterResult->pdf->value(ray->getDirection());
            return std::move(ray);
     //   }


        //sample lighting.... there is a bug....
        /*auto lightPDF = std::makeShared<MeshPDF>(light,result->position);
        MixturePDF mixed(lightPDF,scatterResult->pdf);


        auto ray = std::makeUnique<Ray>(result-> position,mixed.generate(),rayIn->getTime());
        pdf = mixed.value(ray->getDirection());
        return std::move(ray);*/

    }


}
