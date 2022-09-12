// Bounding Volume requirement
#ifndef BVH_H
#define BVH_H


#include "globals.hpp"
#include "object.hpp"
#include "objectList.hpp"

#include <algorithm>

class bvh_node : public Object {
    public:
        bvh_node();

        bvh_node(const  ObjectList& list, double time0, double time1)
            : bvh_node(list.objects, 0, list.objects.size(), time0, time1)
        {}

        bvh_node(
            const std::vector<shared_ptr<Object>>& src_objects,
            size_t start, size_t end, double time0, double time1);

        virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        shared_ptr<Object> left;
        shared_ptr<Object> right;
        aabb box;
};

bool bvh_node::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = box;
    return true;
}

bool bvh_node::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
    if (!box.hit(r, t_min, t_max))
        return false;

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    auto result = hit_left || hit_right;
    if (result) {
        BOUND_HITS++;
    }
    return result;
}

inline int random_int(int min, int max) {
    // Returns a random integer in [min,max].
    return static_cast<int>(random_double(min, max+1));
}

inline bool box_compare(const shared_ptr<Object> a, const shared_ptr<Object> b, int axis) {
    aabb box_a;
    aabb box_b;

    if (!a->bounding_box(0,0, box_a) || !b->bounding_box(0,0, box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";
    switch (axis) {
        case 0:
            return box_a.min().x < box_b.min().x;
        case 1:
            return box_a.min().y < box_b.min().y;
        case 2:
            return box_a.min().z < box_b.min().z;
        default:
            return false;
    }
}


bool box_x_compare (const shared_ptr<Object> a, const shared_ptr<Object> b) {
    return box_compare(a, b, 0);
}

bool box_y_compare (const shared_ptr<Object> a, const shared_ptr<Object> b) {
    return box_compare(a, b, 1);
}

bool box_z_compare (const shared_ptr<Object> a, const shared_ptr<Object> b) {
    return box_compare(a, b, 2);
}

bvh_node::bvh_node(
    const std::vector<shared_ptr<Object>>& src_objects,
    size_t start, size_t end, double time0, double time1
) {

    auto objects = src_objects; // Create a modifiable array of the source scene objects

    int axis = random_int(0,2);
    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start+1])) {
            left = objects[start];
            right = objects[start+1];
        } else {
            left = objects[start+1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span/2;
        left = make_shared<bvh_node>(objects, start, mid, time0, time1);
        right = make_shared<bvh_node>(objects, mid, end, time0, time1);
    }

    aabb box_left, box_right;

    if (  !left->bounding_box (time0, time1, box_left)
       || !right->bounding_box(time0, time1, box_right)
    )
        std::cerr << "No bounding box in bvh_node constructor.\n";

    box = surrounding_box(box_left, box_right);
}
#endif
