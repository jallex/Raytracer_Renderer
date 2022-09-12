#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "globals.hpp"
#include "sphere.hpp"
#include "object.hpp"
#include <iostream>
#include <fstream>
#include <string> 
#include <sstream>

//Sphere Constructor 
Sphere::Sphere(){
}

// Test if ray hit Sphere
bool Sphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
    glm::vec3 oc = r.origin() - center;
    float a = glm::length2(r.direction());
    float half_b = dot(oc, r.direction());
    float c = glm::length2(oc) - radius*radius;

    float discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    float sqrtd = sqrt(discriminant);

    // Find the nearest root within range based on quadratic equation if b=2h
    float root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    // record hit data
    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;
    glm::vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    OBJ_HITS++;
    return true;
}

