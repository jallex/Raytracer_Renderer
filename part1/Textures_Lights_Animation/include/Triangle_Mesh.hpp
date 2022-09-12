#ifndef TRIANGLEMESH_HPP
#define TRIANGLEMESH_HPP

#include "object.hpp"
#include "constants.hpp"
#include "Triangle.hpp"
#include "ObjFile.hpp"
#include <limits>

class TriangleMesh : public Object {
public:
    int num_hits;

    TriangleMesh(ObjFile obj, shared_ptr<Material> m) {
        std::vector<float> vertices = obj.getVertexData();
        // Build vertex list
        for(int i = 0; i <= vertices.size() - 3; i+=3){
            glm::vec3 v = glm::vec3(float(vertices[i]), float(vertices[i+1]), float(vertices[i+2]));
            m_vertexList.push_back(v);
        }

        std::vector<int> indices = obj.getIndiceData();
        for(int i = 0; i <= indices.size() - 3; i+=3){
          int indice1 = indices[i];
            int indice2 = indices[i + 1];
            int indice3 = indices[i + 2];
            m_triangles.push_back(make_shared<Triangle>((m_vertexList[indice1]), (m_vertexList[indice2]), (m_vertexList[indice3]), m));
        }

        num_hits = 0;
        current_position = glm::vec3(0,0,0);
    }

    bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const override {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;
    //iterate through triangles, test if ray hit an object
    for (const auto& triangle : m_triangles) {
        if (triangle->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}


//Set object to a new position (translate to)
void move(glm::vec3 new_pos) const override{
    glm::vec3 difference = new_pos - current_position;
    for(int i = 0; i < m_triangles.size(); i++){
        m_triangles[i]->set_v0(m_triangles[i]->get_v0() + difference);
        m_triangles[i]->set_v1(m_triangles[i]->get_v1() + difference);
        m_triangles[i]->set_v2(m_triangles[i]->get_v2() + difference);
    }
}

//Rotate object by given num degrees (rotate by)
void rotatex(float rot_by) const override{
    auto theta = degrees_to_radians(rot_by);
    for(int i = 0; i < m_triangles.size(); i++){

        float x = current_position.x;
        float y = current_position.y;
        float z = current_position.z;

        x = x*cos(rot_by) - z * sin(rot_by);
        z = x*sin(rot_by) + z * cos(rot_by); 

        m_triangles[i]->set_v0(glm::vec3(m_triangles[i]->get_v0().x,
            m_triangles[i]->get_v0().y*cos(theta) - m_triangles[i]->get_v0().z * sin(theta), 
            m_triangles[i]->get_v0().y*sin(theta) + m_triangles[i]->get_v0().z * cos(theta)));

        m_triangles[i]->set_v1(glm::vec3(m_triangles[i]->get_v1().x,
            m_triangles[i]->get_v1().y*cos(theta) - m_triangles[i]->get_v1().z * sin(theta),
             m_triangles[i]->get_v1().y*sin(theta) + m_triangles[i]->get_v1().z * cos(theta)));

        m_triangles[i]->set_v2(glm::vec3(m_triangles[i]->get_v2().x,
            m_triangles[i]->get_v2().y*cos(theta) - m_triangles[i]->get_v2().z * sin(theta),
             m_triangles[i]->get_v2().y*sin(theta) + m_triangles[i]->get_v2().z * cos(theta)));
    }
}

//Rotate object by given num degrees (rotate by)
void rotatey(float rot_by) const override{
    auto theta = degrees_to_radians(rot_by);
    for(int i = 0; i < m_triangles.size(); i++){
        float x = current_position.x;
        float y = current_position.y;
        float z = current_position.z;

        x = x*cos(rot_by) - z * sin(rot_by);
        z = x*sin(rot_by) + z * cos(rot_by); 

        m_triangles[i]->set_v0(glm::vec3(m_triangles[i]->get_v0().x*cos(theta) - m_triangles[i]->get_v0().z * sin(theta),
            m_triangles[i]->get_v0().y, 
            m_triangles[i]->get_v0().x*sin(theta) + m_triangles[i]->get_v0().z * cos(theta)));

        m_triangles[i]->set_v1(glm::vec3(m_triangles[i]->get_v1().x*cos(theta) - m_triangles[i]->get_v1().z * sin(theta),
            m_triangles[i]->get_v1().y,
             m_triangles[i]->get_v1().x*sin(theta) + m_triangles[i]->get_v1().z * cos(theta)));

        m_triangles[i]->set_v2(glm::vec3(m_triangles[i]->get_v2().x*cos(theta) - m_triangles[i]->get_v2().z * sin(theta),
            m_triangles[i]->get_v2().y,
             m_triangles[i]->get_v2().x*sin(theta) + m_triangles[i]->get_v2().z * cos(theta)));
    }
}

//Rotate object by given num degrees (rotate by)
void rotatez(float rot_by) const override{
    auto theta = degrees_to_radians(rot_by);
    for(int i = 0; i < m_triangles.size(); i++){

        float x = current_position.x;
        float y = current_position.y;
        float z = current_position.z;

        m_triangles[i]->set_v0(glm::vec3(m_triangles[i]->get_v0().x*cos(theta) - m_triangles[i]->get_v0().y * sin(theta),
            m_triangles[i]->get_v0().x*sin(theta) + m_triangles[i]->get_v0().y * cos(theta),
            m_triangles[i]->get_v0().z));

        m_triangles[i]->set_v1(glm::vec3(m_triangles[i]->get_v1().x*cos(theta) - m_triangles[i]->get_v1().y * sin(theta),
            m_triangles[i]->get_v1().x*sin(theta) + m_triangles[i]->get_v1().y * cos(theta),
        m_triangles[i]->get_v1().z)) ;

        m_triangles[i]->set_v2(glm::vec3(m_triangles[i]->get_v2().x*cos(theta) - m_triangles[i]->get_v2().y * sin(theta),
        m_triangles[i]->get_v2().x*sin(theta) + m_triangles[i]->get_v2().y * cos(theta),
        m_triangles[i]->get_v1().z));
    }
}

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

public:
    //Material of the Sphere
    shared_ptr<Material> mat_ptr;
    std::vector<shared_ptr<Triangle>> m_triangles;
    std::vector<glm::vec3> m_vertexList;
    glm::vec3 current_position;
};

// Bounding Volume requirement
bool TriangleMesh::bounding_box(double time0, double time1,
                            aabb &output_box) const {
  float min_x = std::numeric_limits<float>::max();
  float min_y = std::numeric_limits<float>::max();
  float min_z = std::numeric_limits<float>::max();
  float max_x = std::numeric_limits<float>::min();
  float max_y = std::numeric_limits<float>::min();
  float max_z = std::numeric_limits<float>::min();

  for (int i = 0; i < m_vertexList.size(); i++) {
      min_x = fmin(min_x, m_vertexList[i].x);
      min_y = fmin(min_y, m_vertexList[i].y);
      min_z = fmin(min_z, m_vertexList[i].z);
      max_x = fmax(max_x, m_vertexList[i].x);
      max_y = fmax(max_y, m_vertexList[i].y);
      max_z = fmax(max_z, m_vertexList[i].z);
  }

  output_box =
      aabb(glm::vec3(min_x,min_y,min_z),
           glm::vec3(max_x,max_y,max_z));
  return true;
}
#endif
