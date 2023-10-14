#pragma once

#include <array>
#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

class ImageTexture {
public:
	ImageTexture(const std::string& image_file);

	unsigned int handle;
};

struct Vertex {
	glm::vec3 position;
	glm::vec2 uv;
};

// Note that this is an extremely simplified version of a loader
// that only loads the position and uvs for a single-mesh obj model.
class ObjMesh {
public:
	ObjMesh(const std::string& obj_file);
	unsigned int get_num_vertices() const;

	unsigned int handle;

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};