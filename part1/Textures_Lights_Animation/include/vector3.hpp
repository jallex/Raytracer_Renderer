#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include <random>

#include "constants.hpp"

// Utility Functions

inline static glm::vec3 random_vec3() {
    return glm::vec3(random_double(), random_double(), random_double());
}

inline static glm::vec3 random_vec3(double min, double max) {
    return glm::vec3(random_double(min,max), random_double(min,max), random_double(min,max));
}

glm::vec3 random_in_unit_sphere() {
    while (true) {
        auto p = random_vec3(-1,1);
        if (glm::length2(p) >= 1){
            continue;
        }
        return p;
    }
}

glm::vec3 random_unit_vector() {
    return glm::normalize(random_in_unit_sphere());
}

glm::vec3 random_in_hemisphere(const glm::vec3& normal) {
    glm::vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0){ // In the same hemisphere as the normal
        return in_unit_sphere;
    }
    else{
        return -in_unit_sphere;
    }
}

 bool near_zero(glm::vec3 vec) {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(vec.x) < s) && (fabs(vec.y) < s) && (fabs(vec.z) < s);
}

glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n) {
    return v - 2*dot(v,n)*n;
}

glm::vec3 refract(const glm::vec3 & uv, const glm::vec3 & n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    glm::vec3  r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    glm::vec3  r_out_parallel = -sqrt(fabs(1.0 - glm::length2(r_out_perp))) * n;
    return r_out_perp + r_out_parallel;
}

glm::vec3 random_in_unit_disk() {
    while (true) {
        auto p = glm::vec3(random_double(-1,1), random_double(-1,1), 0);
        if (glm::length2(p) >= 1) continue;
        return p;
    }
}

#endif