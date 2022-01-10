#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>

#include "Mesh.h"
#include "../MemoryTools.h"
#include "BVHTree.h"
#include "Lights.h"

namespace ray {


    class Plane;

    class Scene
    {
    public:
        Scene();

        template<class T, typename... Args>
        auto addObject(Args&&...args) -> std::shared_ptr<T>;
        auto addMesh(const std::shared_ptr<Mesh> & mesh) -> std::shared_ptr<Mesh>;


        inline auto getMeshes() const -> const std::vector<std::shared_ptr<Mesh>>& { return meshes;}
        inline auto getMeshes() -> std::vector<std::shared_ptr<Mesh>>& { return meshes;}

        auto intersects(const std::unique_ptr<Ray> & ray, double minT,double maxT) -> std::shared_ptr<CollisionResult>;
        auto constructBVHTree() -> void;

        auto update(RenderParameters *renderParameters) -> void;

        auto getLights() ->   std::shared_ptr<Lights>& { return lights;}



    private:
        std::vector<std::shared_ptr<Mesh>> meshes;

        BoundingBox box;

        std::shared_ptr<BVHTree> bvhTree;

         std::shared_ptr<Lights> lights;

    };



    template<class T, typename... Args>
    inline auto Scene::addObject(Args&&...args) -> std::shared_ptr<T> {
        auto mesh = std::makeShared<T>(std::forward<Args>(args)...);
        box.merge(mesh->getBoundingBox());
        meshes.emplace_back(mesh);
        return mesh;
    }


    auto operator >> (std::istream &inStream, ray::Scene &scene) -> std::istream & ;
    auto operator << (std::ostream &outStream, const ray::Scene &scene) -> std::ostream &;




};
#endif // SCENE_H
