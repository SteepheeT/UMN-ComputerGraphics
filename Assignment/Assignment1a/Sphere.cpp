#include "Sphere.h"


Sphere::Sphere() {
	origin = Point3(0, 0, 0);
	radius = 0;
	color[0] = 0;
	color[1] = 0;
	color[2] = 0;
}

Sphere::Sphere(float x_, float y_, float z_, float r_) {
	origin = Point3(x_, y_, z_);
	radius = r_;
	color[0] = 0;
	color[1] = 0;
	color[2] = 0;
}

Sphere::~Sphere() {

}

void Sphere::SetOrigin(int x_, int y_, int z_) {
	origin = Point3(x_, y_, z_);
}

void Sphere::SetRaidus(float r_) {
	radius = r_;
}

void Sphere::SetColor(Color c) {
	color = c;
}

std::ostream& operator<< (std::ostream& os, const Sphere& p) {
	return os << p.origin << " " << p.radius;
}

std::istream& operator>> (std::istream& is, Sphere& p) {
	char dummy;
	Point3 origin;
	return is >> dummy >> origin >> dummy >> p.radius;
}