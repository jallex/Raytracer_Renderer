#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ray.hpp"
#include "aabb.hpp"
#include "material.hpp"

// Keep track of data related to objects - hit data and material
struct hit_record {
    glm::vec3 p;
    glm::vec3 normal;
    float t;
    double u;
    double v;
    bool front_face;
    shared_ptr<Material> mat_ptr;

    inline void set_face_normal(const Ray& r, const glm::vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};

class Object {
    public:
        virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const = 0;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
        virtual void move(glm::vec3 new_pos) const {

    }
    virtual void rotatex(float new_rot) const {

    }
    virtual void rotatey(float new_rot) const {

    }
    virtual void rotatez(float new_rot) const {

    }
};

#endif
