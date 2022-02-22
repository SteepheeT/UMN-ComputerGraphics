#include "Ray.h"
#include <cmath>
#include <random>
#include "Input_processor.h"
#include "ctime"

int ClosestSphere(Point3& pt, std::vector<Sphere>& sphere_arr) {
	int idx = -1;
	for (int i = 0; i < sphere_arr.size(); i++) {
		Sphere sphere_ = sphere_arr.at(i);
		float d = (sphere_.GetOrigin() - pt).Length();
		if (std::abs(sphere_.GetRadius() - d) < MATH_DIV_OFFSET) {
			idx = i;
		}
	}
	return idx;
}

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
	Point3 front_intersect_pt = Point3(0, 0, 0);
	for (int k = 0; k < sphere_vector.size(); k++) {
		float time = 0.0f;
		Point3 iPoint = Point3(0, 0, 0);
		Sphere sphere_ = sphere_vector.at(k);
		if (IntersectSphere(sphere_.GetOrigin(), sphere_.GetRadius(), &time, &iPoint)) {
			// The object will be displayed only if there are at the back of the viewing window
			//if (time >= std_t) {
				if (min_time == -1) {
					min_time = time;
					front_obj_idx = k;
					front_intersect_pt = iPoint;
				}
				if (time < min_time && k > 0) {
					min_time = time;
					front_obj_idx = k;
					front_intersect_pt = iPoint;
				}
			//}
		}
	}

	// Ray does not intersec with any sphere, return backgound color
	if (front_obj_idx == -1) {
		Color c = ipp.GetBgColor();
		c[0] = c.red() * 255;
		c[1] = c.green() * 255;
		c[2] = c.blue() * 255;
		return c;
	}

	return RayShade(ipp, front_obj_idx, front_intersect_pt);
}

