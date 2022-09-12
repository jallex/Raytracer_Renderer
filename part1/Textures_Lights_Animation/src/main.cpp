#include "aarect.h"
#include "bvh.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/detail/type_vec.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include <iostream>
#include <fstream>
#include <memory>
#include <string> 
#include <sstream>

#include "globals.hpp"
#include "constants.hpp"
#include "objectList.hpp"
#include "Camera.hpp"
#include "vector3.hpp"
#include "Metal.hpp"
#include "Lambert.hpp"
#include "Dielectric.hpp"
#include "Triangle.hpp"
#include "ObjFile.hpp"
#include "Triangle_Mesh.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <chrono>
using namespace std::chrono;

// Generate a scene with random spheres
ObjectList random_scene() {
    ObjectList world;

    auto checker = make_shared<checker_texture>(glm::vec3(0.2, 0.3, 0.1), glm::vec3(100.9, 100.9, 100.9));
    world.add(make_shared<Sphere>(glm::vec3(0,-1000,0), 1000, make_shared<Lambert>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            glm::vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_vec3() * random_vec3();
                    sphere_material = make_shared<Lambert>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random_vec3(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(glm::vec3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambert>(glm::vec3(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(glm::vec3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(glm::vec3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(glm::vec3(4, 1, 0), 1.0, material3));

    return world;
}

ObjectList earth() {
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<Lambert>(earth_texture);
    auto globe = make_shared<Sphere>(glm::vec3(0,0,0), 0.5, earth_surface);

    ObjectList obj;
    obj.add(globe);

    return obj;
}

ObjectList simple_light() {
    ObjectList objects;

    auto pertext = make_shared<image_texture>("earthmap.jpg");
    auto pertext2 = make_shared<image_texture>("images.jpg");
    auto mat_lambert1 = make_shared<Lambert>(glm::vec3(0.7, 0.3, 0.3));

    auto difflight = make_shared<diffuse_light>(glm::vec3(0,1,1));
    objects.add(make_shared<Sphere>(glm::vec3(2,1,0), 1, difflight));

    auto difflight2 = make_shared<diffuse_light>(glm::vec3(1,1,0));
    objects.add(make_shared<Sphere>(glm::vec3(-2,1,0), 1, difflight2));

    auto material3 = make_shared<Metal>(glm::vec3(0.7, 0.0, 0.0), 0.0);

    ObjFile my_file = ObjFile("./../../common/objects/teapot.obj");
    objects.add(make_shared<Sphere>(glm::vec3(0,1,0), 1, make_shared<Lambert>(pertext2)));

    objects.add(make_shared<Sphere>(glm::vec3(0,-1000,0), 1000, make_shared<Lambert>(pertext)));

    auto mat_metal1   = make_shared<Metal>(glm::vec3(0.9, 0.2, 0.3), 0.3);
    auto difflight3 = make_shared<diffuse_light>(glm::vec3(1,1,0));
    objects.add(make_shared<Sphere>(glm::vec3(10,10,0), 1, difflight));

    return objects;
}

// Generate a scene with purple and blue colored spheres
ObjectList my_scene() {
    ObjectList world;

     // Materials
    auto material_ground = make_shared<Lambert>(glm::vec3(0.3, 0.3, 0.8));
    auto mat_lambert1 = make_shared<Lambert>(glm::vec3(0.7, 0.3, 0.3));
    auto mat_lambert2 = make_shared<Lambert>(glm::vec3(0.7, 0.3, 0.9));
    // Reflection 
    auto mat_metal1   = make_shared<Metal>(glm::vec3(0.9, 0.2, 0.3), 0.3);
    auto mat_metal2   = make_shared<Metal>(glm::vec3(0.3, 0.3, 0.8), 0.3);
    auto mat_metal3   = make_shared<Metal>(glm::vec3(0.8, 0.2, 0.8), 0.2);
    auto mat_dialectric1  = make_shared<Dielectric>(1.5);
    auto mat_dialectric2  = make_shared<Dielectric>(.8);

    // Ground
    world.add(make_shared<Sphere>(glm::vec3( 0.0, -100.5, 1.0), 100.0, material_ground));

    // Sphere 
    world.add(make_shared<Sphere>(glm::vec3( 0.0, 0.8, -2.0), .665, mat_lambert1));
    world.add(make_shared<Sphere>(glm::vec3(-1.0, 1.2, -3.0), .2, mat_metal1));
    world.add(make_shared<Sphere>(glm::vec3(0.55, 1.2, -3.0), .4, mat_metal2));
    world.add(make_shared<Sphere>(glm::vec3(.7, 0.2, 1.0), .1, mat_metal3));
    world.add(make_shared<Sphere>(glm::vec3( -1.0, 0.0, -1.0), .5, mat_dialectric1));
    world.add(make_shared<Sphere>(glm::vec3( -.7, 0.0, -4.0), .4, mat_lambert2));
    world.add(make_shared<Sphere>(glm::vec3( 1.5, .2, -1.0), .3, mat_dialectric2));
    return world;
}

ObjectList dof_Scene(){
  ObjectList world;

    auto material_ground = make_shared<Lambert>(glm::vec3(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambert>(glm::vec3(0.1, 0.2, 0.5));
    auto material_left   = make_shared<Dielectric>(1.5);
    auto material_right  = make_shared<Metal>(glm::vec3(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<Sphere>(glm::vec3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(glm::vec3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<Sphere>(glm::vec3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<Sphere>(glm::vec3(-1.0,    0.0, -1.0), -0.45, material_left));
    world.add(make_shared<Sphere>(glm::vec3( 1.0,    0.0, -1.0),   0.5, material_right));

    return world;

}

// Generate a scene with a single triangle
ObjectList single_triangle_Scene(){
  ObjectList world;

  auto material_ground = make_shared<Lambert>(glm::vec3(1.0, 0.0, 0.0));
  auto material_right  = make_shared<Metal>(glm::vec3(0.8, 0.6, 0.2), 0.0);

  Vertex v0 = Vertex(glm::vec3(0,0,0));
  Vertex v1 = Vertex(glm::vec3(0,3,0));
  Vertex v2 = Vertex(glm::vec3(3,0,0));
  world.add(make_shared<Triangle>(v0, v1, v2, material_right));

    return world;
}


//Generate a scene with a bunny
ObjectList bunny_scene(){
  ObjectList world;

  ObjectList world_bvh;
  // Materials
    auto material_ground = make_shared<Lambert>(glm::vec3(0.3, 0.3, 0.8));
    auto mat_lambert1 = make_shared<Lambert>(glm::vec3(0.7, 0.3, 0.3));
    auto mat_lambert2 = make_shared<Lambert>(glm::vec3(0.7, 0.3, 0.9));
    // Reflection 
    auto mat_metal1   = make_shared<Metal>(glm::vec3(0.9, 0.2, 0.3), 0.3);
    auto mat_metal2   = make_shared<Metal>(glm::vec3(0.3, 0.3, 0.8), 0.3);
    auto mat_metal3   = make_shared<Metal>(glm::vec3(0.8, 0.2, 0.8), 0.2);
    auto mat_metal4   = make_shared<Metal>(glm::vec3(0.3, 0.8, 0.9), 0.7);
    auto mat_dialectric1  = make_shared<Dielectric>(1.5);
    auto mat_dialectric2  = make_shared<Dielectric>(.8);

  auto material_right  = make_shared<Metal>(glm::vec3(0.8, 0.6, 0.2), 0.0);
  world.add(make_shared<Sphere>(glm::vec3(-1.0, 1.2, -3.0), .2, mat_metal1));
    world.add(make_shared<Sphere>(glm::vec3(0.55, 1.2, -3.0), .4, mat_metal2));

    world.add(make_shared<Sphere>(glm::vec3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(glm::vec3( -1.0, 0.0, -1.0), .5, mat_dialectric1));
    world.add(make_shared<Sphere>(glm::vec3( 1.5, .2, -1.0), .3, mat_dialectric2));

  ObjFile my_file = ObjFile("./../../common/objects/bunny_centered.obj");
  world.add(make_shared<TriangleMesh>(my_file, mat_metal4));

  world_bvh.add(make_shared<bvh_node>(world, 0, 1));
    return world_bvh;
}


//Generate a scene with a bunny
ObjectList test_animation(){
  ObjectList world;

  ObjectList world_bvh;

  auto mat_metal4   = make_shared<Metal>(glm::vec3(0.3, 0.8, 0.9), 0.7);

  ObjFile my_file = ObjFile("./../../common/objects/bunny_centered.obj");
  world.add(make_shared<TriangleMesh>(my_file, mat_metal4));
  world.add(make_shared<bvh_node>(world, 0, 1));
    return world;
}

//Generate a scene with a bunny
ObjectList test_animation2(){
  ObjectList world;

  ObjectList world_bvh;

 auto mat_metal4   = make_shared<Metal>(glm::vec3(0.3, 0.8, 0.9), 0.7);

    // world.add(make_shared<Sphere>(glm::vec3( 0.0, -100.5, -1.0), 100.0, material_ground));

  ObjFile my_file = ObjFile("./../../common/objects/bunny_centered.obj");
  world.add(make_shared<TriangleMesh>(my_file, mat_metal4));
  world.objects[0]->rotatex(90.0);
  world.add(make_shared<bvh_node>(world, 0, 1));
    return world;
}

Scene test_animation3_scene(){
Scene myScene = Scene(3);

//Animation
  std::vector<shared_ptr<Transform>> transformPerFrame;
  transformPerFrame.push_back(make_shared<Transform>(glm::vec3(0.0, 0.25, 0.0), glm::vec3(0.0, 0.0, 0.0)));
  transformPerFrame.push_back(make_shared<Transform>(glm::vec3(0.0, 0.5, 0.0), glm::vec3(0.0, 0.0, 0.0)));
  transformPerFrame.push_back(make_shared<Transform>(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 0.0)));

  myScene.addAnimation(make_shared<Animation>(transformPerFrame));

return myScene;
}

//Generate a scene with a bunny
ObjectList test_animation3(){
  ObjectList world;

  ObjectList world_bvh;

 auto mat_metal4   = make_shared<Metal>(glm::vec3(0.3, 0.8, 0.9), 0.7);

  ObjFile my_file = ObjFile("./../../common/objects/bunny_centered.obj");
  world.add(make_shared<TriangleMesh>(my_file, mat_metal4));
  world.add(make_shared<bvh_node>(world, 0, 1));
    return world;
}

// Render out a ppm given a scene
void sceneToPPM(std::string outputFileName, Scene scene, ObjectList world, Camera cam, int image_width, int image_height, int samples_per_pixel, int max_depth) {
    int num_rays = 0;
    // Render
    std::ofstream outFile; 
    outFile.open(outputFileName);
    outFile << "P3" << std::endl;
    outFile << image_width << " " << image_height << std::endl;
    outFile << 255 << std::endl;
    std::cout << "Countdown till done \n";
    // Iterate through each pixel, and find color at that pixel
    for(int j = image_height - 1; j >= 0; j--){
        for(int i = 0; i < image_width; i++){
            //Antialiasing Requirement
            glm::vec3 pixel_color(0, 0, 0);
            // Get the proper number of samples
            for (int s = 0; s < samples_per_pixel; ++s) {
                num_rays += 1;
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                Ray ray = cam.get_ray(u, v);
                pixel_color += scene.getColorFromRayInScene(ray, glm::vec3(0.0,0.0,0.0), world, max_depth);
            }

            // Gamma
            // Divide the color by the number of samples and gamma-correct for gamma=2.0.
            auto scale = 1.0 / samples_per_pixel;
            float r = sqrt(scale * pixel_color.x);
            float g = sqrt(scale * pixel_color.y);
            float b = sqrt(scale * pixel_color.z);
           // Write color to PPM
            outFile << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << std::endl;
            outFile << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << std::endl;
            outFile << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << std::endl;
        }
         std::cout << j << "\n";
    }
    std::cout << "Rays Cast: " << num_rays << "\n";
    std::cout << "Bounding Volume Intersections: " << BOUND_HITS << "\n";
    std::cout << "Successful Object Intersections: " << OBJ_HITS << "\n";
    outFile.close();
}

int main(){

  auto start = high_resolution_clock::now();

  auto world = simple_light();

  Scene myScene = test_animation3_scene();

   // Image
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1080;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 900;
    int max_depth = 100;

    // //Camera custom scene
    // double R = cos(pi/4);
    // glm::vec3 lookfrom(.5,1,6);
    // glm::vec3 lookat(0,.5,0);
    // glm::vec3 vup(0,1,0);
    // double dist_to_focus = 0.1;
    // double aperture = 0.1;

    // Camera fov scene
    glm::vec3 lookfrom(0,3,16);
    glm::vec3 lookat(0,1.5,0);
    glm::vec3 vup(0,1,0);
    double dist_to_focus = glm::length(lookfrom-lookat);
    double aperture = 0.05;

    // Camera random scene
    // glm::vec3 lookfrom3(13,2,3);
    // glm::vec3 lookat3(0,0,0);
    // glm::vec3 vup3(0,1,0);
    // double dist_to_focus3 = glm::length(lookfrom3-lookat3);
    // double aperture3 = 0.05;

    // Camera triangle scene
    // glm::vec3 lookfrom4(0,0,3);
    // glm::vec3 lookat4(0,0,0);
    // glm::vec3 vup4(0,1,0);
    // double dist_to_focus4 = 10;
    // double aperture4 = 0.3;

    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, dist_to_focus, aperture);
    // View Requirement
    //lookfrom, lookat, vup, vfov, vertical fov, double aspect_ratio, double aperture, double focus_dist 
    cam.setCameraOrientation(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    glm::vec3 origin = cam.get_origin();
    float x = origin.x;
    float y = origin.y;
    float z = origin.z;
    float b = 0.15;
    x = x*cos(b) - z * sin(b);
    z = x*sin(b) + z * cos(b);    

    std::cout << "Num frames " << myScene.get_num_frames() << "\n";
    auto scene_animations = myScene.getAnimations();
    //iterate through frames
    for(int i = 0; i < myScene.get_num_frames(); i++){
        //iterate through object animations (one per object)
        for(int j = 0; j < scene_animations.size(); j++){
            //Animation : List of animations from Scene for this object
            auto object_animation = scene_animations[j];
            // Get list of object transform for this object
            auto object_on_frames_animation = object_animation->get_transformPerFrame();
            //Get transform on this frame
            auto transform_on_frame = object_on_frames_animation[i];
            world.objects[j]->move(transform_on_frame->get_pos());
            glm::vec3 new_rot = transform_on_frame->get_rot();

             world.objects[j]->rotatex(new_rot.x);
             world.objects[j]->rotatey(new_rot.y);
             world.objects[j]->rotatez(new_rot.z);
        }
        sceneToPPM("frame" + std::to_string(i) + ".ppm", myScene, world, cam, image_width, image_height, samples_per_pixel, max_depth);
    }

  // Record how many seconds it took to render
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<seconds>(stop - start);
  return 0;
}
