#pragma once

#include <array>
#include <iostream>
#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

class ImageTexture {
public:
	ImageTexture(const std::string& image_path);

	unsigned int handle;
};

// We define a vertex with a hash function to avoid duplicating vertices
struct Vertex {
	glm::vec3 position;
	glm::vec2 uv;

	bool operator==(const Vertex& other) const {
		return position == other.position && uv == other.uv;
	}
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return (hash<glm::vec3>()(vertex.position) ^
				(hash<glm::vec2>()(vertex.uv) << 1));
		}
	};
}

// Note that this is an extremely simplified version of a loader
// that only loads the position and uvs for a single-mesh obj model.
class ObjMesh {
public:
	ObjMesh(const std::string& obj_path);

	unsigned int handle;
	unsigned int num_triangles;

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};