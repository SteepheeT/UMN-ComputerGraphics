#include <iostream>
#include <string>
#include <fstream>
#include <cmath>



bool isPosNumber(const std::string &str) {
	if (str == "") {
		return false;
	}
	for (char const& c : str) {
		if (std::isdigit(c) == 0) return false;
	}
	return true;
}

/* 
 * This fucntion checks if the provided input file name is a valid .txt file
 * 
 * @param fileName Output file name provided by user.
 * @return ans Returns whether the file name is valid, true-valid / false-invalid
 */
bool check_if_input_valid(std::string fileName) {
	bool ans = true;
	if (fileName.length() <= 4) {
		ans = false;
	}
	else {
		if (fileName.substr(fileName.length() - 4, 3) != ".txt") {
			ans = false;
		}
	}
	return ans;
}

/*
 * This fucntion checks if the provided output file name is a valid .ppm file.
 * If the file name does not contain '.ppm' the function will add the suffix at the end.
 * If the file name contains suffix but not the correct '.ppm' suffix, the funciton will
 *	ask users if they want to auto-change the suffix to '.ppm'. If yes, drop the current
 *	suffix and add '.ppm', else ignore it and continue.
 *
 * @param fileName Setting file name provided by user.
 */
void check_if_out_contain_ppm(std::string &fileName) {
	size_t idx = fileName.find(".");
	if (idx == std::string::npos) {
		std::cout << "Did not found output file suffix, adding '.ppm' automatically." << std::endl;
		fileName += ".ppm";
		return;
	}
	else {
		std::string subFileName = fileName.substr(idx + 1, fileName.length() - idx);
		size_t idx2 = subFileName.find(".");
		if(idx2 != std::string::npos){
			std::cout << "Illegal filename, '.' is not allowed to be inside a filename." << std::endl;
			std::cout << "Default output file name will be applied." << std::endl;
			fileName = "default_output.ppm";
			return;
		}
		else{
			std::string fileSuffix;
			fileSuffix = fileName.substr(idx, fileName.length() - idx);
			if(fileSuffix != ".ppm"){
				std::cout << "Provided output file not end with '.ppm', currently is: " << fileName 
					<< ", do you want to fix it?" << std::endl;
				std::cout << "enter 'y' to correct it, other to ignore this." << std::endl;
				char ans;
				std::cin >> ans;
				if (ans == 'y') {
					std::string str;
					str = fileName.substr(0, idx);
					str += ".ppm";
					fileName = str;
					return;
				}
				else {
					return;
				}
			}
		}
	}
}

