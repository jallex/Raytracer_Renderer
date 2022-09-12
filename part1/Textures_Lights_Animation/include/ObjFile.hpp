#ifndef OBJ_HPP
#define OBJ_HPP

#include <string>
#include <vector>

#include "glm/vec3.hpp"

class ObjFile{
public:
    // Constructor loads a filename with the .obj extension
    ObjFile(std::string fileName);
    //Return vertice data
    inline std::vector<float> getVertexData() const { return m_verticeData; }
    //Return indice data
    inline std::vector<int> getIndiceData() const { return m_indices; }
    
private:
    //Object vertices
    std::vector<float> m_verticeData;
    //Object indicies
    std::vector<int> m_indices;
};

#endif