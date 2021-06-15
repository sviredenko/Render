#include "Canvas.h"
#include<math.h>
#include<iostream>

float dot(Vect3f a,Vect3f b){
    float res=a.x*b.x+a.y*b.y+a.z*b.z;
    return res;
}
Vect3f normalize(Vect3f a){
        float sum=sqrtf(a.x*a.x+a.y*a.y+a.z*a.z);
        if(sum>0){
            Vect3f res{a.x/sum,a.y/sum,a.z/sum};
            return res;
        }
        return a;
}
Vect3f sum(Vect3f a,Vect3f b){
    Vect3f res{a.x+b.x,a.y+b.y,a.z+b.z};
    return res;
}
Vect3f scalar(Vect3f a,float alpha){
    Vect3f res{a.x*alpha,a.y*alpha,a.z*alpha};
    return  res;  
}
float distance(Vect3f a,Vect3f b){
    Vect3f c=diff(a,b);
    float res=sqrtf(c.x*c.x+c.y*c.y+c.z*c.z);
    return res;
}
Vect3f diff(Vect3f a,Vect3f b){
    Vect3f res;
    res.x=a.x-b.x;
    res.y=a.y-b.y;
    res.z=a.z-b.z;
    return res;
}
Box::Box(Vect3f n_boxMax,Vect3f n_boxMin,Pixel n_color,float n_reflection,float n_refraction){
    boxMax=n_boxMax;
    boxMin=n_boxMin;
    color=n_color;
    reflection=n_reflection;
    refraction =n_refraction;
}
Floor::Floor(Vect3f n_point,Vect3f n_normal,Pixel n_color,float n_reflection,float n_refraction){
    point=n_point;
    normal=n_normal;
    color=n_color;
    reflection=n_reflection;
    refraction =n_refraction;
}
bool Floor::ray_intersect_Floor(const Ray&ray,float&t){
    float k=dot(ray.dir,normal);
    if(fabs(k)<0.00001)
        return false;
    t=dot(diff(point,ray.pos),normal)/k;
   
    return (t>0.00001f) ;
}
bool Box::ray_intersect_Box(const Ray&ray,float &t){
    float tmin;
    float tmax;
    
    float lo=(boxMin.x-ray.pos.x)/ray.dir.x;
    float hi=(boxMax.x-ray.pos.x)/ray.dir.x;
    tmin=std::min(lo,hi);
    tmax=std::max(lo,hi);
    
    float lo1=(boxMin.y-ray.pos.y)/ray.dir.y;
    float hi1=(boxMax.y-ray.pos.y)/ray.dir.y;
    tmin=std::max(tmin,std::min(lo1,hi1));
    tmax=std::min(tmax,std::max(lo1,hi1));
    
    float lo2=(boxMin.z-ray.pos.z)/ray.dir.z;
    float hi2=(boxMax.z-ray.pos.z)/ray.dir.z;
    tmin=std::max(tmin,std::min(lo2,hi2));
    tmax=std::min(tmax,std::max(lo2,hi2));
    t=tmin;
    if(t<0.1){
        t=100;
        return false;
    }
    return (tmin<=tmax) && (tmax>0.1f);
}
Vect3f Box::get_normal(Vect3f hit_point){
        float eps=0.0001;
        Vect3f norm{0,0,0};
        if(fabs(boxMin.x-hit_point.x)<eps)
            norm.x=-1;
        if(fabs(boxMin.y-hit_point.y)<eps)
             norm.y=1;
        if(fabs(boxMin.z-hit_point.z)<eps)
             norm.z=-1;
        if(fabs(boxMax.x-hit_point.x)<eps)
             norm.x=1;
        if(fabs(boxMax.y-hit_point.y)<eps)
             norm.y=-1;
        if(fabs(boxMax.z-hit_point.z)<eps)
             norm.z=1; 
        //std::cout<<norm.x<<" "<<norm.y<<" "<<norm.z<<std::endl;
        return norm;
}
Sphere::Sphere(float n_r,Vect3f n_centr,Pixel n_color,float n_reflection,float n_refraction){
    r=n_r;
    centr=n_centr;
    color=n_color;
    reflection=n_reflection;
    refraction =n_refraction;
}
Vect3f Sphere::get_normal(Vect3f hit_point){
    return normalize(diff(hit_point,get_centr()));
}
bool Sphere::ray_intersect_Sphere(const Ray &ray,float& t){

    Vect3f k=diff(ray.pos,centr);
    //Vect3f dir_norm=normalize(ray.dir);
    float b=dot(k,ray.dir);
    float c=dot(k,k)-r*r;
    float d=b*b-c;
    
    if(d>=0){
            float sqrtfd=sqrtf(d);
            float t1=-b+sqrtfd;
            float t2=-b-sqrtfd;
            
            float t_min=std::min(t1,t2);
            float t_max=std::max(t1,t2);
            
            t=(t_min>0.01)?t_min:t_max;
            if(t<0.01){
                t=100;
                return false;
            }
            return (t>0.01);
    }
    return false;
}
            
