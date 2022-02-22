#include "Input_processor.h"
#include "Point3.h"
#include <iostream>
#include <fstream>
#include <regex>

void InputProcessor::findKeywordIdx(int& idx, const std::string keyword) {
	int arg_size = sizeof(argument_keyword) / sizeof(argument_keyword[0]);
	for (int i = 0; i < arg_size; i++) {
		if (argument_keyword[i] == keyword) {
			idx = i;
			return;
		}
	}
	int obj_size = sizeof(object_keyword) / sizeof(object_keyword[0]);
	for (int i = 0; i < obj_size; i++) {
		if (object_keyword[i] == keyword) {
			idx = i + arg_size;
			return;
		}
	}
	return;
}

bool isPosNumber(std::string& str) {
	if (str == "") {
		return false;
	}
	else if (str[0] == '.') {
		std::string zero = "0";
		str = zero + str;
	}
	return std::regex_match(str, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
}

InputProcessor::InputProcessor() {
	fov = 0;
	image_size[0] = 0;
	image_size[1] = 0;
	ka_ = 0;
	kd_ = 0;
	ks_ = 0;
	n_ = 0;
}

InputProcessor::~InputProcessor() {

}

void InputProcessor::OutputStoredData() {
	std::cout << GetEyePos() << std::endl;
	std::cout << GetViwDir() << std::endl;
	std::cout << GetUpDir() << std::endl;
	std::cout << GetFov() << std::endl;
	std::cout << "width: " << GetImageSize()[0] << ", height: " << GetImageSize()[1] << std::endl;
	std::cout << GetBgColor() << std::endl;
	std::cout << GetIppMtColor() << std::endl;
	std::cout << GetSphereVector().at(0) << std::endl;
	return;
}

void InputProcessor::ReadDataFromFile(const std::string& filename) {
	std::cout << "================= start process input =================" << std::endl;
	std::ifstream in_stream(filename, std::ios::in);
	if (!in_stream.is_open()) {
		return;
	}
	else {
		std::string keyword;
		while (true) {
			keyword = "";
			in_stream >> keyword;
			if (keyword == "") {
				std::cout << "reach the end of file" << std::endl;
				break;
			}
			else {
				std::cout << "read in: " << keyword << std::endl;
				CheckFileContent(keyword, in_stream);
			}
		}
		
	}
	in_stream.close();
}

int InputProcessor::CheckFileContent(const std::string read_keyword, std::ifstream& in_stream) {
	int key_index = -1;
	findKeywordIdx(key_index, read_keyword);

	if(key_index != -1){
		if (key_index == 0) {
			std::string eye_x = "", eye_y = "", eye_z = "";
			in_stream >> eye_x >> eye_y >> eye_z;
			if (isPosNumber(eye_x) && isPosNumber(eye_y) && isPosNumber(eye_z)) {
				SetEyePos(std::stof(eye_x), std::stof(eye_y), std::stof(eye_z));
			}
			else {
				std::cout << "[ERROR] Incorrect or missing eye position corrdinates data." << std::endl;
				throw std::invalid_argument("Invalid eye position argument.");
			}
		}
		else if (key_index == 1) {
			std::string vdir_x = "", vdir_y = "", vdir_z = "";
			in_stream >> vdir_x >> vdir_y >> vdir_z;
			if (isPosNumber(vdir_x) && isPosNumber(vdir_y) && isPosNumber(vdir_z)) {
				SetViwDir(std::stof(vdir_x), std::stof(vdir_y), std::stof(vdir_z));
				view_dir.Normalize();
			}
			else {
				std::cout << "[ERROR] Incorrect or missing view direction data." << std::endl;
				throw std::invalid_argument("Invalid view direction argument.");
			}
		}
		else if (key_index == 2) {
			std::string updir_x = "", updir_y = "", updir_z = "";
			in_stream >> updir_x >> updir_y >> updir_z;
			if (isPosNumber(updir_x) && isPosNumber(updir_y) && isPosNumber(updir_z)) {
				SetUpDir(std::stof(updir_x), std::stof(updir_y), std::stof(updir_z));
				up_dir.Normalize();
			}
			else {
				std::cout << "[ERROR] Incorrect or missing up direction data." << std::endl;
				throw std::invalid_argument("Invalid up direction argument.");
			}
		}
		else if (key_index == 3) {
			std::string view_fov = "";
			in_stream >> view_fov;
			if (isPosNumber(view_fov)) {
				if (std::stof(view_fov) > 180) {
					std::cout << "[ERROR] Invalid fov value, too large." << std::endl;

				}
				else {
					// fov = std::stof(view_fov);
					SetFov(std::stof(view_fov));
				}
			}
			else {
				std::cout << "[ERROR] Incorrect or missing fov data." << std::endl;
				throw std::invalid_argument("Invalid vfov argument.");
			}
		}
		else if (key_index == 4) {
			std::string width = "", height = "";
			in_stream >> width >> height;
			if (isPosNumber(width) && isPosNumber(height)) {
				if (std::stoi(width) * std::stoi(height) == 0) {
					throw std::invalid_argument("Screen size can not be zero.");
				}
				SetImageSize(std::stoi(width), std::stoi(height));
			}
			else {
				std::cout << "[ERROR] Incorrect or missing image size data." << std::endl;
				throw std::invalid_argument("Invalid image size data.");
			}
		}
		else if (key_index == 5) {
			std::string red = "", green = "", blue = "";
			in_stream >> red >> green >> blue;
			if (isPosNumber(red) && isPosNumber(green) && isPosNumber(blue)) {
				if (std::stof(red) > 1.0f || std::stof(red) < 0.0f ||
					std::stof(green) > 1.0f || std::stof(green) < 0.0f ||
					std::stof(blue) > 1.0f || std::stof(blue) < 0.0f) {
					std::cout << "[ERROR] Enter color value between 0.0 - 1.0." << std::endl;
				}
				else {
					SetBgColor(std::stof(red), std::stof(green), std::stof(blue));
				}
				
			}
			else {
				std::cout << "[ERROR] Incorrect or missing background color data." << std::endl;
				throw std::invalid_argument("Invalid background color argument.");
			}
		}
		else if (key_index == 6) {
			std::string red = "", green = "", blue = "";
			in_stream >> red >> green >> blue;
			if (isPosNumber(red) && isPosNumber(green) && isPosNumber(blue)) {
				if (std::stof(red) > 1.0f || std::stof(red) < 0.0f ||
					std::stof(green) > 1.0f || std::stof(green) < 0.0f ||
					std::stof(blue) > 1.0f || std::stof(blue) < 0.0f) {
					std::cout << "[ERROR] Enter color value between 0.0 - 1.0." << std::endl;
				}
				else {
					SetMtColor(std::stof(red), std::stof(green), std::stof(blue));
					in_stream >> red >> green >> blue;
					if (isPosNumber(red) && isPosNumber(green) && isPosNumber(blue)) {
						if (std::stof(red) > 1.0f || std::stof(red) < 0.0f ||
							std::stof(green) > 1.0f || std::stof(green) < 0.0f ||
							std::stof(blue) > 1.0f || std::stof(blue) < 0.0f) {
							std::cout << "[ERROR] Enter color value between 0.0 - 1.0." << std::endl;
						}
						else {
							SetSpColor(std::stof(red), std::stof(green), std::stof(blue));
							std::string ka = "", kd = "", ks = "";
							in_stream >> ka >> kd >> ks;
							if (isPosNumber(ka) && isPosNumber(kd) && isPosNumber(ks)) {
								if (std::stof(ka) < 0.0f || std::stof(kd) < 0.0f || std::stof(ks) < 0.0f) {
									std::cout << "[ERROR] Enter pisitive k value." << std::endl;
								}
								else {
									SetKaMultiplier(std::stof(ka));
									SetKdMultiplier(std::stof(kd));
									SetKsMultiplier(std::stof(ks));
									std::string n = "";
									in_stream >> n;
									if (isPosNumber(n)) {
										if (std::stoi(n) < 0) {
											std::cout << "[ERROR] Enter pisitive k value." << std::endl;
										}
										else {
											SetN(std::stoi(n));
										}
									}
								}
							}
							else {
								std::cout << "[ERROR] Incorrect or missing material color data." << std::endl;
								throw std::invalid_argument("Invalid material color argument.");
							}
						}
					}
					else {
						std::cout << "[ERROR] Incorrect or missing material color data." << std::endl;
						throw std::invalid_argument("Invalid material color argument.");
					}
				}
			}
			else {
				std::cout << "[ERROR] Incorrect or missing material color data." << std::endl;
				throw std::invalid_argument("Invalid material color argument.");
			}
		}
		// normal point or dir light
		else if (key_index == 7) {
			std::string l_x = "", l_y = "", l_z = "", w = "", r = "", g = "", b = "";
			in_stream >> l_x >> l_y >> l_z >> w >> r >> g >> b;
			if (isPosNumber(l_x) && isPosNumber(l_y) && isPosNumber(l_z) && isPosNumber(w)
					&& isPosNumber(r) && isPosNumber(g) && isPosNumber(b)) {
				SetLightSource(std::stof(l_x), std::stof(l_y), std::stof(l_z), 
					std::stoi(w), std::stof(r), std::stof(g), std::stof(b));
			}
		}
		// light attenuation
		else if (key_index == 8) {
		std::string l_x = "", l_y = "", l_z = "", w = "", r = "", g = "", b = "", c1 = "", c2 = "", c3 = "";
			in_stream >> l_x >> l_y >> l_z >> w >> r >> g >> b >> c1 >> c2 >> c3;
			if (isPosNumber(l_x) && isPosNumber(l_y) && isPosNumber(l_z) && isPosNumber(w)
				&& isPosNumber(r) && isPosNumber(g) && isPosNumber(b)
				&& isPosNumber(c1) && isPosNumber(c2) && isPosNumber(c3)) {
				SetAttPtLightSource(std::stof(l_x), std::stof(l_y), std::stof(l_z),
					std::stoi(w), std::stof(r), std::stof(g), std::stof(b),
					std::stof(c1), std::stof(c2), std::stof(c3));
			}
		}
		else if (key_index == 9) {
			std::string cor_x = "", cor_y = "", cor_z = "", radius = "";
			in_stream >> cor_x >> cor_y >> cor_z >> radius;
			if (isPosNumber(cor_x) && isPosNumber(cor_y) && isPosNumber(cor_z) && isPosNumber(radius)) {
				SetSphereData(std::stof(cor_x), std::stof(cor_y), std::stof(cor_z), std::stof(radius));
			}
			else {
				std::cout << "[ERROR] Incorrect or missing background color data." << std::endl;
				throw std::invalid_argument("Invalid Sphere data.");
			}
		}
		else if (key_index == 10) {

		}
	}
	else {
		// std::cout << "[ERROR] Reading incorrect keyword, check the setup file." << std::endl;
		throw std::invalid_argument("Invalid keyword argument: " + read_keyword + ", check setup file.");
	}
	return 0;
}

void InputProcessor::SetEyePos(float x_, float y_, float z_) {
	eye = Point3(x_, y_, z_);
}

void InputProcessor::SetViwDir(float x_, float y_, float z_) {
	view_dir = Vector3(x_, y_, z_);
}

void InputProcessor::SetUpDir(float x_, float y_, float z_) {
	up_dir = Vector3(x_, y_, z_);
}

void InputProcessor::SetFov(float vfov_) {
	fov = vfov_;
}

void InputProcessor::SetImageSize(int width, int height) {
	image_size[0] = width;
	image_size[1] = height;
}

void InputProcessor::SetBgColor(float red, float green, float blue) {
	bg_color = Color(red, green, blue);
}

void InputProcessor::SetMtColor(float red, float green, float blue) {
	material_color = Color((float)red, (float)green, (float)blue);
}

void InputProcessor::SetSpColor(float red, float green, float blue) {
	specular_color = Color((float)red, (float)green, (float)blue);
}

void InputProcessor::SetLightSource(float x_, float y_, float z_, float w, float red, 
	float green, float blue) {
	if (w == 1) {
		// point light
		LightSource lt;
		lt.SetLightSource(Point3(x_, y_, z_), Color(red, green, blue));
		light_arr.push_back(lt);
	}
	else if (w == 0) {
		// directional light
		LightSource lt;
		lt.SetLightSource(Vector3(x_, y_, z_), Color(red, green, blue));
		light_arr.push_back(lt);
	}
}

void InputProcessor::SetAttPtLightSource(float x_, float y_, float z_, float w, float red, 
	float green, float blue, float c1, float c2, float c3) {
		LightSource lt;
		lt.SetAttPtLight(Point3(x_, y_, z_), Color(red, green, blue), c1, c2, c3);
		light_arr.push_back(lt);
}

void InputProcessor::SetKaMultiplier(float ka) {
	ka_ = ka;
}

void InputProcessor::SetKdMultiplier(float kd) {
	kd_ = kd;
}

void InputProcessor::SetKsMultiplier(float ks) {
	ks_ = ks;
}

void InputProcessor::SetFlagLightAtt() {
	flag_light_attenuation = 1;
}

void InputProcessor::SetFlagSoftShadow() {
	flag_soft_shadow = 1;
}

void InputProcessor::SetN(int n) {
	n_ = n;
}

void InputProcessor::SetSphereData(float x_, float y_, float z_, float radius) {
	Sphere s = Sphere(x_, y_, z_, radius);
	s.SetMtColor(GetIppMtColor());
	s.SetSpColor(GetIppSpColor());
	sphere_arr.push_back(s);
}

void InputProcessor::SetCylinderData() {

}


