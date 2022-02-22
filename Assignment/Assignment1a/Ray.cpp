#include "Ray.h"
#include "Input_processor.h"

Ray::Ray() {
	origin_ = Point3(0, 0, 0);
	dir_ = Vector3(0, 0, -1);
}

Ray::Ray(const Point3 origin, Vector3 dir) {
	origin_ = origin;
	if (dir.Length() != 1) {
		dir.Normalize();
	}
	dir_ = dir;
}

Ray::~Ray() {
}

bool Ray::operator==(const Ray& other) const {
    return (origin_ == other.origin()) && (dir_ == other.direction());
}


bool Ray::operator!=(const Ray& other) const {
    return (origin_ != other.origin()) || (dir_ != other.direction());
}

float Ray::Length() const {
	return dir_.Length();
}

Point3 Ray::origin() const {
    return origin_;
}


Vector3 Ray::direction() const {
    return dir_;
}

Color Ray::RayTrace(InputProcessor& ipp, float std_t) {
	std::vector<Sphere> sphere_vector = ipp.GetSphereVector();
	float min_time = -1;
	int front_obj_idx = -1;
	for (int k = 0; k < sphere_vector.size(); k++) {
		float time = 0.0f;
		Point3 iPoint = Point3(0, 0, 0);
		Sphere sphere_ = sphere_vector.at(k);

		if (IntersectSphere(sphere_.GetOrigin(), sphere_.GetRadius(), &time, &iPoint)) {
			// The object will be displayed only if there are at the back of the viewing window
			if (time >= std_t) {
				if (min_time == -1) {
					min_time = time;
					front_obj_idx = k;
				}
				if (time < min_time && k > 0) {
					min_time = time;
					front_obj_idx = k;
				}
			}
		}
	}
	
	if (front_obj_idx == -1) {
		return ipp.GetBgColor();
	}

	return RayShade(ipp, front_obj_idx);
}

Color Ray::RayShade(InputProcessor& ipp, int obj_idx) {
	return ipp.GetSphereVector().at(obj_idx).GetColor();
}

bool Ray::IntersectSphere(const Point3& center, float radius, float* scale_t, Point3* i_point) const {
	const double A = dir_[0] * dir_[0] + dir_[1] * dir_[1] + dir_[2] * dir_[2];

	const double B = 2 * (dir_[0] * (origin_[0] - center[0])
		+ dir_[1] * (origin_[1] - center[1])
		+ dir_[2] * (origin_[2] - center[2]));

	const double C = std::pow((origin_[0] - center[0]), 2)
		+ std::pow((origin_[1] - center[1]), 2)
		+ std::pow((origin_[2] - center[2]), 2)
		- std::pow(radius, 2);

	double discriminant = std::pow(B, 2) - 4 * A * C;

	float insertion_time = 0.0f;

	if (discriminant < 0.0) {
		return false;
	}
	else {
		// got one or two interceotion with the sphere
		double discRoot = std::sqrt(discriminant);
		double t1 = (-B - discRoot) / 2 * A;
		double t2 = (-B + discRoot) / 2 * A;
		bool hit1 = false;
		bool hit2 = false;
		if (t1 > 0) {
			hit1 = true;
		}
		if (t2 > 0) {
			hit2 = true;
		}
		if (hit1 && hit2) {
			if (t1 == t2) {
				*scale_t = (float)t1;
			}
			else if (t1 < t2) {
				*scale_t = (float)t1;

			}
			else {
				*scale_t = (float)t2;
			}
		}
		*i_point = origin_ + (*scale_t) * dir_;
		return true;
	}
}

void Ray::set(Point3 newOrigin, Vector3 newDir) {
	origin_ = newOrigin;
	dir_ = newDir;
}