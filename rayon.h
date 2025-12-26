#ifndef RAYON_H
#define RAYON_H
#include <iostream>
#include <cmath>
//ajout d'un include
#include <vector>

class Vector3f {
    public :
        float x_; 
        float y_;
        float z_; 
        Vector3f() {
            x_= 0; 
            y_ = 0;
            z_ = 0;
        };
        Vector3f(float x, float y, float z){
            x_= x; 
            y_ = y;
            z_ = z;   
        }
        Vector3f operator+ (Vector3f v1) const {  // const car on ne veut pas modifier le vecteur; & ? vect3f n'est pas une grosse struct 
            Vector3f v;
            v.x_ =  x_ + v1.x_;  //ATENTION A L'ORDRE
            v.y_ = y_ + v1.y_;
            v.z_ =  z_ + v1.z_;
            return v;
        }
        Vector3f operator- (Vector3f v1) const {  // const car on ne veut pas modifier le vecteur; & ? vect3f n'est pas une grosse struct 
            Vector3f v;
            v.x_ =  x_ - v1.x_;  //ATENTION A L'ORDRE
            v.y_ = y_ - v1.y_;
            v.z_ =  z_ - v1.z_;
            return v;
        }


        Vector3f operator* (float a) const {  // const car on ne veut pas modifier le vecteur; & ? vect3f n'est pas une grosse struct 
            Vector3f v;
            v.x_ = a * x_;
            v.y_ = a * y_;
            v.z_ = a * z_;
            return v;
        }

        Vector3f operator* (Vector3f v1) const {  // const car on ne veut pas modifier le vecteur; & ? vect3f n'est pas une grosse struct 
            Vector3f v;
            v.x_ = v1.x_ * x_;  // multiplaction commutative 
            v.y_ = v1.y_ * y_;
            v.z_ = v1.z_ * z_;
            return v;
        }

        Vector3f directionV (Vector3f v) const{
            Vector3f d;
            d.x_ = v.x_ - x_;
            d.y_ = v.y_ - y_;
            d.z_ = v.z_ - z_;
            return d;

        }

        float norme () const{  //normalisation 
            float n;
            n = std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
            return n; 

        }

        Vector3f normalise() const{ //normalise un vecteur 
            float n = norme();
            Vector3f v = Vector3f(0, 0, 0);
            if (n>0){
                float m = 1.0f / n;
                v.x_= x_ * m;
                v.y_ = y_ * m;
                v.z_ = z_* m;
            }
            return v;
        }
};

float prod_scal (Vector3f v1, Vector3f v2);
Vector3f prod_vect(Vector3f v1, Vector3f v2);
Vector3f direction (Vector3f v, Vector3f w);
bool egal (Vector3f v1, Vector3f v2);

class Ray3f {
    public: 
        Vector3f origin_; 
        Vector3f direction_;
        Ray3f(){
            origin_ = Vector3f();
            direction_ = Vector3f();
        }
        Ray3f(Vector3f o, Vector3f d){
            origin_ = o;
            direction_ = d;
        }
};

class Camera {
    public: 
        Vector3f position_; 
        Vector3f direction_;
        Camera(){
            position_ = Vector3f();
            direction_ = Vector3f();  
        }

        Camera(Vector3f p, Vector3f d){
            position_ = p;
            direction_ = d;
        }

};

class Material {
    public: 
        float r_;
        float g_;
        float b_; 
        float shininess_;

        Material(){
            r_ = 0;
            g_ = 0;
            b_ = 0;
            shininess_ =0;
        }
        Material(float r, float g, float b, float s){
            r_ = r;
            g_ = g;
            b_ = b;
            shininess_ =s;
        }

};

class Shape {
    public: 
        Material matter_; 

        virtual bool is_hit(Ray3f ray) =0 ;  // & ? pas grosse struct 
        virtual Ray3f reflect(Ray3f ray)=0;


};




class Cube : public Shape{
    public: 
        Vector3f origin_; 
        Vector3f height_;
        Vector3f width_; // pour faire des paralélépipèdes
        Cube(){
            origin_ = Vector3f();
            height_ = Vector3f();
            width_ = Vector3f();
            matter_ = Material();
        }

        Cube(Vector3f o, Vector3f h,Vector3f w, Material m){
            origin_ = o;
            height_ = h;
            width_ = w;
            matter_ = m;
        }


        bool est_dans_surf(Vector3f w,  Vector3f h, Vector3f d, Vector3f direction, Vector3f v) const;

/* 
            if (egal (direction,width_)){
            float dist_u = prod_scal(direction,depth );
            float dist_v = prod_scal(direction, height_);
            }
                else {
                    if (egal(direction, width_ * (-1))){
                    float dist_u = prod_scal(direction,depth* (-1) );
                    float dist_v= prod_scal(direction, height_* (-1));
                    }
                    else {
                        if (egal(direction,height_)){
                        float dist_u = prod_scal(direction,depth);
                        float dist_v = prod_scal(direction, width_);
                        }
                        else{
                            if (egal(direction,height_ * (-1))){
                            float dist_u = prod_scal(direction,depth* (-1) );
                            float dist_v = prod_scal(direction, width_* (-1));
                            }
                            else{
                                if (egal(direction,depth)){
                                float dist_u = prod_scal(direction,width_ );
                                float dist_v= prod_scal(direction, height_);
                                }
                                else{
                                    if (egal(direction, width_ * (-1))){
                                    float dist_u = prod_scal(direction,width_* (-1) );
                                    float dist_v = prod_scal(direction, height_* (-1));
                                    }
                                }
                            }
                        }
                    }
                    
        }

*/


/*
        std::vector<Vector3f> vect_normal(){
            std::vector<Vector3f> v;

            Vector3f depth = prod_vect(width_, height_);
            
            // face haut 
            


            // face devant 
            
            
            //face coté 
            

            return v;
        }
*/
        bool is_hit(Ray3f ray);
        Ray3f reflect(Ray3f ray);

};

class Quad : public Shape{
    public: 
        Vector3f origin_; 
        Vector3f width_;

        Vector3f height_;
        Quad(){
            origin_ = Vector3f();
            width_= Vector3f();
            height_ = Vector3f();
            matter_ = Material();
        }

        Quad(Vector3f o, Vector3f w, Vector3f h, Material m){
            origin_ = o;
            width_= w;
            height_ = h;
            matter_ = m;
        }


        bool is_hit(Ray3f ray);
        Ray3f reflect(Ray3f ray);
};

class Sphere : public Shape{
    public: 
        Vector3f origin_; 
        float radius_;
        
        Sphere(){
            origin_ = Vector3f();
            radius_= 0;
            matter_ = Material();
        }

        Sphere(Vector3f o, float r, Material m){
            origin_ = o;
            radius_= r;
            matter_ = m;
        }
        bool is_hit(Ray3f ray);
        Ray3f reflect(Ray3f ray);
};


class Scene {
    public: 
        Camera camera_; 
        std::vector<Shape*> shapes_;
        Ray3f source_;
        void render(int width, int height, std::string filename);

};

// class Grille {
//     int width_;
//     int height_; 
// };

#endif