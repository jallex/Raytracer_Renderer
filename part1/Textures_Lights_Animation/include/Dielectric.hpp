#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include "constants.hpp"
#include "material.hpp"
#include "vector3.hpp"
#include "object.hpp"


//Dielectric Material
class Dielectric : public Material {
    public:
        Dielectric(float index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(
            const Ray& r_in, const hit_record& rec, glm::vec3& attenuation, Ray& scattered
        ) const override {
            attenuation = glm::vec3(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

            glm::vec3 unit_direction = normalize(r_in.direction());
            float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            float sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            glm::vec3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = Ray(rec.p, direction);
            return true;
        }

    public:
        float ir; // index of refraction

    private:
        static float reflectance(float cosine, float ref_idx) {
            // Use Schlick's approximation for reflectance
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }
};

#endif