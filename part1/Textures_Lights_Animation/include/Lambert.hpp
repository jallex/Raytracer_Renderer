#ifndef LAMBERT_HPP
#define LAMBERT_HPP

#include "glm/glm.hpp"
#include "constants.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "vector3.hpp"
#include "object.hpp"
#include <iostream>
#include <memory>

//Lambertian material (Diffuse)
class Lambert : public Material {
    public:
        Lambert(const glm::vec3& a) : albedo(make_shared<solid_color>(a)) {}
        Lambert(shared_ptr<texture> a) : albedo(a) {}

        virtual bool scatter(
            const Ray& r_in, const hit_record& rec, glm::vec3& attenuation, Ray& scattered
        ) const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            // account for a scatter direction approaching zero
            if (near_zero(scatter_direction)){
                scatter_direction = rec.normal;
            }
            scattered = Ray(rec.p, scatter_direction);
            attenuation = albedo->value(rec.u, rec.v, rec.p);
            return true;
        }

    public:
        shared_ptr<texture> albedo;
};

#endif
