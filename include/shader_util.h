#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>

class Shader {
public:
	Shader(const std::string& vertex_path, const std::string& fragment_path);

	unsigned int handle;

private:
	std::string read_shader(const std::string& shader_path);
	unsigned int compile_shader(const std::string& shader_path, GLenum type);
};
