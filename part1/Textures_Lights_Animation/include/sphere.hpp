#ifndef sphere_HPP
#define sphere_HPP

#include "glm/detail/type_vec.hpp"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ray.hpp"
#include "object.hpp"
#include "material.hpp"

// Sphere class
class Sphere: public Object {
    public:
        Sphere();
        Sphere(glm::vec3 cen, float r, shared_ptr<Material> m)
         : center(cen), radius(r), mat_ptr(m) {};

        virtual bool hit(
            const Ray& r, float t_min, float t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        glm::vec3 center;
        float radius;
        //Material of the Sphere
        shared_ptr<Material> mat_ptr;

    private:
        static void get_sphere_uv(const glm::vec3& p, double& u, double& v) {
            // p: a given point on the sphere of radius one, centered at the origin.
            // u: returned value [0,1] of angle around the Y axis from X=-1.
            // v: returned value [0,1] of angle from Y=-1 to Y=+1.
            //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
            //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
            //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

            auto theta = acos(-p.y);
            auto phi = atan2(-p.z, p.x) + pi;

            u = phi / (2*pi);
            v = theta / pi;
        }
};

inline bool Sphere::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = aabb(
        center - glm::vec3(radius, radius, radius),
        center + glm::vec3(radius, radius, radius));
    return true;
}
#endif
