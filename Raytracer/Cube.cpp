#include "Cube.h"
#include "Plane.h"
#include "BVHTree.h"
#include "../MemoryTools.h"
#include "CollisionResult.h"
#include <utility>

namespace ray
{
    /**
     * @brief Cube::Cube create a cube
     * @param lt  the left top postion
     * @param rb  the right bottom position
     * @param material  the material of current cube
     */
    Cube::Cube(const Cartesian3 & lt, const Cartesian3 & rb, const std::shared_ptr<Material> & material)
        :Mesh(material),leftTop(lt),rightBottom(rb)
    {
        box = {lt,rb};

        //current 8 faces
        slides.emplace_back(std::makeShared<Plane>(std::pair<float,float>(lt.x,lt.y),std::pair<float,float>(rb.x,rb.y),rb.z,PlaneType::XY_PLANE,material));
        slides.emplace_back(std::makeShared<Plane>(std::pair<float,float>(lt.x,lt.y),std::pair<float,float>(rb.x,rb.y),lt.z,PlaneType::XY_PLANE,material));

        slides.emplace_back(std::makeShared<Plane>(std::pair<float,float>(lt.x,lt.z),std::pair<float,float>(rb.x,rb.z),rb.y,PlaneType::XZ_PLANE,material));
        slides.emplace_back(std::makeShared<Plane>(std::pair<float,float>(lt.x,lt.z),std::pair<float,float>(rb.x,rb.z),lt.y,PlaneType::XZ_PLANE,material));

        slides.emplace_back(std::makeShared<Plane>(std::pair<float,float>(lt.y,lt.z),std::pair<float,float>(rb.y,rb.z),rb.x,PlaneType::YZ_PLANE,material));
        slides.emplace_back(std::makeShared<Plane>(std::pair<float,float>(lt.y,lt.z),std::pair<float,float>(rb.y,rb.z),lt.x,PlaneType::YZ_PLANE,material));

        //create a tree for cube
        tree = std::makeShared<BVHTree>();
        tree->buildTree(slides);

    }

    //intersects with ray
    auto Cube::intersects(const std::unique_ptr<Ray> & ray, double minT,double maxT) -> std::shared_ptr<CollisionResult>
    {
        //calculate the pos with transform
        auto angle = transform.getAngle().getEulerAngles();
        //set back to original position and angles.
        Quaternion q;
        q.setEulerAngles(angle * -1);
        auto origin = ray->getOrigin() - transform.getTranslation();
        auto direction = ray->getDirection();
        origin = q.GetMatrix() * origin;
        direction = q.GetMatrix()  * direction;
        //and calculate the inersection result
        auto newRay = std::makeUnique<Ray>(origin,direction,ray->getTime());

        auto ret =  tree->intersects(newRay,minT,maxT);

        if(ret != nullptr){
            //set to new position and angles.
            auto q = transform.getAngle();
            ret->position = newRay->getPosition(ret->t);
            ret->position = q.GetMatrix() * ret->position;
            ret->position = ret->position + transform.getTranslation();
            auto n = q.GetMatrix() * ret->normal;
            ret->setFaceNormal(newRay,n);
        }
        return ret;
    }

}
