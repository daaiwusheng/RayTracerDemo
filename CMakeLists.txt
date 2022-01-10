cmake_minimum_required(VERSION 2.8)
project(RaytraceRenderWindow)

set(CMAKE_CXX_STANDARD 11)

# 添加头文件
set(GLEW_H /usr/local/Cellar/glew/2.2.0_1/include/GL)
set(GLFW_H /usr/local/Cellar/glfw/3.3.4/include/GLFW)
set(GLAD_H /usr/local/include/)
set(GLAD_H /usr/local/include/glad)
set(KH_H /usr/local/include/KHR)
#包含上述头文件所在目录
include_directories(${GLEW_H} ${GLFW_H} ${GLAD_H} ${KH_H})

# 添加目标链接
set(GLEW_LINK /usr/local/Cellar/glew/2.2.0_1/lib/libGLEW.2.2.0.dylib)
set(GLFW_LINK /usr/local/Cellar/glfw/3.3.4/lib/libglfw.3.3.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)



add_executable(RaytraceRenderWindow main.cpp
        ArcBall.cpp
        ArcBall.h
        ArcBallWidget.cpp
        ArcBallWidget.h
        BoundingBox.cpp
        BoundingBox.h
        Cartesian3.cpp
        Cartesian3.h
        Homogeneous4.cpp
        Homogeneous4.h
        Matrix4.cpp
        Matrix4.h
        MemoryTools.cpp
        MemoryTools.h
        Quaternion.cpp
        Quaternion.h
        Randomizer.h
        RaytraceRenderWidget.cpp
        RaytraceRenderWidget.h
        RenderController.cpp
        RenderController.h
        RenderParameters.h
        RenderWidget.cpp
        RenderWidget.h
        RenderWindow.cpp
        RenderWindow.h
        RGBAImage.cpp
        RGBAImage.h
        RGBAValue.cpp
        RGBAValue.h
        TexturedObject.cpp
        TexturedObject.h
        Timer.h
        Transform.cpp
        Transform.h
        src/glad.c
        Raytracer/BVHTree.cpp
        Raytracer/BVHTree.h
        Raytracer/Camera.cpp
        Raytracer/Camera.h
        Raytracer/CollisionResult.cpp
        Raytracer/CollisionResult.h
        Raytracer/ColorTexture.cpp
        Raytracer/ColorTexture.h
        Raytracer/Cube.cpp
        Raytracer/Cube.h
        Raytracer/DiffuseLight.cpp
        Raytracer/DiffuseLight.h
        Raytracer/Lights.cpp
        Raytracer/Lights.h
        Raytracer/Material.cpp
        Raytracer/Material.h
        Raytracer/Mesh.cpp
        Raytracer/Mesh.h
        Raytracer/OBN.cpp
        Raytracer/OBN.h
        Raytracer/PDF.cpp
        Raytracer/PDF.h
        Raytracer/Plane.cpp
        Raytracer/Plane.h
        Raytracer/Ray.cpp
        Raytracer/Ray.h
        Raytracer/Raytracer.cpp
        Raytracer/Raytracer.h
        Raytracer/Scene.cpp
        Raytracer/Scene.h
        Raytracer/Serialization.cpp
        Raytracer/Serialization.h
        Raytracer/Sphere.cpp
        Raytracer/Sphere.h
        Raytracer/TexturePool.cpp
        Raytracer/TexturePool.h
        Raytracer/Triangle.cpp
        Raytracer/Triangle.h
        Raytracer/TriangleMesh.cpp
        Raytracer/Triangle.h
        Renderer/Color.cpp
        Renderer/Color.h
        Renderer/MathUtils.h
        Renderer/RendererThread.cpp
        Renderer/RendererThread.h
        Renderer/Texture2D.cpp
        Renderer/Texture2D.h
        )



find_package(Qt5 COMPONENTS
        Core
        REQUIRED)

find_package(Qt5Widgets REQUIRED)

# mac下这步很重要
if (APPLE)
    target_link_libraries(RaytraceRenderWindow "-framework OpenGL")
    target_link_libraries(RaytraceRenderWindow "-framework GLUT")
endif()

target_link_libraries(RaytraceRenderWindow
        Qt5::Core
        )
target_link_libraries(RaytraceRenderWindow
        Qt5::Widgets
        )

target_link_libraries(RaytraceRenderWindow
        Qt5::Gui
        )





