#include "util.h"

const char* read_shader(const std::string& shader_path) {
	std::fstream file_stream;
	std::stringstream string_stream;
	std::string code;
	try {
		file_stream.open(shader_path);
		string_stream << file_stream.rdbuf();
		file_stream.close();
		code = string_stream.str();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "Failed to read shader" << std::endl;
	}

	return code.c_str();
}