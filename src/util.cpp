#include "util.h"

std::filesystem::path shader_directory = SHADER_DIR;

std::string read_shader(const std::string& shader_file) {
    // CMake will define the shader directory path
    std::filesystem::path shader_path = shader_directory / shader_file;
	std::fstream file_stream;
	std::stringstream string_stream;
	std::string code;
	try {
        std::cout << "Compiling shader from " << shader_path.string() << std::endl;
		file_stream.open(shader_path);
		string_stream << file_stream.rdbuf();
		file_stream.close();
		code = string_stream.str();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "Failed to read shader" << std::endl;
	}

	return code;
}

int compile_shader(const std::string& shader_file, GLenum type) {
    int shader = glCreateShader(type);
    std::string shader_code = read_shader(shader_file);
    const char* code = shader_code.c_str();
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);

    char log[512];
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, log);
        std::cerr << "Shader compilation failed: \n" << log << std::endl;
    };

    return shader;
}

int create_program(const std::string& vertex_path, const std::string& fragment_path) {
    int vertex_shader = compile_shader(vertex_path, GL_VERTEX_SHADER);
    int fragment_shader = compile_shader(fragment_path, GL_FRAGMENT_SHADER);

    int program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    char log[512];
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, log);
        std::cerr << "Shader program linking failed: \n" << log << std::endl;
    }

    return program;
}