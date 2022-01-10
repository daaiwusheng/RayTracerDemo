#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <fstream>
#include "Scene.h"
#include "Plane.h"
#include "Cube.h"
#include "Material.h"
#include "Sphere.h"
#include "Triangle.h"
#include "TriangleMesh.h"

namespace Serialization
{

    auto readPlane(std::istream &inStream) -> std::shared_ptr<ray::Plane>;
    auto readSphere(std::istream &inStream) -> std::shared_ptr<ray::Sphere>;
    auto readCube(std::istream &inStream) -> std::shared_ptr<ray::Cube>;
    auto readTriangle(std::istream &inStream) -> std::shared_ptr<ray::Triangle>;
    auto readTriangleMesh(std::istream &inStream) -> std::shared_ptr<ray::TriangleMesh>;

    auto readMaterial(std::istream &inStream) -> std::shared_ptr<ray::Material>;

}

#endif // SERIALIZATION_H
