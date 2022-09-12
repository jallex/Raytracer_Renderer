#ifndef scene_HPP
#define scene_HPP

#include <iostream>

#include "ray.hpp"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "object.hpp"
#include "Animation.hpp"

class Scene {
    public:
        Scene(int num_f){
            num_hits = 0;
            listObjectAnimations = std::vector<shared_ptr<Animation>>();
            num_frames = num_f;
        }

        Scene(){
            num_hits = 0;
            listObjectAnimations = std::vector<shared_ptr<Animation>>();
            num_frames = 0;
        }
        ~Scene(){}

        void set_num_hits(int num){
            num_hits = num;
        }

        int get_num_hits(){
            return num_hits;
        }

        glm::vec3 getColorFromRayInScene(const Ray& r, const glm::vec3& background, const Object& world, int depth){
    hit_record rec;
    if (depth <= 0){
        return glm::vec3(0,0,0);
    }

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    Ray scattered;
    glm::vec3 attenuation;
    glm::vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * getColorFromRayInScene(scattered, background, world, depth-1);
}

    void addAnimation(shared_ptr<Animation> a){
        listObjectAnimations.push_back(a);
    }

    std::vector<shared_ptr<Animation>> getAnimations(){
        return listObjectAnimations;
    }

    int get_num_frames(){
        return num_frames;
    }
    
    private:
    int num_hits;
    //List of object animations that match up to object by index
    std::vector<shared_ptr<Animation>> listObjectAnimations;
    int num_frames;
};

#endif

