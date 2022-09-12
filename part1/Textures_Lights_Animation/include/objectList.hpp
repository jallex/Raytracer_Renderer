#ifndef OBJECT_LIST_HPP
#define OBJECT_LIST_HPP

#include "aabb.hpp"
#include "object.hpp"

#include <iostream>
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

// List of Objects that can go in a scene
class ObjectList : public Object {
    public:
        ObjectList() {}
        ObjectList(shared_ptr<Object> object) { add(object); }
        //clear list of objects
        void clear() { objects.clear(); }
        //add object to list of objects
        void add(shared_ptr<Object> object) { objects.push_back(object); }

        void erase(int index){
            objects.erase(objects.begin() + index);
        }
        void add_at_index(int index, shared_ptr<Object> object) {
            objects.insert(objects.begin() + index, object);
        }
        // test if ray hit list of objects
        virtual bool hit(
            const Ray& r, float t_min, float t_max, hit_record& rec) const override;
        virtual bool bounding_box(
            double time0, double time1, aabb& output_box) const override;

        std::vector<shared_ptr<Object>> get_objects(){
            return objects;
        }

    public:
        std::vector<shared_ptr<Object>> objects;
};


// Bounding Volume requirement
bool ObjectList::bounding_box(double time0, double time1, aabb& output_box) const {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    for (const auto& object : objects) {
        if (!object->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

bool ObjectList::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;
    //iterate through objects, test if ray hit an object
    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif
