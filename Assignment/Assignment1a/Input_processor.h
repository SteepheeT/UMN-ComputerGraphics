#ifndef INPUT_PROCESSOR_H_
#define INPUT_PROCESSOR_H_

#include <string>
#include <vector>
#include "Point3.h"
#include "Vector3.h"
#include "Color.h"
#include "Sphere.h"

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
	void SetSphereData(float x_, float y_, float z_, float radius);
	void SetCylinderData();

	Point3 GetEyePos() { return eye; }
	Vector3 GetViwDir() { return view_dir; }
	Vector3 GetUpDir() { return up_dir; }
	float GetFov() { return fov; }
	int* GetImageSize() { return image_size; }
	Color GetBgColor() { return bg_color; }
	Color GetMtColor() { return material_color; }
	
	std::vector<Sphere> GetSphereVector() { return sphere_arr; }

private:
	void findKeywordIdx(int& idx, const std::string keyword);

	std::string argument_keyword[7] = { "eye", "viewdir", "updir", "vfov", "imsize", "bkgcolor", "mtlcolor" };
	std::string object_keyword[2] = { "sphere", "cylinder" };
	Point3 eye;
	Vector3 view_dir;
	Vector3 up_dir;
	float fov;
	int image_size[2];
	Color bg_color;
	Color material_color;
	std::vector<Sphere> sphere_arr;
	
};



#endif // !INPUT_PROCESSOR_H_
