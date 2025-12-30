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


answer Quad::is_hit(Ray3f ray){

     
    Vector3f depth_= prod_vect(width_, height_);
    Vector3f depth_norm = depth_.normalise();
    answer reponse = {false, Vector3f(),Vector3f()};
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
    reponse.hit = est_dans_surf(vect);
    if (reponse.hit == true) {
        reponse.pt_inter = pt_inter;
        reponse.norm = depth_norm;
    }
    return(reponse);
 
}



// partie Laura

answer Sphere::is_hit(Ray3f ray){
    answer reponse = {false, Vector3f(),Vector3f()};
    //calcul du discrimant du polynome du second degré que l'on obtient en combinant 2 égalités sur comment s'écrit un point appartenant à un rayon et la condtion pour que ce point appartienne à la sphère 
    float a = prod_scal(ray.direction_,ray.direction_);
    float b = 2 * prod_scal(ray.direction_ ,(ray.origin_ -  origin_));
    float c = -2 * prod_scal(origin_,ray.origin_)+ prod_scal(origin_,origin_) + prod_scal(ray.origin_,ray.origin_) -radius_*radius_;
    float discriminent = b*b - 4*a*c;
    if (discriminent == 0) {
        reponse.hit = true;
        float t = - b / (2*a);
        reponse.pt_inter = Vector3f(ray.origin_ + ray.direction_*t);
    }
    if (discriminent > 0) {
        reponse.hit = true;
        float t1 = (- b - std::sqrt(discriminent))/ (2*a);
        float t2 = (- b + std::sqrt(discriminent))/ (2*a);
        Vector3f p1 = Vector3f(ray.origin_ + ray.direction_*t1);
        Vector3f p2 = Vector3f(ray.origin_ + ray.direction_*t2);
        if((p1 - ray.origin_).norme() >(p2 - ray.origin_).norme()){
            reponse.pt_inter = p2;
        }
        else{
            reponse.pt_inter = p1;
        }
    
    }
    if (discriminent >= 0) {
        reponse.norm = (reponse.pt_inter - origin_).normalise();
        
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


answer Cube::is_hit(Ray3f ray){

    Vector3f depth= prod_vect(width_, height_);
    std::vector<Quad> q;
    std::vector<Vector3f> pt_intersect; // stock si il a intersection entre le rayon et le quadrilatère et si collision contient aussi les coordonnées du point le plus proche, (0,0,0) sinon ainsi que la nomale à la surface où il y a collision
    

    
    q.push_back(Quad(origin_ + depth *(-0.5f),height_, width_, matter_) ); // devant
    q.push_back(Quad(origin_ + depth * 0.5f, width_,height_,  matter_)); //derrière #devant sur image correct
    q.push_back(Quad(origin_ + width_ * 0.5f, height_,depth, matter_)); // droite 
    q.push_back(Quad(origin_ + width_ * (-0.5f),depth ,height_,  matter_)); //gauche #gauche sur l'image correct
    q.push_back(Quad(origin_ + height_ * (-0.5f), width_ ,depth, matter_)); //haut 
    q.push_back(Quad(origin_ + height_ *0.5f, depth,width_, matter_));// bas
    // on pourrait se contenter de tester les faces jusqu'à ce qu'une soit en collision avec le rayon mais on veut aussi avoir les coordonnées du point le plus proche donc on va tester pour toutes les faces
    
    answer pt_proche = {false, Vector3f(),Vector3f()};
    float dist_min = INFINITY; 
    for (size_t i = 0; i < q.size(); i++) {
        answer pt_inter = q[i].is_hit(ray);
        
        if (pt_inter.hit) {
            float dist = (pt_inter.pt_inter - ray.origin_).norme();
            if (dist < dist_min) {
                dist_min = dist;
                pt_proche = pt_inter;

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

float max(float a, float b){
    if (a <= b){
        return b;
    }
    return a;
}


float Scene::intensite(answer a) {
    if (a.hit){
        Vector3f v1= a.norm;
        Vector3f v2 =(source_.origin_ - a.pt_inter).normalise();
        //Vector3f v2 =((source_.direction_ - source_.origin_) - a.pt_inter).normalise();
        return 0.2f + 0.8f * max(0.0f, prod_scal(v1, v2));
        return (max(0,prod_scal(v1,v2))); // si le produit scalaire est négatif, alors la lumière est derrière la surface donc l'objet est  noir.
    // v1 et v2 etant normalisés, le produit scalaire entre les deux est donc égal au cosinus de l'angle entre les 2 vecteurs 
    //plu sle cosinus est proche de 1 est plus le point d'intersection ets éclairé 
    }
    return 0;
}


void draw_color(SDL_Renderer* rend, Material col, float intens){
    // rend= pinceau change de couleur
    // Rouge Vert Bleu et transparence
    SDL_SetRenderDrawColor(rend, col.r_ * intens , col.g_ * intens, col.b_ *intens, 255);
}

// Pour reflect, renvoie juste le rayon d'origine pour que ça compile
Ray3f Shape::reflect(Ray3f r, answer a) { 
    Ray3f reflet = Ray3f();
    if(a.hit && (matter_.shininess_ !=0)){
        Vector3f direction_reflet = (r.direction_ +  a.norm * ((- 2) * prod_scal(r.direction_ , a.norm))).normalise();
        reflet = Ray3f(a.pt_inter + a.norm * 0.001f, direction_reflet); // si avec les arrondis de calculs on a un pt_iter qui est légèrement décalé et à l'interieure de la surface, la réflection va donc avoir lieu sur le meme solide donc on décale un peu le point d'intersection en cas d'erreurs de calculs sur les floats 
    }  // + a.norm * 0.001f n'enlève pas les petits points 
    return reflet;
    }

Material Material::couleur( Material reflet){
    return Material((r_ * (1- shininess_) + reflet.r_*shininess_), (g_ * (1- shininess_) + reflet.g_*shininess_),(b_ * (1- shininess_) + reflet.b_*shininess_),0);

}



Material recursive(Ray3f ray, 
    Scene scene, int count) {
    if (count > 7) {
        return Material(0, 0, 0, 0); // si il a trop de rebonds 
    }
    // ça ça va changer, c'est à dire que le rayon va être projeté de la caméra ves le pixel.
    // donc l'origine sera bien (x,y,-50) par exemple
    // comme ça on ne change pas l'endroit 
    //bon en fait là je place la caméra en origine x y -10
    //bref c'est le 2.a ici
    

    // je veux en x en y et en z en fonction de sphere

    
    //SDL_RenderClear(renderer); //JONATHAN 
    float dist_min = INFINITY;
    Shape* shape_proche = nullptr; 
    answer pt_proche;
    // si un rayon traverse une forme puis une autre à la sortie de la première on ne veut voir que la première touchée par le rayon, celle cachée ne devant pas apparaitre sur la fenetre 
    
    for (size_t i =0; i< scene.shapes_.size(); i++){
        answer rep = scene.shapes_[i]->is_hit(ray);
        if (rep.hit) {
            float dist = (rep.pt_inter - ray.origin_).norme();
            if (dist < dist_min && dist> 0.01f) { // pas obligé le >
                dist_min = dist;
                shape_proche = scene.shapes_[i];
                pt_proche = rep;
            }
        }
    }
    if (shape_proche != nullptr) {

        /////////////////////////////////////////////// A CHECKER 
        
         // On calcule la couleur de base avec l'intensité lumineuse
        float intens = scene.intensite(pt_proche);
        float shininess = shape_proche->matter_.shininess_ ;
        Material couleur_obj(shape_proche->matter_.r_ * intens, shape_proche->matter_.g_ * intens, shape_proche->matter_.b_ * intens, shininess);

        if(shininess != 0){ //il y a un rayon réfléchi
            Ray3f rayon_reflet = shape_proche->reflect(ray, pt_proche);
            
            Material couleur_reflet = recursive(rayon_reflet, scene, count +1);
            
            // On mélange la couleur de l'objet et celle du reflet
            return couleur_obj.couleur(couleur_reflet);
        }

        return couleur_obj;
    }

    // 4. Si on ne touche rien (le vide/ciel)
    return Material(0, 0, 0, 0); 
}   
            
void Scene::box(Vector3f centre, float w, float h, float d, Material m){

    Vector3f x(1,0,0);
    Vector3f y(0,1,0);
    Vector3f z(0,0,1);

    Quad* qback = new Quad(
        centre + z * (d * 0.5f), // Centre du quad
        y*h,
        x*w,// on fait cette ordre pour orienter le vecteur normal vers l'intérieur de la boite 
        m
    );
    
    Quad* qleft = new Quad(
        centre + x *(- w * 0.5f), 
        y*h,
        z*d,  
        m
    );

    Quad* qright = new Quad(
        centre + x *(w * 0.5f), 
        z*d, //inversion des coordonnées par rapport à qleft pour orienter le vecteur normal vers l'extérieur de la boite 
        y*h, 
        m
    );
    Quad* qup = new Quad(
        centre + y * (h * 0.5f), 
        z*d,
        x*w,
        m
    );
    Quad* qdown = new Quad(
        centre + y * (- h * 0.5f),
        x*w,
        z*d,
        m
    );
    shapes_.push_back(qup);
    shapes_.push_back(qdown);
    shapes_.push_back(qleft);
    shapes_.push_back(qright);
    shapes_.push_back(qback);
}

