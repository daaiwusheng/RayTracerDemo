#include "BVHTree.h"
#include "Mesh.h"
#include "CollisionResult.h"
#include "Ray.h"
#include "../Randomizer.h"
#include <cassert>
#include <algorithm>

namespace ray
{

   /**
     * accelaration structure for raytracing scene
     * @brief BVHNode::BVHNode  create a bvn node in bvh Tree.
     * @param meshes the all meshes
     * @param start  start position
     * @param end    end position
     */
    BVHNode::BVHNode(const std::vector<std::shared_ptr<Mesh>> & meshes,int32_t start,int32_t end)
    {
       //generate a random axis(x,y,z direction)
       auto axis = Randomizer::get().nextInt(0,3);

       assert(axis <= 2);


       auto numOfMeshes = end - start;

       if(numOfMeshes == 1)
       {
            left = meshes[start];
            right = meshes[start];
       }
       else if(numOfMeshes == 2)
       {
            left =  meshes[start];
            right = meshes[start + 1];

            if(left->getBoundingBox().getMin()[axis] < right->getBoundingBox().getMin()[axis]){
                left =  meshes[start + 1];
                right = meshes[start];
            }
       }
       else
       {
            auto sortable = meshes;
            //sort meshes between start and end by current axis.
            std::sort(sortable.begin() + start, sortable.begin() + end,[=](const std::shared_ptr<Mesh> & left, const std::shared_ptr<Mesh> & right){
                return left->getBoundingBox().getMin()[axis] < right->getBoundingBox().getMin()[axis];
            });

           auto mid = start + numOfMeshes / 2;
           //create the children nodes
           left = std::makeShared<BVHNode>(sortable,start,mid);
           right = std::makeShared<BVHNode>(sortable,mid,end);
       }

       box.merge(left->getBoundingBox());
       box.merge(right->getBoundingBox());

    }


    /**
     * @brief BVHNode::intersects with ray
     * @param ray current ray
     * @param minT current min Ray time
     * @param maxT current max Ray time
     * @return
     */
    auto BVHNode::intersects(const std::unique_ptr<Ray> & ray, double minT,double maxT) -> std::shared_ptr<CollisionResult>
    {
        if(!box.intersects(ray,minT,maxT)){
            return nullptr;
        }

        auto leftRet = left->intersects(ray,minT,maxT);
        auto rightRet = right->intersects(ray,minT,maxT);

        if(leftRet != nullptr && rightRet != nullptr)
        {
            if(leftRet->t < rightRet->t)
            {
                return leftRet;
            }
            return rightRet;
        }
        else if(leftRet)
        {
            return leftRet;
        }
        else if(rightRet)
        {
            return rightRet;
        }
        return nullptr;
    }


    BVHTree::BVHTree()
    {

    }


    auto BVHTree::buildTree(const std::vector<std::shared_ptr<Mesh>> & mesh) -> std::shared_ptr<BVHNode>
    {
        //create root node
        return root = std::makeShared<BVHNode>(mesh,0,mesh.size());
    }

    auto BVHTree::intersects(const std::unique_ptr<Ray> & ray, double minT,double maxT) -> std::shared_ptr<CollisionResult>
    {
        return root->intersects(ray,minT,maxT);
    }

}
