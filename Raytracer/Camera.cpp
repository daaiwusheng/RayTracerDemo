#include "Camera.h"
#include <cmath>
#include <algorithm>
#include "../Renderer/MathUtils.h"
#include "Ray.h"
#include "../MemoryTools.h"
namespace ray {

    /**
     * @brief Camera::Camera  constructor for camera
     * @param position   Camera position
     * @param lookAt     the object position
     * @param up         up vector
     * @param fov        filed of view
     * @param aspectRatio
     */
    Camera::Camera(const Cartesian3 & position, const Cartesian3 & lookAt, const Cartesian3 & up,
                   double fov,double aspectRatio):origin(position),look(lookAt),up(up),fov(fov),aspectRatio(aspectRatio)
    {
           recalculate();
    }


    auto Camera::setLeftCorner(const Cartesian3 & lc) -> void
    {
        leftCorner = lc;
    }
    auto Camera::setHorizontal(const Cartesian3 & h)-> void
    {
        horizontal = h;
    }
    auto Camera::setVertical(const Cartesian3 & v)-> void
    {
        vertical = v;
    }
    auto Camera::setOrigin(const Cartesian3 & o)-> void
    {
        origin = o;
    }

    auto Camera::lookAt( const Cartesian3 & lookAt) -> void
    {
        look = lookAt;
    }


    auto Camera::setAspectRatio(float aspectRatio) -> void
    {
        this->aspectRatio = aspectRatio;
    }


    /**
     * @brief Camera::recalculate  calculate the range for camera
     */
    auto Camera::recalculate() -> void
    {
        auto theta = MathUtils::radians(fov);
        auto h = tan(theta/2);
        float viewportHeight = 1;//2 * h;
        float viewportWidth = aspectRatio * viewportHeight;

        auto foward = (origin - look).unit();
        auto right = up.cross(foward).unit();
        auto cameraUp = foward.cross(right).unit();


        horizontal = right * viewportWidth;
        vertical = cameraUp * viewportHeight;

        leftCorner = origin - horizontal/2 - vertical/2 - foward;
    }

    /**
     * @brief Camera::castRay  send ray
     * @param u  pixel x
     * @param v  pixed y
     * @return
     */
    auto Camera::castRay(double u,double v) -> std::unique_ptr<Ray>{
        return std::makeUnique<Ray>(
           origin,
           leftCorner + u * horizontal + v * vertical - origin
        );
    }
};
