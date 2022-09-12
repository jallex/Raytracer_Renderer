#ifndef ray_HPP
#define ray_HPP

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Ray
{
public:
    Ray() {}
    Ray(const glm::vec3 &o, const glm::vec3 &d)
        : orig(o), dir(d)
    {
    }

    glm::vec3 origin() const { return orig; }
    glm::vec3 direction() const { return dir; }

    glm::vec3 at(float t) const
    {
        return orig + dir * t;
    }

private:
    // Ray origin
    glm::vec3 orig;
    // Ray direction
    glm::vec3 dir;
};

#endif
