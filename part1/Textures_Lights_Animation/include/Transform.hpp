#ifndef Transform_HPP
#define Transform_HPP

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Transform class
class Transform {
    public:
        Transform();
        Transform(glm::vec3 pos, glm::vec3 rot)
         : position(pos), rotation(rot) {};

         glm::vec3 get_pos(){
            return position;
         }

         glm::vec3 get_rot(){
            return rotation;
         }

    public:
        glm::vec3 position;
        glm::vec3 rotation;
};

#endif