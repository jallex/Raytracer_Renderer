#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "glm/gtx/extented_min_max.hpp"
#include "object.hpp"
#include "constants.hpp"

struct Vertex {
public:
    Vertex(glm::vec3 coord){
        position = coord;
    }
    glm::vec3 position;

};

class Triangle : public Object {
public:
    Triangle(Vertex vertex0, Vertex vertex1, Vertex vertex2, shared_ptr<Material> m) {

        v0 = vertex0.position;
        v1 = vertex1.position;
        v2 = vertex2.position;
        e1 = v1 - v0;
        e2 = v2 - v0;
        n = glm::normalize(glm::cross(e1, e2));

        mat_ptr = m;
    }

    virtual bool hit(
            const Ray& r, float t_min, float t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

     void set_v0(glm::vec3 v){
        v0 = v;
    }

    void set_v1(glm::vec3 v){
        v1 = v;
    }

    void set_v2(glm::vec3 v){
        v2 = v;
    }

    glm::vec3 get_v0(){
        return v0;
    }

    glm::vec3 get_v1(){
        return v1;
    }

    glm::vec3 get_v2(){
        return v2;
    }


public:
    glm::vec3 v0, v1, v2;
    glm::vec3 e1, e2;
    glm::vec3 n;
    glm::vec3 normal[3];
    glm::vec3 uv[3];
    //Material of the Sphere
    shared_ptr<Material> mat_ptr;
};

// Bounding Volume requirement
inline bool Triangle::bounding_box(double time0, double time1,
                            aabb &output_box) const {
  output_box =
      aabb(glm::vec3(fmin(v0.x, fmin(v1.x, v2.x)),
                     fmin(v0.y, fmin(v1.y, v2.y)),
                     fmin(v0.z, fmin(v1.z, v2.z))),
           glm::vec3(fmax(v0.x, fmax(v1.x, v2.x)),
                     fmax(v0.y, fmax(v1.y, v2.y)),
                     fmax(v0.z, fmax(v1.z, v2.z))));
  return true;
}
#endif
