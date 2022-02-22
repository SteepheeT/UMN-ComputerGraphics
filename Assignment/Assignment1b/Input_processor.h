#ifndef INPUT_PROCESSOR_H_
#define INPUT_PROCESSOR_H_

#include <string>
#include <vector>
#include "Point3.h"
#include "Vector3.h"
#include "Color.h"
#include "Sphere.h"
#include "LightSource.h"

class InputProcessor {
public:
	InputProcessor();
	~InputProcessor();

	void OutputStoredData();

	int CheckFileContent(const std::string keyword, std::ifstream& in_stream);
	void ReadDataFromFile(const std::string& filename);
	void SetEyePos(float x_, float y_, float z_);
	void SetViwDir(float x_, float y_, float z_);
	void SetUpDir(float x_, float y_, float z_);
	void SetFov(float vfov_);
	void SetImageSize(int width, int height);
	void SetBgColor(float red, float green, float blue);
	void SetMtColor(float red, float green, float blue);
	void SetSpColor(float red, float green, float blue);
	void SetLightSource(float x_, float y_, float z_, float w, float red, float green, float blue);
	void SetAttPtLightSource(float x_, float y_, float z_, float w, float red, float green, float blue, float c1, float c2, float c3);
	void SetKaMultiplier(float ka);
	void SetKdMultiplier(float kd);
	void SetKsMultiplier(float ks);
	void SetSphereData(float x_, float y_, float z_, float radius);
	void SetFlagLightAtt();
	void SetFlagSoftShadow();
	void SetN(int n);
	
	void SetCylinderData();

	Point3 GetEyePos() { return eye; }
	Vector3 GetViwDir() { return view_dir; }
	Vector3 GetUpDir() { return up_dir; }
	float GetFov() { return fov; }
	int* GetImageSize() { return image_size; }
	Color GetBgColor() { return bg_color; }
	Color GetIppMtColor() { return material_color; }
	Color GetIppSpColor() { return specular_color; }
	float GetKaMultiplier() { return ka_; }
	float GetKdMultiplier() { return kd_; }
	float GetKsMultiplier() { return ks_; }
	int GetFlagLightAtt() { return flag_light_attenuation; }
	int GetFlagSoftShadow() { return flag_soft_shadow; }
	int GetN() { return n_; }

	std::vector<LightSource> GetLightVector() { return light_arr; }
	std::vector<Sphere> GetSphereVector() { return sphere_arr; }

private:
	void findKeywordIdx(int& idx, const std::string keyword);

	std::string argument_keyword[9] = { "eye", "viewdir", "updir", "vfov", "imsize", "bkgcolor", "mtlcolor", "light", "attlight" };
	std::string object_keyword[2] = { "sphere", "cylinder" };
	Point3 eye;
	Vector3 view_dir;
	Vector3 up_dir;
	float fov;
	int image_size[2];
	Color bg_color;
	Color material_color;
	Color specular_color;
	std::vector<Sphere> sphere_arr;
	std::vector<LightSource> light_arr;
	float ka_, kd_, ks_;
	int n_;
	int flag_light_attenuation = 0, flag_soft_shadow = 0;

};



#endif // !INPUT_PROCESSOR_H_
