
#include "Transform.h"


Transform::Transform()
{

    localTransform.SetIdentity();
    scale.SetIdentity();
    parentTransform.SetIdentity();
    transform.SetIdentity();
    inverseTransform.SetIdentity();
    rotation.SetIdentity();

}


auto Transform::setScale(float x, float y, float z) -> void
{
    scale.SetScale(x,y,z);
    updateTransformDirty();//set as dirty, in the next update ,reculate the transformation
}

auto Transform::getScale()  -> const Cartesian3
{
    return scale.getScale();
}

auto Transform::setRotation(float x, float y, float z) -> void
{
    setEulerAngles({x,y,z});
}

auto Transform::setRotation(const Matrix4 & transform) -> void
{
    //angle = transform;
    rotationMatrix = transform;
    updateTransformDirty();
}

auto Transform::setQuaternion(const Quaternion & q) -> void{
    angle = q;
    updateTransformDirty();
}

auto Transform::setEulerAngles(const Cartesian3& a) -> void
{
    angle.setEulerAngles(a);

    updateTransformDirty();
}

auto Transform::setPosition(const Cartesian3& pos) -> void
{
    position = pos;
    updateTransformDirty();
}
auto Transform::setParentTransform(const Matrix4 & transform) -> void
{
    parentTransform = transform;
    updateTransformDirty();
}

auto Transform::calculateLocalTransform()->const Matrix4&
{
    localTransform.SetTranslation(position);
    localTransform*= angle.GetMatrix();
    localTransform*= rotationMatrix;
    localTransform*= scale;

    return localTransform;
}

auto Transform::calculateTransform() -> void
{
    transform = localTransform * getParentTransform();

    inverseTransform = transform.inverse();
}

auto Transform::getParentTransform()->const Matrix4&
{
    return parentTransform;
}


auto Transform::getTranslation() -> const Cartesian3
{
    return transform.getTranslation();
}

auto Transform::updateTransformDirty() -> void
{
    transformDirty = true;
}

auto Transform::update(RenderParameters * param) -> bool
{
    auto s = param->zoomScale;
    if(param->scaleObject && param->objectSize != 0){
        s = param->zoomScale /  param->objectSize;
    }

    auto sc = Cartesian3{s,s,s};
    if(scale.getScale() == sc){

    }else{
        setScale(s,s,s);
    }

    if(param->rotationMatrix != rotationMatrix){
        setRotation(param->rotationMatrix);
    }


    auto ret = transformDirty;
    if(transformDirty)
    {
        calculateLocalTransform();
        calculateTransform();
        transformDirty = false;
    }
    return ret;
}
