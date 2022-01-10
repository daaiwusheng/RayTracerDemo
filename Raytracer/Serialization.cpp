#include "Serialization.h"
#include "DiffuseLight.h"
#include "../Renderer/MathUtils.h"
#include "../Renderer/Texture2D.h"
#include "TexturePool.h"
#include <unordered_map>
#include <utility>
#include <cassert>



//Serialization functions
namespace Serialization
{
    auto readMaterial(std::istream &inStream) -> std::shared_ptr<ray::Material>
    {
        std::string type;
        inStream >> type;
        std::shared_ptr<ray::Material> material;


        if(type == "Lambertian" || type == "DiffuseLight" || type == "Metal"){
            std::string textureType;
            inStream >> textureType;//texture type;
            std::shared_ptr<Texture> texture;
            if(textureType == "ColorTexture")
            {
                float r,g,b,a;
                inStream >> r >> g >> b >>a;
                texture = std::makeShared<ray::ColorTexture>(Color{r,g,b,a});
            }
            else if(textureType == "Texture2D")
            {
                std::string file;
                inStream >> file;
                texture = std::makeShared<Texture2D>(TexturePool::get().load(file));
            }
            else
            {
                std::cout<<"warring : unknown texture type "<<textureType <<std::endl;
            }

            if(type == "Lambertian")
                material = std::makeShared<ray::Lambertian>(texture);
            if(type == "DiffuseLight")
                material = std::makeShared<ray::DiffuseLight>(texture);
            if(type == "Metal"){
                float fuzz = 0;
                inStream >> fuzz;
                material = std::makeShared<ray::Metal>(texture,fuzz);
            }

        }
        else if(type == "Dielectric")
        {
            float k ;
            inStream >> k;
            material = std::makeShared<ray::Dielectric>(k);
        }


        assert(material != nullptr);

        return material;

    }

    auto readPlane(std::istream &inStream) -> std::shared_ptr<ray::Plane>
    {
        float minA,minB,maxA,maxB,k;
        inStream >> minA >> minB >> maxA >> maxB >> k;
        std::string type;
        inStream >> type;

        static std::unordered_map<std::string,ray::PlaneType> types = {
            {"YZ",ray::PlaneType::YZ_PLANE},
            {"XY",ray::PlaneType::XY_PLANE},
            {"XZ",ray::PlaneType::XZ_PLANE}
        };
        auto material = readMaterial(inStream);

        return std::makeShared<ray::Plane>(std::pair<float,float>{minA,minB},std::pair<float,float>{maxA,maxB},k,types[type],material);
    }

    auto readSphere(std::istream &inStream) -> std::shared_ptr<ray::Sphere>
    {
        Cartesian3 position;
        inStream >> position;
        float radius;
        inStream >> radius;

        auto material = readMaterial(inStream);

        return std::makeShared<ray::Sphere>(position,radius,material);
    }


    auto readCube(std::istream &inStream) -> std::shared_ptr<ray::Cube>
    {

        Cartesian3 lt,rb;
        inStream >> lt >> rb;
        auto material = readMaterial(inStream);
        auto cube = std::makeShared<ray::Cube>(lt,rb,material);


        Cartesian3 rotation,translation;
        inStream >> rotation >> translation;

        cube->getTransform().setRotation(
                MathUtils::radians(rotation.x),
                MathUtils::radians(rotation.y),
                MathUtils::radians(rotation.z)
        );

        cube->getTransform().setPosition(translation);

        return cube;

    }

    auto readTriangleMesh(std::istream &inStream) -> std::shared_ptr<ray::TriangleMesh>
    {
        std::string file;

        Cartesian3 translation,rotation;
        inStream >>file>>translation>>rotation;


        TexturedObject obj;
        std::ifstream in1(file);
        std::ifstream in2;
        obj.ReadObjectStream(in1,in2);

        in1.close();
        in2.close();

        auto material = readMaterial(inStream);


        auto triangle = std::makeShared<ray::TriangleMesh>(obj,material);


        triangle->getTransform().setRotation(
                MathUtils::radians(rotation.x),
                MathUtils::radians(rotation.y),
                MathUtils::radians(rotation.z)
        );
        triangle->getTransform().setPosition(translation);
        triangle->setShadingType(ray::ShadingType::BlinnPhong);

        return triangle;
    }

    auto readTriangle(std::istream &inStream) -> std::shared_ptr<ray::Triangle>
    {
        Cartesian3 v1,v2,v3,uv1,uv2,uv3,translation,rotation;
        inStream >> v1 >> v2 >> v3 >> uv1>>uv2>>uv3>>translation>>rotation;
        auto material = readMaterial(inStream);
        auto triangle = std::makeShared<ray::Triangle>(v1,v2,v3,material);
        triangle->setTexCoord(uv1,uv2,uv3);

        triangle->getTransform().setRotation(
                MathUtils::radians(rotation.x),
                MathUtils::radians(rotation.y),
                MathUtils::radians(rotation.z)
        );
        triangle->getTransform().setPosition(translation);
        triangle->setShadingType(ray::ShadingType::BlinnPhong);

        return triangle;
    }


}
