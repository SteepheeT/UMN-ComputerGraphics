# CSCI 5607 RayTracer Readme File
#### This is a readme file created by Junyue Tao about the ray-tracing program
## Files and Build
### Build
The provided file includes a CMake file named CMakeList.txt.

Recommended CMake version on Windows is v3.13 or higher, as new features of CMake related to Visual Studio are released on that version. The version below 3.13 may lead to issues like being unable to set read-in file names using CMake stript.

To build on Windows or Ubuntu, first, build using
```
cmake .
```
Then CMake generator will auto-generate VS solution files on windows or Makefile on Ubuntu.

On Ubuntu, continue to execute the following command inside the terminal
```
make .
```
It will generate an executable file called raytracer. Use
```
./raytracer <input_setting_file_name> <output_file_name>
```
to execute the program.

The input file name is necessary to successfully execute the program, and the output file name is optional, if not provided, the default name will be used.

### File content
The program is written in c++ and it depends on variety of files including "Vector3", "Point3", "Ray", etc. Files are built for later convenience of future improvement and further development. Following are a glance at some files:
#### Vector3.h
```
class Vector3 {
public:
   Vector3();
   Vector3(float x, float y, float z);
   Vector3(float* v);
   Vector3(const Vector3& v);
   virtual ~Vector3();
  
   bool operator==(const Vector3& v) const;
   bool operator!=(const Vector3& v) const;
   Vector3& operator=(const Vector3& v);
   float operator[](const int i) const;
   float& operator[](const int i);

   float Dot(const Vector3& v) const;
   Vector3 Cross(const Vector3& v) const;
   float Length() const;
   void Normalize();
   Vector3 ToUnit() const;

   const Vector3& ZeroVector();
   const Vector3& OneVector();
   const Vector3& UnitVectorX();
   const  Vector3& UnitVectorY();
   const Vector3& UnitVectorZ();
   static Vector3 Normalize(const Vector3& v);
   static Vector3 Cross(const Vector3& v1, const Vector3& v2);
   static float Dot(const Vector3& v1, const Vector3& v2);
private:
   float v[3];
};
```
#### Point3.h
```
class Point3 {
public:
   Point3();
   Point3(float x, float y, float z);
   Point3(float* p);
   Point3(const Point3& p);
   virtual ~Point3();

   bool operator==(const Point3& p) const;
   bool operator!=(const Point3& p) const;
   Point3& operator=(const Point3& p);
   float operator[](const int i) const;
   float& operator[](const int i);

   const Point3& Origin();
   const Point3& Zero();
   const  Point3& One();
private:
   float p[3];
};
```
#### Ray.h
```
class Ray {
public:
   Ray();
   Ray(const Point3 origin, Vector3 direction);
   virtual ~Ray();
  
   bool operator==(const Ray& other) const;
   bool operator!=(const Ray& other) const;

   float Length() const;
   Point3 origin() const;
   Vector3 direction() const;
  
   bool IntersectSphere(const Point3& center, float radius,
       float* scale_t, Point3* i_point) const;

   Color RayTrace(InputProcessor& ipp, float std_t);
   Color RayShade(InputProcessor& ipp, int obj_idx);
private:
   Point3 origin_;
   Vector3 dir_;
};
```
#### Color.h
```
class Color {
public:
   Color();
   Color(float red, float green, float blue);
   Color(float* p);
   Color(const std::vector<float>& vals);
   Color(const Color& p);
   virtual ~Color();

   bool operator==(const Color& p) const;
   bool operator!=(const Color& p) const;
   Color& operator=(const Color& p);
   float operator[](const int i) const;
   float& operator[](const int i);

   std::vector<float> ToVector() const;
private:
   float c[3];
};
```

## Functionality of parameters
### The "up" direction
The up direction, or vector, determines the rotation angle of the camera. The up direction does not necessarily be the y-axis, and it also dies not necessarily be perpendicular to the camera's viewing direction, or the viewing vector. It contains the rotation information of the camera. For example:

Assume we have a camera located at <0, 0, 0> and facing <1, 0, 0>, also with up: <0, 1, 0>. By the above setting, we'll know that the u direction is <0, 0, 1>, v direction is also <0, 1, 0>. However, the value of up vector also can be <0.5, 0.5, 0>, and the result image will remain the same.

In this case, even the angle between the viewing direction and the up vector is less than 90 degrees, it does not mean that the camera is looking down. Yaw and pitch will be determined by the viewing direction, and as for the up vector, it determines where the camera is rotating. For example, if we change up vector to <0, 0, 1>, then it means the camera is rotated along the x-axis for 90 degrees.

### vfov
The vertical field of view describes the vertical dimensions of the measurement field in the object plane. In general, a larger VFOV will have a larger scene, but distortion will also get worsen. Thus, VFOV around 30 - 60 is better. See difference from 170vfov.ppm and 60vfov.ppm

### fov and viewing distance
The viewing distance will not have any effect on the final image, the size and distortion effect will only be affected by VFOV parameters.
However, you might need to know that too large a viewing distance might lead to the object in the 3D world being placed between the camera and the viewing window. Thus, always adjust object coordinates after adjusting the viewing distance. This is why the submit_case.ppm looks like this. 

