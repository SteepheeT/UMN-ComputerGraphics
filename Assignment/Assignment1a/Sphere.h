#ifndef SPHERE_H_
#define SPHERE_H_

#include "Point3.h"
#include "Color.h"

class Sphere {
public:
	Sphere();
	Sphere(float x_, float y_, float z_, float r_);
	~Sphere();

	void SetOrigin(int x_, int y_, int z_);
	void SetRaidus(float r_);
	void SetColor(Color c);

	Point3 GetOrigin() const { return origin; }
	float GetRadius() const { return radius; }
	Color GetColor() const { return color; }

	friend std::ostream& operator<< (std::ostream& os, const Sphere& p);
	friend std::istream& operator>> (std::istream& is, Sphere& p);

protected:
	Point3 origin;
	float radius;
	Color color;
};

#endif // !SPHERE_H_

