 #include "Triangle.hpp"

#include <iostream>

#include "globals.hpp"
#include "constants.hpp"
#include <iostream>
#include <string> 

bool Triangle::hit(const Ray &r, float t_min, float t_max, hit_record& rec) const {
   //Step 1. Does ray hit on the plane that the triangle is on? 
    // Find normal of triangle 
    glm::vec3 a = v1 - v0; //edge 0
    glm::vec3 b = v2 - v0; // edge 1
    glm::vec3 c = glm::cross(a, b); // this is the triangle's normal
    glm::vec3 normal = glm::normalize(c); //normalize normal

    // If plane and ray are parallel, return false
    float normaldot_raydir = glm::dot(normal, r.direction());
    if(fabs(normaldot_raydir) < std::pow(2.0, -52.0)) // Almost 0
    {
        //normal is perpendicular to ray, meaning ray and triangle are parallel.
        return false;
    }

    // Compute plane equation to find triangle's plane
    float D = -glm::dot(normal, v0);
    // Plane equation Ax + By + Cz + D = 0
    float t = -(glm::dot(normal, r.origin()) + D) / glm::dot(normal, r.direction());

    //Check if triangle is in behind ray
    if(t < 0){
        return false;
    }

    //Compute intersection point
    glm::vec3 P = r.origin() + t * r.direction();


    // Step 2 : Does ray hit within this triangle?
    //Using inside-outside test
    glm::vec3 C; //vector perpendicular to triangle's plane

    //edge 0
    glm::vec3 edge0 = v1 - v0;
    glm::vec3 vp0 = P - v0;
    C = glm::cross(edge0, vp0);
    if(glm::dot(normal, C) < 0){
		return false; // P is on the right side
    }

    //edge 1
    glm::vec3 edge1 = v2 - v1;
    glm::vec3 vp1 = P - v1;
    C = glm::cross(edge1, vp1);
    if(glm::dot(normal, C) < 0){
		return false; // P is on the right side
    }

    //edge 2
    glm::vec3 edge2 = v0 - v2;
    glm::vec3 vp2 = P - v2;
    C = glm::cross(edge2, vp2);
    if(glm::dot(normal, C) < 0){
        return false; // P is on the right side
    }

    glm::vec3 pvec = glm::cross(r.direction(), b);
    double det = glm::dot(a, pvec); 
    double invDet = 1 / det;
    glm::vec3 tvec = r.origin() - v0;
    glm::vec3 qvec = glm::cross(tvec, b); 
    double t2 = glm::dot(b, qvec) * invDet;
 
    // For multiple objects, we take the closest one
    if (t < t_min || t_max < t)
    {
        return false;
    }
    
    // record hit data
    rec.t = t2;
    rec.p = r.at(rec.t);
    rec.normal = normal;
    glm::vec3 outward_normal = normal;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    OBJ_HITS++;
   return true;
}
