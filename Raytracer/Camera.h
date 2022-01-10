#ifndef CAMERA_H
#define CAMERA_H

#include "../Cartesian3.h"
#include <memory>

namespace ray{
    class Ray;

    class Camera
    {
    public:
        Camera() = default;
        Camera( const Cartesian3 & position, const Cartesian3 & lookAt, const Cartesian3 & up,
                double fov,double aspectRatio);

        inline auto getLeftCorner() const -> const Cartesian3 & { return leftCorner;}
        inline auto getHorizontal() const -> const Cartesian3 & { return horizontal;}
        inline auto getVertical() const -> const Cartesian3 & { return vertical;}
        inline auto getOrigin() const -> const Cartesian3 & { return origin;}


        auto setLeftCorner(const Cartesian3 & lc) -> void;
        auto setHorizontal(const Cartesian3 & h)-> void;
        auto setVertical(const Cartesian3 & v)-> void;
        auto setOrigin(const Cartesian3 & origin)-> void;


        auto lookAt( const Cartesian3 & lookAt) -> void;

        auto castRay(double u,double v) -> std::unique_ptr<Ray>;

        auto recalculate() -> void;

        auto setAspectRatio(float aspectRatio) -> void;

    private:


        Cartesian3 leftCorner;
        Cartesian3 horizontal;
        Cartesian3 vertical;
        Cartesian3 origin;
        Cartesian3 look;
        Cartesian3 up;

        float fov;
        float aspectRatio;
    };
}


#endif // CAMERA_H
