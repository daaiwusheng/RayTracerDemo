#include "TriangleMesh.h"

#include "CollisionResult.h"
#include "Ray.h"
#include <cmath>

namespace ray
{

    TriangleMesh::TriangleMesh(const TexturedObject & texturObject,const std::shared_ptr<Material> & material)
        :Mesh(material),texturObject(texturObject)
    {

         box = {{INFINITY,INFINITY,INFINITY},{-INFINITY,-INFINITY,-INFINITY}};

         //calculate aabb box;
         for(auto &v : texturObject.vertices){
             for(int32_t i = 0;i<3;i++){
                 if (v[i] < box.getMin()[i]) box.getMin()[i] = v[i];
             }
             for(int32_t i = 0;i<3;i++){
                 if (v[i] > box.getMax()[i]) box.getMax()[i] = v[i];
             }
         }
    }

    auto TriangleMesh::update(RenderParameters *renderParameters) -> bool
    {
       /* Cartesian3 pos = transform.getTranslation();
        if(renderParameters->centreObject){
            auto scale = renderParameters->zoomScale;
            if(renderParameters->scaleObject && renderParameters->objectSize != 0){
                scale = renderParameters->zoomScale /  renderParameters->objectSize;
            }

            auto centreOfGravity = texturObject.centreOfGravity;
            pos += Cartesian3{ -centreOfGravity.x * scale, -centreOfGravity.y* scale , -centreOfGravity.z * scale};
        }
        pos+=Cartesian3{renderParameters->xTranslate,renderParameters->yTranslate,0};
        getTransform().setPosition(pos);*/
        return Mesh::update(renderParameters);
    }

    auto TriangleMesh::intersects(const std::unique_ptr<Ray> & ray, double minT,double maxT) -> std::shared_ptr<CollisionResult>
    {
        std::shared_ptr<CollisionResult> result;
        auto & trans = transform.getTransform();

        for(auto i = 0;i<texturObject.faceVertices.size() ;i++){
            auto & v = texturObject.faceVertices[i];

            auto v0 = trans * texturObject.vertices[v[0]];
            auto v1 = trans * texturObject.vertices[v[1]];
            auto v2 = trans * texturObject.vertices[v[2]];
            //get three vertices.

            auto r = intersectsWithTriangle(v0,v1,v2,ray,minT,maxT,i);
            if(result == nullptr){
                result = r;
            }

            if(r != nullptr){
                if(result != nullptr && r->t < result->t){
                     result = r;
                }
            }
        }
        return result;
    }


    auto TriangleMesh::intersectsWithTriangle(const Cartesian3& v0,const Cartesian3& v1,const Cartesian3& v2, const std::unique_ptr<Ray> & ray, double minT,double maxT,int32_t index) -> std::shared_ptr<CollisionResult>
    {
       auto v0v1 = v1 - v0;
       auto v0v2 = v2 - v0;
       auto pvec = ray->getDirection().cross(v0v2);
       float det = v0v1.dot(pvec);

       // ray and triangle are parallel if det is close to 0
       if (std::fabs(det) < 1e-8) return nullptr;

       float invDet = 1 / det;

       auto tvec = ray->getOrigin() - v0;
       auto u = tvec.dot(pvec) * invDet;
       if (u < 0 || u > 1) return nullptr;

       auto qvec = tvec.cross(v0v1);
       auto v = ray->getDirection().dot(qvec) * invDet;
       if (v < 0 || u + v > 1) return nullptr;

       auto t = v0v2.dot(qvec) * invDet;

       if(t < maxT && t > minT){
           auto normal = v0v1.cross(v0v2); // N
           auto r = assembleCollisionResult(t,ray,normal);

           auto & coords = texturObject.faceTexCoords[index];
            //get the three corrds in current face.
           auto t0 = texturObject.textureCoords[coords[0]];
           auto t1 = texturObject.textureCoords[coords[1]];
           auto t2 = texturObject.textureCoords[coords[2]];

            //barycentric interpolation
           auto coordinates = (1 - u - v) * t0 + u * t1 + v * t2;

           r->u = coordinates.x;
           r->v = coordinates.y;

           return r;
       }
       return nullptr;
    }

}