Canvas::Canvas(Vect3f n_eye,Vect3f n_canvas_locate,Vect3f n_canvas_normal,float a_width,float a_height,Sphere*a_sphere,Box *a_box,Floor * n_floor,Floor * n_wall,Vect3f n_light1,Vect3f n_light2){
    eye=n_eye;
    canvas_locate=n_canvas_locate;
    width=a_width;
    height=a_height;
    sphere=a_sphere;
    box=a_box;
    light1=n_light1;
    light2=n_light2;
    floor=n_floor;
    canvas_normal=n_canvas_normal;
    wall=n_wall;
    deep=5;
}
float Sphere::get_reflection(Vect3f hit_point){
    /*Vect3f r1=normalize(diff(hit_point,centr));
    Vect3f r2=centr;
    r2.z+=r;
    r2=normalize(r2);
    return reflection*fabs(dot(r1,r2))+1;*/
  
        return reflection;
}
float Sphere::get_refraction(Vect3f hit_point){
    
    Vect3f r1=normalize(diff(hit_point,centr));
    Vect3f r2=centr;
    Vect3f r3=centr;
    Vect3f r4=centr;
    r2.z=r2.z-r;
    r2.x=r2.x-r;
    r2=normalize(r2);
    
    r3.z=r3.z-r;
    r3.x=r3.x+r;
    r3=normalize(r3);
    
    r4.z=r4.z-r;
    r4.y=r4.y;
    r4=normalize(r4);
    
    float scalar2=dot(r1,r2);
    float scalar3=dot(r1,r3);
    float scalar4=dot(r1,r4);
    
    if(scalar2>0.94)
        return 0.5;
        
    else
        if(scalar3>0.94)
           return 0.5; 
        else
            if(scalar4>0.94)
                return 0.3;
            else
                return refraction;
}
 Pixel Sphere::get_color(Vect3f hit_point){
     //return color;
    Pixel color_inclusion_2{200,0,200,0};
    Pixel color_inclusion_3{200,0,200,0};
    Pixel color_inclusion_4{200,0,200,0};
    Vect3f r1=normalize(diff(hit_point,centr));
    Vect3f r2=centr;
    Vect3f r3=centr;
    Vect3f r4=centr;
    
    r2.z=r2.z-r;
    r2.x=r2.x-r;
    r2=normalize(r2);
    
    r3.z=r3.z-r;
    r3.x=r3.x+r;
    r3=normalize(r3);
    
    r4.z=r4.z-r;
    r4.y=r4.y;
    r4=normalize(r4);
    
    float scalar2=dot(r1,r2);
    float scalar3=dot(r1,r3);
    float scalar4=dot(r1,r4);
    
    if(scalar2>0.94)
        return color_inclusion_2;
        
    else
        if(scalar3>0.94)
            return color_inclusion_3;
        else
            if(scalar4>0.94)
                return color_inclusion_4;
            else
                return color;
     
}
Hit Canvas::RaySceneIntersection(const Ray&ray){
    float t=10000;
    Hit hit={false,0,0,0,{0,0,0,0},{0,0,0}};
    float t2=0;
    if(sphere->ray_intersect_Sphere(ray,t2) && t2>0){
        t=t2;
        hit.exist=true;
        hit.t=t;
        
        Vect3f hit_point=sum(ray.pos,scalar(ray.dir,t));
        hit.normal=sphere->get_normal(hit_point);
        hit.refraction=sphere->get_refraction(hit_point);
        hit.reflection=sphere->get_reflection(hit_point);
        hit.color=sphere->get_color(hit_point);
        
    }
    
    if(box->ray_intersect_Box(ray,t2) && t2<t && t2>0 ){
        t=t2;
        hit.exist=true;
        hit.t=t2;
        hit.color=box->get_color();
        Vect3f hit_point=sum(ray.pos,scalar(ray.dir,t2));
        hit.normal=box->get_normal(hit_point);
        hit.refraction=box->get_refraction();
        hit.reflection=box->get_reflection();
    }
    t2=0;
    if(floor->ray_intersect_Floor(ray,t2) && t2<t && t2>0){
        hit.exist=true;
        hit.t=t2;
        hit.color=floor->get_color();
        hit.normal=floor->get_normal();
        hit.refraction=floor->get_refraction();
        hit.reflection=floor->get_reflection();
        t=t2;
    }
     t2=0;
    if(wall->ray_intersect_Floor(ray,t2) && t2<t && t2>0){
        t=t2;
        hit.exist=true;
        hit.t=t2;
        hit.color=wall->get_color();
        hit.normal=wall->get_normal();
        hit.refraction=wall->get_refraction();
        hit.reflection=wall->get_reflection();
       
    }
    return hit;
}
bool Canvas::Visible(Vect3f hit_point,Vect3f light){
    Ray ray;

    ray.dir=normalize(diff(light,hit_point));
    ray.pos=hit_point;////
    Hit hit=RaySceneIntersection(ray);
    float t=diff(light,hit_point).x/ray.dir.x;
   
    return (!hit.exist) || hit.t>t;
}
float Canvas::Shade(Vect3f hit_point,Vect3f normal,Vect3f light){
    float k_diffuse=0.3;
    float k_specular=0.2;
    float K_specular=0;
    float K_diffuse=0;
    //std::cout<<"hello"<<std::endl;
    Vect3f light_dir=normalize(diff(light,hit_point));
    Vect3f h=normalize(diff(scalar(normal,-2*dot(light_dir,normal)),light_dir));
    K_diffuse=k_diffuse*dot(light_dir,normal);
    //if(K_diffuse<0)
       // K_diffuse=0;
    /*if(k_diffuse*dot(light1_dir,normal)<0);
        std::cout<<normal.x<<" "<<normal.y<<" "<<normal.z<<std::endl;*/
    if(dot(normal,h)>0)
        K_specular=k_specular*powf(dot(normal,h),1000);
    return K_diffuse+K_specular;
    
}
float Canvas::Schlick(Ray ray,Hit hit,Vect3f light){
    Vect3f hit_point=sum(ray.pos,scalar(ray.dir,hit.t));
    Vect3f light1_dir=normalize(diff(ray.pos,hit_point));
    float o_cos=dot(hit.normal,light1_dir);
    if(o_cos<0)
        o_cos=-o_cos;
    float R0=powf((1-hit.refraction)/(1+hit.refraction),2);
    float R=R0+(1-R0)*powf((1-o_cos),5.f);
    return R;
} 
Ray Canvas::reflect(Ray ray ,Hit  hit){
    Ray res_ray;
    res_ray.pos=sum(ray.pos,scalar(ray.dir,hit.t));
  // Vect3f h=normalize(diff(scalar(normal,2*dot(light1_dir,normal)),light1_dir));
    res_ray.dir=normalize(sum(scalar(hit.normal,-2*dot(hit.normal,ray.dir)),ray.dir));
    return res_ray;
}
Ray Canvas::refract(Ray ray,Hit hit,bool &flag){
    float n2=hit.refraction;
    float n1=1;
    float n12=n1/n2;
    float cos_alpha1=-dot(hit.normal,ray.dir);
    Ray res_ray;
     res_ray.pos=sum(ray.pos,scalar(ray.dir,hit.t));
    if(cos_alpha1<0){
        n12=1/n12;
        cos_alpha1*=-1;
       hit.normal=scalar(hit.normal,-1);
    };
    float cos_alpha2=sqrtf(1-powf(n12,2)*(1-powf(cos_alpha1,2)));
    if(fabs(powf(n12,2)*(1-powf(cos_alpha1,2) ))<=1)
            flag=true;
    res_ray.dir=normalize(sum(scalar(ray.dir,n12),scalar(hit.normal,n12*cos_alpha1-cos_alpha2)));
    
    return res_ray;
}
Pixel Canvas::RayTrace(const Ray& ray){
         bool flag=false;
        Pixel color{0,0,0,0};
        Hit hit=RaySceneIntersection(ray);
        if(!hit.exist)
            return color;
        Vect3f hit_point=sum(ray.pos,scalar(ray.dir,hit.t));
        float k=0;
        float ambitient=0.2;//подключили окружающее освещение
        
        //pix=scalar_Pixel(hit.color,ambitient);
        if(Visible(hit_point,light1)){//в случае видимости подключаем рассеянный и зеркальный свет
           
            float dist=distance(hit_point,light1);
            if(dist>0){
                 k+=800*Shade(hit_point,hit.normal,light1)/powf(dist,2);
            }
            
        }
        if(Visible(hit_point,light2)){
            float dist=distance(hit_point,light2);
            if(dist>0)
                k+=800*Shade(hit_point,hit.normal,light2)/powf(dist,2);
            
        }
        if(hit.refraction>0.f && deep>0){
           
            Ray refract_ray=refract(ray,hit,flag);
            float k_refract=1-(Schlick(ray,hit,light1)+Schlick(ray,hit,light2))/2;
     
            deep=deep-1;
            if(flag){
                Pixel color_refract=RayTrace(refract_ray);
                color=sum_Pixel(color,scalar_Pixel(color_refract,k_refract));//color_refract);//;
            }
        }
        if(hit.reflection>0.f && deep>0){
            
            float k_reflect=Schlick(ray,hit,light1);
            k_reflect=(k_reflect+Schlick(ray,hit,light2))/2;
            Ray reflect_ray=reflect(ray,hit);
            deep=deep-1;
            if(flag)
                color=sum_Pixel(color,scalar_Pixel(RayTrace(reflect_ray),k_reflect));
            else
                 color=sum_Pixel(color,RayTrace(reflect_ray));
                           
            //else 
               // color=scalar_Pixel(RayTrace(reflect_ray),0.3);
                //RayTrace(reflect_ray);////
           // std::cout<<int(color.r)<<int(color.g)<<int(color.b)<<std::endl;
        }
        
        k+=ambitient;
        color=sum_Pixel(color,scalar_Pixel(hit.color,k));//sum_Pixel(color,scalar_Pixel(hit.color,k));
        if(deep!=5)
            deep=deep+1;
        return color;
        
}
Ray Canvas::Pixel_to_Ray(int i, int j,int canvas_width,int canvas_height){
    Ray ray;
    Vect3f pos=eye;
    Vect3f dir;
    dir.z=canvas_locate.z;
    
    int i_new=-canvas_width/2+i;
    int j_new=canvas_height/2-j;
    /*теперь перевернули систему координат и опустили*/
    dir.x=i_new*width/canvas_width+canvas_locate.x-pos.x;
    dir.y=j_new*height/canvas_height+canvas_locate.y-pos.y;
    ray.pos=pos;
    ray.dir=normalize(dir);
    return ray;
}

void Canvas::Render(Image&picture,int canvas_width,int canvas_height){
    for(int i=0;i<canvas_width;i++)
        for(int j=0;j<canvas_height;j++){
            Ray ray=Pixel_to_Ray(i,j,canvas_width,canvas_height);
            /*if(ray.dir.x<0.5 && ray.dir.y<0.5)
                std::cout<<ray.dir.x<<std::endl<<ray.dir.y<<std::endl<<ray.dir.z<<std::endl;*/
            Pixel pix=RayTrace(ray);
            picture.PutPixel(i,j,pix);
        }
}


