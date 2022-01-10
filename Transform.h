
#pragma once

#include "Matrix4.h"
#include "Quaternion.h"
#include "Cartesian3.h"
#include "RenderParameters.h"

class Transform
{
public:
    Transform();
    auto setScale(float x, float y, float z = 0) -> void;
    auto getScale()  -> const Cartesian3;
    auto setRotation(float x, float y, float z = 0) -> void;
    auto setRotation(const Matrix4 & transform) -> void;
    auto setQuaternion(const Quaternion & ) -> void;
    auto setEulerAngles(const Cartesian3& angle) -> void;
    auto setPosition(const Cartesian3& angle) -> void;
    auto setParentTransform(const Matrix4 & transform) -> void;

    auto calculateLocalTransform()->const Matrix4&;
    auto calculateTransform() -> void;
    auto getParentTransform()->const Matrix4&;

    auto getTranslation() -> const Cartesian3;
    inline auto getInverseTransform() const -> const Matrix4& { return inverseTransform; }
    inline auto getTransform() const -> const Matrix4& { return transform; }
    inline auto getTransform()  ->  Matrix4& { return transform; }

    auto updateTransformDirty() -> void;
    auto update(RenderParameters * param) -> bool;

    inline auto getAngle() const  -> const Quaternion&{ return angle;}
    inline auto getAngle()   ->  Quaternion&{ return angle;}

private:
    Matrix4 scale;
    Matrix4 parentTransform;
    Matrix4 rayParentTransform;

    Matrix4 localTransform;
    Cartesian3 position;
    Quaternion angle;

    Matrix4 transform ;

    Matrix4 rotationMatrix;


    Matrix4 inverseTransform;
    bool transformDirty = false;

    Matrix4 rotation;


};
