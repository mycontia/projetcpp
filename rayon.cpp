#include "rayon.h"

// Simulation d'un plan en z = 0 (On dit que ça touche si le rayon descend)
bool Quad::is_hit(Ray3f ray) {
    return ray.direction_.z_ < 0; 
}

// bool Sphere::is_hit(Ray3f ray) {
//     //C'est pour un point ici
//     return (ray.origin_.x_ > -0.01+this->origin_.x_ && ray.origin_.x_ < 0.01 + this->origin_.x_ &&
//             ray.origin_.y_ > -0.01+this->origin_.y_ && ray.origin_.y_ < 0.01+this->origin_.y_);
// }

// Pour reflect, renvoie juste le rayon d'origine pour que ça compile
Ray3f Sphere::reflect(Ray3f ray) { return ray; }
Ray3f Quad::reflect(Ray3f ray) { return ray; }

// partie Laura

bool Sphere::is_hit(Ray3f ray){
    float a = prod_scal(ray.direction_,ray.direction_);
    float b = 2 * prod_scal(ray.direction_ ,(ray.origin_ -  origin_));
    float c = -2 * prod_scal(origin_,ray.origin_)+ prod_scal(origin_,origin_) + prod_scal(ray.origin_,ray.origin_) -radius_*radius_;
    float discriminent = b*b - 4*a*c;
    return (discriminent > 0);
}

float prod_scal (Vector3f v1, Vector3f v2) {
    float n = v1.x_ * v2.x_ + v1.y_ * v2.y_ + v1.z_ * v2.z_;
    return n;
}