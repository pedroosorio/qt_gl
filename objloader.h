#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "mesh.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <cctype>

#include <QDebug>

class OBJLoader
{
public:
    OBJLoader(Mesh *target);
    ~OBJLoader();
    bool LoadOBJ(std::string path);
private:
    Mesh *target_mesh;
    // Parse functions
    bool discardLine(std::string &line, std::vector<std::string> &line_args);
    bool parseLine(std::string &line);
    bool parseVectorEntry(std::vector<std::string> &line_args);
    bool parseUVEntry(std::vector<std::string> &line_args);
    bool parseNormalEntry(std::vector<std::string> &line_args);
    bool parseFaceEntry(std::vector<std::string> &line_args);
    void indexMeshData();
    // State bools
    bool has_vertex_data, has_uv_data, has_normal_data, has_face_data;
    // Temp data that will only be passed onto target_mesh if the load is successful
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<MeshFace> faces;
};

#endif // OBJLOADER_H
