#include "Scene.h"
#include "CollisionResult.h"
#include "BVHTree.h"
#include "Plane.h"
#include "Material.h"
#include "Serialization.h"
#include <unordered_map>
#include "../Randomizer.h"
#include "../TexturedObject.h"
namespace ray
{

    Scene::Scene()
    {
        lights = std::makeShared<Lights>();
    }

    //intersects all objects.
    auto Scene::intersects(const std::unique_ptr<Ray> & ray, double minT,double maxT) -> std::shared_ptr<CollisionResult>
    {
        if(!box.intersects(ray,minT,maxT) || bvhTree == nullptr)
        {
            return nullptr;
        }
        return bvhTree->intersects(ray,minT,maxT);
    }

    //add a mesh in current scene
    auto Scene::addMesh(const std::shared_ptr<Mesh> & mesh) -> std::shared_ptr<Mesh>
    {
        box.merge(mesh->getBoundingBox());
        meshes.emplace_back(mesh);
        return mesh;
    }

    //update current scene with parameters
    auto Scene::update(RenderParameters *renderParameters) -> void
    {
        bool needRebuild = false;
        for(auto & mesh : meshes){
    //if positions are changed, the bvhTree should be rebuild
            if(mesh->update(renderParameters)){
                needRebuild = true;
            }
        }
        if(needRebuild && bvhTree)
            bvhTree->buildTree(meshes);
    }

    auto Scene::constructBVHTree() -> void
    {
         bvhTree = std::makeShared<BVHTree>();
         bvhTree->buildTree(meshes);
    }



    //serialization function
    auto operator >> (std::istream &inStream, ray::Scene &scene) -> std::istream &
    {
        while (true)
        {
            if (inStream.eof())
                break;

            std::string str;
            inStream >> str;
            std::shared_ptr<Mesh> mesh;
            if(str == "Plane")
            {
                mesh = scene.addMesh(Serialization::readPlane(inStream));
            }
            else if(str == "Cube")
            {
                mesh = scene.addMesh(Serialization::readCube(inStream));
            }
            else if(str == "Sphere")
            {
                mesh = scene.addMesh(Serialization::readSphere(inStream));
            }
            else if(str == "Triangle")
            {
                mesh = scene.addMesh(Serialization::readTriangle(inStream));
            }
            else if(str == "TriangleMesh")
            {
                mesh = scene.addMesh(Serialization::readTriangleMesh(inStream));
            }

            if(mesh != nullptr){
                auto material = mesh->getMaterial();
                if(material != nullptr &&  (material->getType() == Material::MaterialType::DiffuseLight
                                            ||
                                            material->getType() == Material::MaterialType::Dielectric
                                            ||
                                            material->getType() == Material::MaterialType::Metal
                                            ))
                {

                    scene.getLights()->addLight(mesh);
                }
            }

        }

        return inStream;
    }


    auto operator << (std::ostream &outStream, const ray::Scene &scene) -> std::ostream &
    {

        return outStream;
    }

}
