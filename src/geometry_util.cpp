#include "geometry_util.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader.h>

ImageTexture::ImageTexture(const std::string& image_path) {
    // Create and configure texture handle
    unsigned int handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image into the texture if it's available
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(image_path.c_str(), &width, &height, &channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
}

ObjMesh::ObjMesh(const std::string& obj_path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, obj_path.c_str());

    if (!warn.empty()) {
        std::cout << "tinyobjloader warning: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "tinyobjloader error: " << err << std::endl;
    }

    if (!ret) {
        std::cerr << "Error occurred while trying to load " << obj_path << std::endl;
        return;
    }

    // Assume that the entire model uses the same set of textures
    num_triangles = 0;
    std::unordered_map<Vertex, unsigned int> unique_vertices;
    for (const auto& shape : shapes) {
        for (const auto& index : shapes[0].mesh.indices) {
            Vertex vertex{};

            vertex.position = glm::vec3(attrib.vertices[index.vertex_index + 0],
                attrib.vertices[index.vertex_index + 1],
                attrib.vertices[index.vertex_index + 2]);

            vertex.uv = glm::vec2(attrib.texcoords[index.texcoord_index + 0],
                attrib.texcoords[index.texcoord_index + 1]);

            if (unique_vertices.count(vertex) == 0) {
                unique_vertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(unique_vertices[vertex]);
            num_triangles++;
        }
    }
    num_triangles /= 3;

    // Configure the OpenGL handles
    glGenVertexArrays(1, &handle);
    glBindVertexArray(handle);

    // Two buffers; one for the positions and the other for the uv-coords.
    unsigned int buffer;
    glGenBuffers(1, &buffer);

    // Configure both buffers
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    unsigned int index_buffer = 0;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}