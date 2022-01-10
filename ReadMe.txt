compile :

module add qt/5.13.0
qmake RaytraceRenderWindow.pro
make -j8

or execute the compile_feng.sh to compile

Run:

./Raytrace ../path_to/model.obj ../path_to/texture.ppm ../Scene/current.scene (optional)

the last param is the scene file which exists Scene folder

if the file path didn't be passed to application, 
the application will show the default scene (load and show the model with texture like the left window)


for the scene file, the format is 


---the sample cout 
SAMPLE_COUNT 1000 (required)
---the max depth for the raytracer 
MAX_DEPTH 30  (required)
---Position(Vector3) LookAt(Vector) Up(Vector) fov(float) aspectRatio(float)
Camera 278 278 -600 278 278 0 0 1 0 40 1.333333  (required)
---background color(float)
BACKGROUND 0.0 0.0 0.0 1  (required)


---create scene objects format

Plane MinPos(float float) MaxPos(float float) K(float) PlaneType(XY YZ XZ) Material

Cube leftTop(Vector3) rightBottom(Vector3) Material Translation(Vector3) Rotation(Vector3) 

Sphere origin(Vector3) radius Material

Triangle v1(Vector3) v2(Vector3) v3(Vector3) uv1(Vector3) uv2(Vector3) uv3(Vector3) Translation(Vector3) Rotation(Vector3) Material

TriangleMesh file(String) Translation(Vector3) Rotation(Vector3) Material

---Material Objects

Lambertian Texture(Texture2D ColorTexture)

Metal Texture(Texture2D ColorTexture) fuzz(float)

DiffuseLight Texture(Texture2D ColorTexture) 

Dielectric ratio(float)

---Texture Objects

Texture2D file(String)

ColorTexture Color(float4)


the example scene files exists under the folder "Scene


Others:

 the code about Raytracing exists in Raytrace folder. For the project, I used openmp library to enable multi-thread to renderer,before running the project,
the enviroment should be installed the openmp library.

Thanks.













