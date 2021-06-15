#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"

struct Point
{
  int x;
  int y;
};
struct Vect3f{
    float x;
    float y;
    float z;
};
float dot(Vect3f a,Vect3f b);
float distance(Vect3f a,Vect3f b);
Vect3f diff(Vect3f a,Vect3f b);
Vect3f normalize(Vect3f a);
Vect3f sum(Vect3f a,Vect3f b);
Vect3f scalar(Vect3f ,float alpha);
struct Ray{
    Vect3f pos;
    Vect3f dir;
    
};
struct Hit{
    bool exist;
    float t;
    float reflection;
    float refraction;
    Pixel color; 
    Vect3f normal;
};
class Floor{
public:
        bool ray_intersect_Floor(const Ray&ray,float&t);
        Vect3f get_normal(){return normal;};
        Floor(Vect3f n_point,Vect3f n_normal,Pixel n_color,float n_reflection,float n_refraction);
        Pixel get_color(){return color;};
        float get_reflection(){return reflection;};
        float get_refraction(){return refraction;};
private:
        Vect3f point;
        Vect3f normal;
        Pixel color;
        float reflection;
        float refraction;
};
class Box{
public:
    Box(Vect3f n_boxMax,Vect3f n_boxMin,Pixel n_color,float n_reflection,float n_refraction);
    bool ray_intersect_Box(const Ray&ray,float &t);
    Pixel get_color(){return color;};
    Vect3f get_normal(Vect3f hit_point);
    float get_reflection(){return reflection;};
    float get_refraction(){return refraction;};
private:
    Vect3f boxMax;
    Vect3f boxMin;
    Pixel color;
    float reflection;
    float refraction;
};
class Sphere{
public:
    Sphere(float n_r,Vect3f n_centr,Pixel n_color,float n_reflection,float n_refraction);
    bool ray_intersect_Sphere(const Ray&ray,float&t);
    Pixel get_color(Vect3f hit_point);
    Vect3f get_centr(){return centr;};
    Vect3f get_normal(Vect3f hit_point);
    float get_reflection(Vect3f hit_point);
    float get_refraction(Vect3f hit_point);
private:
        float r;
        float reflection;
        float refraction;
        Vect3f centr;
        Pixel color;
  
        
};
class Canvas{
public:
    
    Canvas(Vect3f n_eye,Vect3f n_canvas_locate,Vect3f n_canvas_normal,float a_width,float a_height,Sphere*a_sphere,Box *a_box,Floor* n_floor,Floor*n_wall,Vect3f n_light1,Vect3f n_light2);
    void Render(Image&picture,int canvas_width,int canvas_height);
    Hit RaySceneIntersection(const Ray& ray);
    Pixel RayTrace(const Ray& ray);
    Ray Pixel_to_Ray(int i, int j,int canvas_width,int canvas_height);
    bool Visible(Vect3f hit_point,Vect3f light);
    float Shade(Vect3f hit_point,Vect3f normal,Vect3f light);
    Ray reflect(Ray ray,Hit hit);
    Ray refract(Ray ray,Hit hit,bool &flag);
    float Schlick(Ray ray,Hit hit,Vect3f light);
private:
    Vect3f eye;
    Vect3f canvas_locate;
    Vect3f canvas_normal;
    float width;
    float height;
    Sphere* sphere;
    Box* box;
    Vect3f light1;
    Vect3f light2;
    Floor*floor;
    Floor*wall;
    int deep;
};

#endif //MAIN_PLAYER_H
