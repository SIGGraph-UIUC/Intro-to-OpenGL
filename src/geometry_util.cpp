#include "geometry_util.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader.h>

std::filesystem::path resource_directory = RESOURCES_DIR;

ImageTexture::ImageTexture(const std::string& image_file) {
    // Create and configure texture handle
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image into the texture if it's available
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    std::filesystem::path image_path = resource_directory / image_file;
    std::string path = image_path.string();
    std::cout << "Loading image from " << path << std::endl;

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
}

ObjMesh::ObjMesh(const std::string& obj_file) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;
    std::filesystem::path obj_path = resource_directory / obj_file;
    std::string path = obj_path.string();
    std::cout << "Loading OBJ file from " << path << std::endl;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());

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
    std::unordered_map<Vertex, uint32_t> unique_vertices{};
    for (const auto& shape : shapes) {
        for (size_t index = 0; index < shape.mesh.indices.size(); index += 3) {
            Vertex triangle[3];
            unsigned int triangle_indices[3];
            for (int i = 0; i < 3; i++) {
                tinyobj::index_t idx = shape.mesh.indices[index + i];
                triangle[i].position = glm::vec3(attrib.vertices[3 * idx.vertex_index + 0],
                                                 attrib.vertices[3 * idx.vertex_index + 1],
                                                 attrib.vertices[3 * idx.vertex_index + 2]);

                triangle[i].uv = glm::vec2(attrib.texcoords[2 * idx.texcoord_index + 0],
                                           attrib.texcoords[2 * idx.texcoord_index + 1]);

                triangle[i].normal = glm::vec3(0.0f, 0.0f, 0.0f);

                if (unique_vertices.count(triangle[i]) == 0) {
                    unique_vertices[triangle[i]] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(triangle[i]);
                }

                triangle_indices[i] = unique_vertices[triangle[i]];
                indices.push_back(unique_vertices[triangle[i]]);
            }

            glm::vec3 e1 = triangle[1].position - triangle[0].position;
            glm::vec3 e2 = triangle[2].position - triangle[0].position;
            glm::vec3 normal = glm::cross(e1, e2);

            for (int i = 0; i < 3; i++) {
                vertices[triangle_indices[i]].normal += normal;
            }
        }
    }

    for (Vertex& vertex : vertices) {
        vertex.normal = glm::normalize(vertex.normal);
    }

    num_triangles = indices.size() / 3;
    std::cout << "Number of faces read from " << obj_file << ": " << num_triangles << std::endl;

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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    unsigned int index_buffer = 0;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

unsigned int ObjMesh::get_num_vertices() const {
    return 3 * num_triangles;
}