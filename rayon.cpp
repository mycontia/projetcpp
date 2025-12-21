#include "rayon.h"

// int main (){

// return 0; 

// }

// Simulation d'un plan en z = 0 (On dit que ça touche si le rayon descend)
bool Quad::is_hit(Ray3f ray) {
    return ray.direction_.z_ < 0; 
}

bool Sphere::is_hit(Ray3f ray) {
    //C'est pour un point ici
    return (ray.origin_.x_ > -0.01+this->origin_.x_ && ray.origin_.x_ < 0.01 + this->origin_.x_ &&
            ray.origin_.y_ > -0.01+this->origin_.y_ && ray.origin_.y_ < 0.01+this->origin_.y_);
}

// Pour reflect, renvoie juste le rayon d'origine pour que ça compile
Ray3f Sphere::reflect(Ray3f ray) { return ray; }
Ray3f Quad::reflect(Ray3f ray) { return ray; }