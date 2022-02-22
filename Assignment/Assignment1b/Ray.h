#ifndef RAY_H_
#define RAY_H_

#include <iostream>
#include "Point3.h"
#include "Color.h"
#include "Vector3.h"
#include "Input_processor.h"

class Ray {
public:

    /// Defaults to a ray at the origin and pointing in the -Z direction
    Ray();

    /// Creates a ray from a 3D origin and direction
    Ray(const Point3 origin, Vector3 direction);

    /// Ray destructor
    virtual ~Ray();

    bool operator==(const Ray& other) const;

    bool operator!=(const Ray& other) const;

    /// Returns the length of the direction vector
    float Length() const;

    /// Returns the origin
    Point3 origin() const;

    /// Returns the direction
    Vector3 direction() const;

    /// <summary>
    /// Checks to see if the ray intersects a sphere. If there was an intersection, 
    /// return true, *scale_t is the scaler of the direction vector of the ray, and 
    /// *i_point is the intersection point.
    /// </summary>
    /// 
    /// <param name="center">Sphere center Point3 variable</param>
    /// <param name="radius">Sphere radius</param>
    /// <param name="scale_t">Ray direction vector multiplier</param>
    /// <param name="i_point">Ray-sphere insertion point</param>
    /// <returns>true if the ray has intersection with the sphere, otherwise false</returns>
    bool IntersectSphere(const Point3& center, float radius,
        float* scale_t, Point3* i_point) const;

    /// <summary>
    /// Ray Tracing funciton, looping through each object in the scene. Detecting if
    /// the ray has any intersection point with the obj, find the object at the front,
    /// based on its index, call RayShade function and get the color of that object.
    /// </summary>
    /// <param name="ipp">input manager obj</param>
    /// <returns>the most front object's color.</returns>
    Color RayTrace(InputProcessor& ipp, float std_t);

    /// <summary>
    /// Based on the index, find the corresponding object color.
    /// </summary>
    /// <param name="ipp">input manager obj</param>
    /// <param name="obj_idx">index of the object</param>
    /// <returns>object color.</returns>
    Color RayShade(InputProcessor& ipp, int obj_idx, Point3 iPoint);

    /// Sets a new origin and direction
    void set(Point3 newOrigin, Vector3 newDir);

private:
    Point3 origin_;
    Vector3 dir_;



};

#endif // !RAY_H_


