#include "objloader.h"

OBJLoader::OBJLoader(Mesh *target)
{
    target_mesh = target;
    has_vertex_data = has_uv_data = has_normal_data = has_face_data = false;
}

OBJLoader::~OBJLoader()
{

}

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

// split a string into a vector of strings
static inline std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

// For now, just read pure-mesh data, ignoring everything regarding materials and textures
bool OBJLoader::LoadOBJ(std::string path)
{
    vertices.clear(); indices.clear(); uvs.clear(); normals.clear(); faces.clear();
    std::ifstream objfile(path);
    if(!objfile.is_open()) { qDebug() << "Failed to open" << path.c_str(); return false; }
    bool no_error = true;
    for( std::string line; getline(objfile,line);)
    {
        trim(line);
        no_error = parseLine(line);
        if(!no_error) return false;
    }

    indexMeshData();
    return true;
}

bool OBJLoader::discardLine(std::string& line, std::vector<std::string>& line_args)
{
    trim(line);
    if(line.size() <= 1) return true; //Discard empty lines
    if(line[0] == '#') return true; //Discard comments
    line_args = split(line, ' ');
    // For now, just analyse pure mesh data
    // Possible values: ' ' separating axis data
    // ------------------------------- SUPPORTED
    // v - vertices (x, y and z)
    // vt - uv texture coordinate (x and y)
    // vn - normals (x , y and z)
    // f - face data (x, y and z) - each component (v/vt/vn) is separated using '/'
    // ------------------------------- NOT SUPPORTED
    // vp - free form geometry
    // l - line data
    // mtllib - material specified using a .mtl lib
    // usemtl - name of the material
    // s - smooth shading on or off
    // o - object data
    // g - group data
    if(line_args[0] == "v" || line_args[0] == "vt" || line_args[0] == "vn" || line_args[0] == "f") return false;
    return true;
}

bool OBJLoader::parseLine(std::string& line)
{
    std::vector<std::string> line_args;
    if(discardLine(line, line_args)) return true;
    std::string command = line_args[0];
    bool ret = false;
    if(command == "v") { ret = parseVectorEntry(line_args); has_vertex_data = true; return ret; }
    if(command == "vt") { ret = parseUVEntry(line_args); has_uv_data = true; return ret; }
    if(command == "vn") { ret = parseNormalEntry(line_args); has_normal_data = true; return ret; }
    if(command == "f") { ret = parseFaceEntry(line_args); has_face_data = true; return ret; }
    return ret;
}

bool OBJLoader::parseVectorEntry(std::vector<std::string> &line_args)
{
    //A vector entry has 3 values
    if(line_args.size()!=4) return false;
    vertices.push_back(glm::vec3( std::stof(line_args[1]),
                                  std::stof(line_args[2]),
                                  std::stof(line_args[3]) ));
    return true;
}

bool OBJLoader::parseUVEntry(std::vector<std::string> &line_args)
{
    if(line_args.size()!=3) return false;
    uvs.push_back(glm::vec2( std::stof(line_args[1]),
                             std::stof(line_args[2]) ));
    return true;
}

bool OBJLoader::parseNormalEntry(std::vector<std::string> &line_args)
{
    if(line_args.size()!=4) return false;
    normals.push_back(glm::vec3( std::stof(line_args[1]),
                                 std::stof(line_args[2]),
                                 std::stof(line_args[3]) ));
    return true;
}

bool OBJLoader::parseFaceEntry(std::vector<std::string> &line_args)
{
    if(line_args.size()!=4) return false;
    int32_t face_values = 1; //at least, the vertex
    if(has_uv_data) face_values++;
    if(has_normal_data) face_values++;

    if(faces.size()==0){
        //Output some small report
        qDebug() << "OBJ File Loading:\n" << "\tvertices:" << vertices.size() << "\n\t uvs:" << uvs.size() << "\n\t normals:" << normals.size();
    }

    MeshFace face;
    for(int arg=1; arg < 4; arg++){
        std::vector<std::string> vertex_args = split(line_args[arg], '/');
        face.p[arg-1].x = std::stoi(vertex_args[0]);
        if(face_values>1) face.p[arg-1].y = std::stoi(vertex_args[1]);
        if(face_values>2) face.p[arg-1].z = std::stoi(vertex_args[2]);
    }

    faces.push_back(face);
    return true;
}

void OBJLoader::indexMeshData()
{
    target_mesh->vertices = vertices;

    for(auto face: faces){
        // VERTEX - UV - NORMALS
        target_mesh->indices.push_back(static_cast<GLuint>(face.p[0].x-1));
        target_mesh->indices.push_back(static_cast<GLuint>(face.p[1].x-1));
        target_mesh->indices.push_back(static_cast<GLuint>(face.p[2].x-1));

        if(has_uv_data){
            target_mesh->uvs.push_back(uvs[face.p[0].y-1]);
            target_mesh->uvs.push_back(uvs[face.p[1].y-1]);
            target_mesh->uvs.push_back(uvs[face.p[2].y-1]);
            if(has_normal_data){
                target_mesh->normals.push_back(normals[face.p[0].z-1]);
                target_mesh->normals.push_back(normals[face.p[1].z-1]);
                target_mesh->normals.push_back(normals[face.p[2].z-1]);
            }
        } else if(has_normal_data){
            target_mesh->normals.push_back(normals[face.p[0].y-1]);
            target_mesh->normals.push_back(normals[face.p[1].y-1]);
            target_mesh->normals.push_back(normals[face.p[2].y-1]);
        }

        //Save faces to mesh
    }
}