int main(int argc, char* argv[]) {
	std::string height_str = "", width_str = "";
	int height = -1, width = -1, limit = 255;

	// pre-define the output ppe file name
	std::string input_file = "";
	std::string output_file = "default_output.ppm";

	// check command line parameters number and process the parameter.
	if (argc > 3) {
		std::cout << "Too much parameters." << std::endl;
		std::cout << "Usage: [Executble File] [Setting txt File Name] [Output ppm File Name]" << std::endl;
		std::cout << "If [Output ppm File Name] are not specified, output file will use the same name as the input txt file." << std::endl;
		std::cout << "If both [Setting txt File Name] and [Output ppm File Name] are not specified, default settings will applied." << std::endl;
		return 1;
	}
	else if (argc == 1) {
		std::cout << "Didn't receive additional parameter, applying default setting." << std::endl;
		std::cout << "To use customized setting. Specify the setting file and the output file name." << std::endl;
		std::cout << "Usage: [Executble File] [Setting txt File Name] [Output ppm File Name]" << std::endl;
		width = 1024;
		height = 768;
	}
	else if (argc == 2) {
		input_file = argv[1];
		std::cout << "One parameter received." << std::endl;
		if (check_if_input_valid(input_file)) {
			std::cout << "Invalid setting file format." << input_file << std::endl;
			return 1;
		}
		else {
			std::cout << "Reading setting from file: " << input_file << std::endl;
		}
		std::cout << "Output file name will use the same name as input setting file." << std::endl;
		std::cout << "To use customized setting. Please specify the setting file name and the output file name." << std::endl;
		std::cout << "Usage: [Executble File] [Setting txt File Name] [Output ppm File Name]" << std::endl;

		// Read setting data from provided input file
		std::ifstream in_stream(input_file, std::ios::in);
		
		if (in_stream.is_open()) {
			std::string keyword = "";
			in_stream >> keyword;
			if (keyword != "") {
				if (isPosNumber(keyword)) {
					std::cout << "[ERROR] Missing keyword of input file." << std::endl;
					return 1;
				}
				else {
					// If the keyword position is a negative value, treat it as missing keyword
					if (keyword[0] == '-') {
						std::string substring = keyword.substr(1, keyword.length() - 1);
						if (isPosNumber(substring)) {
							std::cout << "[ERROR] Missing keyword of input file." << std::endl;
							return 1;
						}
					}
					else {
						if (keyword != "imsize") {
							std::cout << "[ERROR] Incorrect keyword of input file." << std::endl;
							std::cout << "[ERROR] Found keyword: " << keyword << std::endl;
							return 1;
						}
					}
				}
			}
			if (in_stream.eof() && keyword == "") {
				std::cout << "[ERROR] Missing keyword of input file." << std::endl;
				return 1;
			}

			in_stream >> width_str;
			if (isPosNumber(width_str)) {
				width = std::stoi(width_str);
			}
			else {
				if (width_str == "") {
					std::cout << "[ERROR] Width of input file missing." << std::endl;
					return 1;
				}
				std::cout << "[ERROR] Width of input file should be a positive integer." << std::endl;
				return 1;
			}

			in_stream >> height_str;
			if (isPosNumber(height_str)) {
				height = std::stoi(height_str);
			}
			else {
				if (height_str == "") {
					std::cout << "[ERROR] Height of input file missing." << std::endl;
					return 1;
				}
				std::cout << "[ERROR] Height of input file should be a positive integer." << std::endl;
				return 1;
			}
			in_stream.close();
		}
		std::string str = input_file.substr(0, input_file.length() - 4);
		output_file = str + ".ppm";
	}
	else {
		input_file = argv[1];
		output_file = argv[2];
		std::cout << "Two parameter received." << std::endl;
		if (check_if_input_valid(input_file)) {
			std::cout << "Invalid setting file format." << input_file << std::endl;
			return 1;
		}
		else {
			std::cout << "Reading setting from file: " << input_file << std::endl;
		}
		check_if_out_contain_ppm(output_file);
		std::cout << "Output data to file: " << output_file << std::endl;
		std::ifstream in_stream(input_file, std::ios::in);
		if (in_stream.is_open()) {
			in_stream >> width >> height;
			in_stream.close();
			if (height < 0 || width < 0) {
				std::cout << std::endl;
				std::cout << "[ERROR]Bad setting parameter detected" << std::endl;
				std::cout << "Three setting digits CANNOT be NEGATIVE." << std::endl;
				std::cout << std::endl;
				std::cout << "Program halt." << std::endl;
				return 1;
			}
		}
		else {
			std::cout << "Openning file error." << std::endl;
			return 1;
		}
	}
	
	std::ofstream out_fstream(output_file, std::ios::out);

	out_fstream << "P3\n"
		<< "#oaT euynuJ yb detaerC_0WH7065ICSC" << "\n"
		<< width << "\n"
		<< height << "\n"
		<< limit << "\n";

	int cutoff = height < width ? std::floor(height / 4) : std::floor(width / 4);

	for (uint32_t i = 0; i < height; i++) {
		for (uint32_t j = 0; j < width; j++) {
			if (j % 4 == 0) {
				out_fstream << "\n";
			}
			if ((i * j) % cutoff == 0) {
				out_fstream << 12 << " " << 255 << " " << 255 << " ";
			}
			else {
				out_fstream << 25 << " " << 51 << " " << 0 << " ";
			}
		}
	}

	out_fstream.close();
	return 0;
}
