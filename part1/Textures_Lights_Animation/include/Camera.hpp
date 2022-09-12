#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "constants.hpp"
#include "vector3.hpp"
#include <iostream>

// Camera requirement
class Camera {
    public:
        Camera() {
            //Default Camera
            auto aspect_ratio = 16.0 / 9.0;
            auto viewport_height = 2.0;
            auto viewport_width = aspect_ratio * viewport_height;
            auto focal_length = 1.0;

            origin = glm::vec3(0, 0, 0);
            horizontal = glm::vec3(viewport_width, 0.0, 0.0);
            vertical = glm::vec3(0.0, viewport_height, 0.0);
            lower_left_corner = origin - horizontal/2 - vertical/2 - glm::vec3(0, 0, focal_length);
        }
        Camera(
            glm::vec3 lookfrom,
            glm::vec3 lookat,
            glm::vec3   vup,
            double vfov, // vertical fov in degrees
            double aspect_ratio,
            double aperture,
            double focus_dist //Depth of Field Requirement
        ) {
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;
            // get unit vector
             w = glm::normalize(lookfrom - lookat);
             u = glm::normalize(cross(vup, w));
             v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

            lens_radius = aperture / 2;
        }

        Ray get_ray(float s, float t) const {
            glm::vec3 rd = lens_radius * random_in_unit_disk();
            glm::vec3 offset = u * rd.x + v * rd.y;

            return Ray(
                origin + offset,
                lower_left_corner + s*horizontal + t*vertical - origin - offset
            );
        }

        // Allow Camera to be re-positioned
        void setCameraOrientation(glm::vec3 lookfrom,
            glm::vec3 lookat,
            glm::vec3   vup,
            float vfov, // vertical fov in degrees
            double aspect_ratio,
            double aperture,
            double focus_dist) {
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;
            // get unit vector
             w = glm::normalize(lookfrom - lookat);
             u = glm::normalize(cross(vup, w));
             v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

            lens_radius = aperture / 2;
        }

        glm::vec3 get_origin(){
            return origin;
        }
        glm::vec3 get_lower_left_corner(){
            return lower_left_corner;
        }
        glm::vec3 get_horizontal(){
            return horizontal;
        }
        glm::vec3 get_vertical(){
            return vertical;
        }
        glm::vec3 get_w(){
            return w;
        }
        glm::vec3 get_v(){
            return v;
        }
        glm::vec3 get_u(){
            return u;
        }
        float get_lens_radius(){
            return lens_radius;
        }


    private:
        glm::vec3 origin;
        glm::vec3 lower_left_corner;
        glm::vec3 horizontal;
        glm::vec3 vertical;
        glm::vec3 u, v, w;
        float lens_radius;
};
#endif