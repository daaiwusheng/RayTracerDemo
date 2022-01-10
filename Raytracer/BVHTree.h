#ifndef BVHTREE_H
#define BVHTREE_H

#include "../MemoryTools.h"
#include "Mesh.h"
#include <vector>


namespace ray
{


    class CollisionResult;
    class Ray;

    class BVHNode : public Mesh
    {
    public:
        BVHNode(const std::vector<std::shared_ptr<Mesh>> & meshes,int32_t start,int32_t end);
        auto intersects(const std::unique_ptr<Ray> & ray, double minT,double maxT) -> std::shared_ptr<CollisionResult>;
    private:
        BoundingBox box;
        std::shared_ptr<Mesh> left;
        std::shared_ptr<Mesh> right;
    };


    class BVHTree
    {
    public:
        BVHTree();
        auto buildTree(const std::vector<std::shared_ptr<Mesh>> & mesh) -> std::shared_ptr<BVHNode>;
        inline auto getRoot() -> std::shared_ptr<BVHNode> { return root;}
        inline auto getRoot() const -> const std::shared_ptr<BVHNode> { return root;}
        auto intersects(const std::unique_ptr<Ray> & ray, double minT,double maxT) -> std::shared_ptr<CollisionResult>;
    private:
        std::shared_ptr<BVHNode> root;
    };
}


#endif // BVHTREE_H
