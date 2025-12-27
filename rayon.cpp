#include "rayon.h"
#include<iostream>

// Simulation d'un plan en z = 0 (On dit que ça touche si le rayon descend)
/* bool Quad::is_hit(Ray3f ray) {
    return ray.direction_.z_ < 0; 
}

*/

bool Quad::est_dans_surf(Vector3f v){
    
    // Face Devant ou Derrière 
    float dist_u = prod_scal(v, width_.normalise());
    float dist_v = prod_scal(v, height_.normalise());
    float max_u = width_.norme() * 0.5f;
    float max_v = height_.norme() * 0.5f;
    return (((std::abs(dist_u) <= max_u) && (std::abs(dist_v) <= max_v))); // Le point est à l'intérieur 

}


std::pair<bool,Vector3f> Quad::is_hit(Ray3f ray){

    std::pair<bool,Vector3f> reponse; 
    reponse.first = false;
    reponse.second = Vector3f();
    Vector3f depth_= prod_vect(width_, height_);
    Vector3f depth_norm = depth_.normalise();

    float prod_D = prod_scal(ray.direction_,depth_norm);

    //si le produit scalaire entre la direction du rayon et la normale à une des 6 surfaces vaut 0 alors ces deux vecteurs sont perpendiculaires et donc le rayon est parallèle à la surface en question 

    if (prod_D == 0){
        return reponse;
    }
     
    float t = prod_scal((origin_ - ray.origin_),depth_norm) / (prod_D);//distance entre l'origine du rayon et le point d'impact sur la face infinie du quad
    if (t<= 0){ // intersection derrière la caméra ? pas mettre 0 mais epsilon ? 
        return reponse;
    }
    Vector3f pt_inter = ray.origin_ + ray.direction_ * t; 

    //Vector3f vect = (*it_pt)[2]- origin_; //vecteur centre du cube au point d'intersection 
    Vector3f vect = pt_inter - origin_;
    //bool est_dedans = Cube::est_dans_surf(width_norm, height_norm, depth_norm,(*it_pt)[1], vect);
    reponse.first = est_dans_surf(vect);
    if (reponse.first == true) {
        reponse.second = pt_inter;
    }
    return(reponse);
 
}

// Pour reflect, renvoie juste le rayon d'origine pour que ça compile
Ray3f Sphere::reflect(Ray3f ray) { return ray; }
Ray3f Quad::reflect(Ray3f ray) { return ray; }

// partie Laura

std::pair<bool,Vector3f> Sphere::is_hit(Ray3f ray){
    std::pair<bool,Vector3f> reponse; 
    reponse.first = false;
    reponse.second = Vector3f();
    //calcul du discrimant du polynome du second degré que l'on obtient en combinant 2 égalités sur comment s'écrit un point appartenant à un rayon et la condtion pour que ce point appartienne à la sphère 
    float a = prod_scal(ray.direction_,ray.direction_);
    float b = 2 * prod_scal(ray.direction_ ,(ray.origin_ -  origin_));
    float c = -2 * prod_scal(origin_,ray.origin_)+ prod_scal(origin_,origin_) + prod_scal(ray.origin_,ray.origin_) -radius_*radius_;
    float discriminent = b*b - 4*a*c;
    if (discriminent == 0) {
        reponse.first = true;
        float t = - b / (2*a);
        reponse.second = Vector3f(ray.origin_ + ray.direction_*t);
    }
    if (discriminent > 0) {
        reponse.first = true;
        float t1 = (- b - std::sqrt(discriminent))/ (2*a);
        float t2 = (- b + std::sqrt(discriminent))/ (2*a);
        Vector3f p1 = Vector3f(ray.origin_ + ray.direction_*t1);
        Vector3f p2 = Vector3f(ray.origin_ + ray.direction_*t2);
        if((p1 - ray.origin_).norme() >(p2 - ray.origin_).norme()){
            reponse.second = p2;
        }
        else{
            reponse.second = p1;
        }
    
    }
    return reponse;
}

Vector3f dir(Ray3f ray ){
    Vector3f v;
    v  = ray.origin_ + ray.direction_;
    return v;
}

bool egal (Vector3f v1, Vector3f v2)  {
            
            return ((v1.x_ == v2.x_) && (v1.y_ == v2.y_) && (v1.z_ == v2.z_));
        };


std::pair<bool,Vector3f> Cube::is_hit(Ray3f ray){

    Vector3f depth= prod_vect(width_, height_);
    std::vector<Quad> q;
    std::vector<Vector3f> pt_intersect; // stock si il a intersection entre le rayon et le quadrilatère et si collision contient aussi les coordonnées du po,its le plus proche, (0,0,0) sinon
    q.push_back(Quad(origin_ + depth *0.5f, width_, height_, matter_) ); // devant
    q.push_back(Quad(origin_ + depth * (-0.5f), width_, height_, matter_)); //derrière 
    q.push_back(Quad(origin_ + width_ * 0.5f, height_, depth, matter_)); // droite 
    q.push_back(Quad(origin_ + width_ * (-0.5f), height_, depth, matter_)); //gauche
    q.push_back(Quad(origin_ + height_ * 0.5f, width_, depth, matter_)); //haut 
    q.push_back(Quad(origin_ + height_ *(-0.5f), width_, depth, matter_));// bas
    // on pourrait se contenter de tester les faces jusqu'à ce qu'une soit en collision avec le rayon mais on veut aussi avoir les coordonnées du point le plus proche donc on va tester pour toutes les faces
    
    std::pair<bool,Vector3f> pt_proche;
    float dist_min = INFINITY; 
    for (size_t i = 0; i < q.size(); i++) {
        std::pair<bool, Vector3f> pt_inter = q[i].is_hit(ray);
        
        if (pt_inter.first) {
            float dist = (pt_inter.second - ray.origin_).norme();
            if (dist < dist_min) {
                dist_min = dist;
                pt_proche.second = pt_inter.second;
                pt_proche.first = pt_inter.first;
            }
        }
    }
    return pt_proche;
}



float prod_scal (Vector3f v1, Vector3f v2) {
    float n = v1.x_ * v2.x_ + v1.y_ * v2.y_ + v1.z_ * v2.z_;
    return n;
}

Vector3f prod_vect(Vector3f v1, Vector3f v2) {
    Vector3f v;
        v.x_ = v1.y_ * v2.z_ - v1.z_ * v2.y_;
        v.y_ =v1.z_ * v2.x_ - v1.x_ * v2.z_;
        v.z_ =v1.x_ * v2.y_ - v1.y_* v2.x_;
        return v;

}



Ray3f Cube::reflect(Ray3f ray) {
    return ray; 
}