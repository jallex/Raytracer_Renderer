#ifndef Animation_HPP
#define Animation_HPP

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Transform.hpp"

// Animation class
class Animation {
    public:
        Animation(){
            transformPerFrame = std::vector<shared_ptr<Transform>>();
            num_frames = 0;
        }

        Animation(std::vector<shared_ptr<Transform>> t)
         : transformPerFrame(t) {
            num_frames = t.size();
         };

         void appendTransform(shared_ptr<Transform> t){
            transformPerFrame.push_back(t);
            num_frames += 1;
         }

         int get_num_frames(){
            return num_frames;
         }

         std::vector<shared_ptr<Transform>> get_transformPerFrame(){
            return transformPerFrame;
         }

    public:
        std::vector<shared_ptr<Transform>> transformPerFrame;
        int num_frames;
        int fps = 1;
};

#endif