Color Ray::RayShade(InputProcessor& ipp, int obj_idx, Point3 iPoint) {
	std::vector<Sphere> sphere_vec = ipp.GetSphereVector();
	std::vector<LightSource> light_arr = ipp.GetLightVector();
	Sphere sphere_ = sphere_vec.at(obj_idx);
	int light_size = light_arr.size();
	float ka = ipp.GetKaMultiplier(), kd = ipp.GetKdMultiplier(), ks = ipp.GetKsMultiplier();
	Color diffuse_color_ = sphere_.GetMtColor(), specu_color_ = sphere_.GetSpColor();

	float light_intense = 1.0f / light_arr.size();

	Vector3 V = ipp.GetEyePos() - iPoint;
	V.Normalize();
	Vector3 L;
	Vector3 H;

	float Ir1 = ka * diffuse_color_.red();
	float Ig1 = ka * diffuse_color_.green();
	float Ib1 = ka * diffuse_color_.blue();

	float Ir2 = 0, Ig2 = 0, Ib2 = 0;

	float shadow_flag;
	for (int i = 0; i < light_size; i++) {
		LightSource light_ = light_arr.at(i);
		shadow_flag = 1.0;
		
		// implement light attenuation
		float att_factor = 1;
		if (ipp.GetFlagLightAtt() == 1) {
			if (light_.GetTypeMark() == 1 && light_.GetAttVar()[0] != -1) {
				// std::cout << "calculate att light: using " << light_.GetTypeMark() << std::endl;
				float* att = light_.GetAttVar();
				Vector3 dis = light_.GetPointLtOrigin() - iPoint;
				float len = dis.Length();
				att_factor = 1.0f / (att[0] + att[1] * len + att[2] * len * len);
			}
		}

		if (light_.GetW() == 1) {
			L = light_.GetPointLtOrigin() - iPoint;
		}
		else if (light_.GetW() == 0) {
			L = -light_.GetDirLtDirection();
		}
		L.Normalize();
		H = (V + L) / 2;
		H.Normalize();
		Vector3 N = iPoint - sphere_.GetOrigin();
		N.Normalize();
		
		// calculating shadows
		Ray shadow_ray = Ray(iPoint, L);
		float shadow_t = -1;
		Point3 shadow_pt = Point3(0, 0, 0);
		float shadow_t_max = -1.0f;
		if (light_.GetW() == 1) {
			shadow_t_max = (light_.GetPointLtOrigin() - iPoint).Length();
		}

		if (ipp.GetFlagSoftShadow() == 1) {
			// soft shadow
			float jitter_rad = 0.5;
			float soft_shadow_val = 0.0f;
			float soft_itr_size = 60.0f;
			// generate 30 different shadow ray to implement soft shadow
			for (int k = 0; k < soft_itr_size; k++) {
				shadow_flag = 1.0f;
				for (int j = 0; j < sphere_vec.size(); j++) {
					// prevent numerical error due to self_intersect
					if (j == obj_idx) {
						continue;
					}
					Sphere shadow_sphere = sphere_vec.at(j);
					if (shadow_ray.IntersectSphere(shadow_sphere.GetOrigin(), shadow_sphere.GetRadius(), &shadow_t, &shadow_pt)) {
						if (shadow_t > 0) {
							if ((light_.GetW() == 1 && shadow_t < shadow_t_max) || light_.GetW() == 0) {
								// there is some anpther sphere exist between the current sphere and current light source
								shadow_flag = 0.0f;
							}
						}
					}
					soft_shadow_val += shadow_flag;
				}
				// generate random shadow ray here for next round
				Point3 new_lt_origin = light_.GetPointLtOrigin();
				for (int l = 0; l < 3; l++) {
					std::random_device randdev;
					std::mt19937 generator(randdev());
					std::uniform_real_distribution<> distrib(-jitter_rad, jitter_rad);
					float jit_delta = distrib(generator);
					new_lt_origin[l] += jit_delta;
				}
				Vector3 L_jit = new_lt_origin - iPoint;
				L_jit.Normalize();
				shadow_ray.set(new_lt_origin, L_jit);
			}
			shadow_flag = soft_shadow_val / soft_itr_size;
		}
		else {
			// hard shadow
			for (int j = 0; j < sphere_vec.size(); j++) {
				// prevent numerical error due to self_intersect
				if (j == obj_idx) {
					continue;
				}
				Sphere shadow_sphere = sphere_vec.at(j);
				if (shadow_ray.IntersectSphere(shadow_sphere.GetOrigin(), shadow_sphere.GetRadius(), &shadow_t, &shadow_pt)) {
					if (shadow_t > 0) {
						if ((light_.GetW() == 1 && shadow_t < shadow_t_max) || light_.GetW() == 0) {
							// there is some anpther sphere exist between the current sphere and current light source
							shadow_flag = 0.0f;
						}
					}
				}
			}
		}
		

		// adding I terms together
		Ir2 += shadow_flag * att_factor * light_intense * (kd * diffuse_color_.red() * (N.Dot(L) > 0 ? N.Dot(L) : 0) +
			ks * specu_color_.red() * pow((N.Dot(H) > 0 ? N.Dot(H) : 0), ipp.GetN()));

		Ig2 += shadow_flag * att_factor * light_intense * (kd * diffuse_color_.green() * (N.Dot(L) > 0 ? N.Dot(L) : 0) +
			ks * specu_color_.green() * pow((N.Dot(H) > 0 ? N.Dot(H) : 0), ipp.GetN()));

		Ib2 += shadow_flag * att_factor * light_intense * (kd * diffuse_color_.blue() * (N.Dot(L) > 0 ? N.Dot(L) : 0) +
			ks * specu_color_.blue() * pow((N.Dot(H) > 0 ? N.Dot(H) : 0), ipp.GetN()));
	}

	float Ir = Ir1 + Ir2;
	float Ig = Ig1 + Ig2;
	float Ib = Ib1 + Ib2;
	
	Ir = Ir > 1.0f ? 1.0f : Ir;
	Ig = Ig > 1.0f ? 1.0f : Ig;
	Ib = Ib > 1.0f ? 1.0f : Ib;

	Color c = Color(floor(Ir * 255), floor(Ig * 255), floor(Ib * 255));

	return c;
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
		if (!(hit1) && !(hit2)) {
			return false;
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

/*
eye 0.0 0.0 8.0
viewdir 0.0 0.0 -1.0
updir 0 1 0
vfov 60
imsize 800 800

bkgcolor 0 0 0

attlight 0 0 -1 1 1 1 1 1 0.5 0.1

mtlcolor 1 1 0 1 1 1 0.1 0.9 0 22
sphere -4 0 -7.5 3.0

mtlcolor 0 1 0 1 1 1 0.1 0.8 0.6 22
sphere 4 0 -5 3


*/