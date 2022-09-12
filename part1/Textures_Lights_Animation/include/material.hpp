#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "constants.hpp"
#include "texture.hpp"

struct hit_record;

class Material {
    public:
        virtual glm::vec3 emitted(double u, double v, const glm::vec3& p) const {
            return glm::vec3(0,0,0);
        }
        virtual bool scatter(
            const Ray& r_in, const hit_record& rec, glm::vec3& attenuation, Ray& scattered
        ) const = 0;
};

class diffuse_light : public Material  {
    public:
        diffuse_light(shared_ptr<texture> a) : emit(a) {}
        diffuse_light(glm::vec3 c) : emit(make_shared<solid_color>(c)) {}

        virtual bool scatter(
            const Ray& r_in, const hit_record& rec, glm::vec3& attenuation, Ray& scattered
        ) const override {
            return false;
        }

        virtual glm::vec3 emitted(double u, double v, const glm::vec3& p) const override {
            return emit->value(u, v, p);
        }

    public:
        shared_ptr<texture> emit;
};
#endif
