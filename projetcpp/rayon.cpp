#include "rayon.h"
#include<iostream>

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

Vector3f dir(Ray3f ray ){
    Vector3f v;
    v  = ray.origin_ + ray.direction_;
    return v;
}

bool egal (Vector3f v1, Vector3f v2)  {
            
            return ((v1.x_ == v2.x_) && (v1.y_ == v2.y_) && (v1.z_ == v2.z_));
        };


bool Cube::is_hit(Ray3f ray) const{

    Vector3f width_norm = width_.normalise();  // vecteur normalisé 
    Vector3f height_norm = height_.normalise();
    Vector3f depth_= prod_vect(width_, height_);
    Vector3f depth_norm = depth_.normalise();

    std::vector<float> ps; //{produit scalaire entre la direction du rayon et chacun des 6 vecteurs normaux aux 6 faces du quadrilatère ou le rayon n'est pas parallèle à la face en question }
    std::vector<std::vector<Vector3f> > pt; // {{point qui appartient à une face (si le rayon n'est pas parallèle à la face), vecteur normal à la surface (surface du haut : height, de droite : width ...), point d'inetrsection entre le rayon et la surface infinie}, ... }
    std::vector<Vector3f> coord; // contient les coordonnées des points d'intersection qui appartiennent à la surface finie
    float prod_H = prod_scal(ray.direction_, height_norm);
    float prod_D = prod_scal(ray.direction_,depth_norm);
    float prod_W = prod_scal(ray.direction_,width_norm);

    //si le produit scalaire entre la direction du rayon et la normale à une des 6 surfaces vaut 0 alors ces deux vecteurs sont perpendiculaires et donc le rayon est parallèle à la surface en question 
    if (prod_H != 0){
        ps.push_back(prod_H);
        ps.push_back(-prod_H);

        std::vector<Vector3f> haut; 
        haut.push_back(origin_ + height_ ); 
        haut.push_back(height_);         
        pt.push_back(haut);

        std::vector<Vector3f> bas; 
        bas.push_back(origin_ + height_ * (-0.5f)); 
        bas.push_back(height_ * (-1.0f));         
        pt.push_back(bas);
        
        //pt.push_back ({origin_+ height_ * (0.5f), height_}); // point appartenant a la face du haut 
        //pt.push_back ({ origin_ + height_ * (-0.5f), height_ * (-1)}); // point appartenant a la face du bas
    }
    if (prod_D != 0){
        ps.push_back(prod_D);
        ps.push_back(-prod_D);

        std::vector<Vector3f> devant; 
        devant.push_back(origin_ + depth_ ); 
        devant.push_back(depth_);         
        pt.push_back(devant);

        std::vector<Vector3f> derriere; 
        derriere.push_back(origin_ + depth_ * (-0.5f)); 
        derriere.push_back(depth_ * (-1.0f));         
        pt.push_back(derriere);

       // pt.push_back ({ origin_ + depth_ * (0.5f), depth_}); // point appartenant a la face de devant
        //pt.push_back ({ origin_ + depth_ * (-0.5f), depth_* (-1)});// point appartenant a la face de derrière
    }
    if (prod_W != 0){
        ps.push_back(prod_W);
        ps.push_back(-prod_W);
        
        std::vector<Vector3f> droite; 
        droite.push_back(origin_ + width_ ); 
        droite.push_back(width_);         
        pt.push_back(droite);

        std::vector<Vector3f> gauche; 
        gauche.push_back(origin_ + width_ * (-0.5f)); 
        gauche.push_back(width_ * (-1.0f));         
        pt.push_back(gauche);

        //pt.push_back ({ origin_ + width_ * (0.5f), width_});// point appartenant a la face de droite
        //pt.push_back ({ origin_ + width_ * (-0.5f), width_* (-1)});// point appartenant a la face de gauche
    }

    std::vector<float>::iterator it_ps= ps.begin();
    std::vector<float>::iterator it_ps_fin= ps.end();
    std::vector<std::vector<Vector3f> >::iterator it_pt = pt.begin();
    std::vector<Vector3f> reponse;
    
    while (it_ps < it_ps_fin) {
        float t = prod_scal(((*it_pt)[0] - ray.origin_),((*it_pt)[1]).normalise()) / (*it_ps);//distance entre l'origine du rayon et le point d'impact sur la face infinie du quad
        if (t<= 0){ // intersection derrière la caméra ? pas mettre 0 mais epsilon ? 
            it_ps = ps.erase(it_ps);
            it_pt = pt.erase(it_pt);

        }
        else {



/* 
            std::pair<bool,Vector3f> reponse; 
            if(reponse.first == false ){
                it_ps = ps.erase(it_ps);
                it_pt = pt.erase(it_pt);
            }
            (*it_pt).push_back(reponse.second);
*/


            Vector3f pt_inter = ray.origin_ + ray.direction_ * t;
            (*it_pt).push_back(pt_inter); // les coordonnées du point d'intersection entre le rayon et la face infinie 

            Vector3f vect = (*it_pt)[2]- origin_; //vecteur centre du cube au point d'intersection 
            bool est_dedans = est_dans_surf(width_norm, height_norm, depth_norm,(*it_pt)[1], vect);
            if (not est_dedans) {
                it_ps = ps.erase(it_ps);  // inutile puisuqe les coordonnées qui insterscetent sont stockés dans reponse ( on libère juste de la mémoire ) 
                it_pt = pt.erase(it_pt);//inutile mais si on met pas il faut augmanter quand meme l'iterateur donc attention 
            }
            else {
                 coord.push_back(pt_inter);
                 it_ps ++;
                 it_pt ++; 
            }

    }
    }
    return (reponse.size() > 0);

    
    // point appartenant a la face du haut 
    //Vector3f A_h = origin_ + height_ * (1/2);
    // point appartenant a la face du bas 
    //Vector3f A_b = origin_ + height_ * (-1/2);
    // point appartenant a la face de devant  
    //Vector3f B_der = origin_ + depth * (1/2);
    // point appartenant a la face de derrière
    //Vector3f B_dev = origin_ + depth * (-1/2);
    // point appartenant a la face de droite
    //Vector3f C_d = origin_ + width_ * (1/2);
    // point appartenant a la face de gauche
    //Vector3f C_g = origin_ + width_ * (-0.5f);
    // float prod_A = prod_scal(ray.direction_, height_norm);
    // float prod_B = prod_scal(ray.direction_,depth_norm);
    // float prod_C = prod_scal(ray.direction_,width_norm); 
    // if (prod_A != 0) {
    //     float t = prod_scal((origin_ - ray.origin_),height_) / prod_A;//distance entre l'origine du raypoon et le point d'impact sur la face infinie du quad
    //     if (t>0){
    //         Vector3f pt_A = ray.origin_ + ray.direction_ * t;
    //         Vector3f dist_A = pt_A - origin_;
    //         if ((abs(dist_A.x_) <= (0.5f) * width_) && (abs(dist_A.y_) <= (1/2) * depth)){
    //             return true;
    //         }

    //     }
    // }

 
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

Vector3f direction (Vector3f v, Vector3f w) {
    Vector3f d;
    d.x_ = w.x_ - v.x_;
    d.y_ = w.y_ - v.y_;
    d.z_ = w.z_ - v.z_;
    return d;

}