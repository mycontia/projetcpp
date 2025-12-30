#ifndef RAYON_H
#define RAYON_H
#include <iostream>
#include <cmath>
//ajout d'un include
#include <vector>
#include <SDL2/SDL.h>

/**
 * @class Vector3f
 * @brief Classe pour gérer les vecteurs en 3D
 * * Contient les coordonées x, y, z et les opérations de base
 */
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
        Vector3f operator+ (Vector3f v1) const {  // const car on ne veut pas modifier le vecteur
            Vector3f v;
            v.x_ =  x_ + v1.x_;  
            v.y_ = y_ + v1.y_;
            v.z_ =  z_ + v1.z_;
            return v;
        }
        Vector3f operator- (Vector3f v1) const {  
            Vector3f v;
            v.x_ =  x_ - v1.x_;  
            v.y_ = y_ - v1.y_;
            v.z_ =  z_ - v1.z_;
            return v;
        }


        Vector3f operator* (float a) const {  
            Vector3f v;
            v.x_ = a * x_;
            v.y_ = a * y_;
            v.z_ = a * z_;
            return v;
        }

        Vector3f operator* (Vector3f v1) const {  
            Vector3f v;
            v.x_ = v1.x_ * x_;  
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

        /**
         * @brief calcul la norme du vecteur
         * @return la longeur du vecteur
         */
        float norme () const{  
            float n;
            n = std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
            return n; 

        }

        /**
         * @brief transforme le vecteur en un vcteur de norme 1
         */
        Vector3f normalise() const{ 
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

/**
 * @struct answer
 * @brief stock les infos sur l'intersection 
 */
struct answer {
    bool hit;  ///<  vrai si ca touche
    Vector3f pt_inter; ///< point d'impact
    Vector3f norm; ///< normale à la surface
};

//  Prototypes des fonctions outils
float prod_scal (Vector3f v1, Vector3f v2);
Vector3f prod_vect(Vector3f v1, Vector3f v2);
Vector3f direction (Vector3f v, Vector3f w);
bool egal (Vector3f v1, Vector3f v2);
float max(float a, float b);


/**
 * @class Ray3f
 * @brief represente un rayon lumineux
 */
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

/**
 * @class Camera
 * @brief gere le point de vue de l'utilisateur
 */
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

/**
 * @class Material
 * @brief contient les propriétés de couleur et de brillance
 */
class Material {
    public: 
        float r_;
        float g_;
        float b_; 
        float shininess_;
        Material couleur(Material reflet);
        
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

/**
 * @class Shape
 * @brief classe mère abstraite pour tous les objets de la scene
 */
class Shape {
    public: 
        Material matter_; 

        /**
         * @brief determine si le rayon touche l'objet
         * @param ray le rayon envoyé
         * @return une structure answer avec les détails 
         * renvoie un couple indiquant s'il y a intersection et si oui renvoie le point le plus proche sinon renvoie 0
         */
        virtual answer is_hit(Ray3f ray) =0 ;  
        
        /**
         * @brief calcul le rayon de reflexion
         */
        Ray3f reflect(Ray3f r, answer a);
};



/**
 * @class Cube
 * @brief forme de type cube ou pavé
 */
class Cube : public Shape{
    public: 
        Vector3f origin_; 
        Vector3f height_;
        Vector3f width_; 
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

        answer is_hit(Ray3f ray);
};

/**
 * @class Quad
 * @brief un simple quadrilatère dans l'espace
 */
class Quad : public Shape{
    public: 
        Vector3f origin_; 
        Vector3f width_;
        Vector3f height_;

        virtual ~Quad() {};
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


        answer is_hit(Ray3f ray);
        
        /** @brief check si le point est dans les limites du quad */
        bool est_dans_surf(Vector3f v);

};

/**
 * @class Sphere
 * @brief objet de type sphere
 */
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
        answer is_hit(Ray3f ray);

};

/**
 * @class Scene
 * @brief contient tous les objets et la lumiere
 */
class Scene {
    public: 
        Camera camera_; 
        std::vector<Shape*> shapes_;
        Ray3f source_;
        
        /** @brief crée une boite de type cornell box */
        void box(Vector3f centre, float w, float h, float d, Material m);
        
        /** @brief calcul l'intensité lumineuse au point d'impact */
        float intensite(answer a);
        
        void render(int width, int height, std::string filename);
};

// Fonctions de rendu globales
void draw_color(SDL_Renderer* rend, Material col, float intens);

/**
 * @brief fonction recursive pour les reflets
 * @param count nombre de rebonds maximums
 */
Material recursive(Ray3f ray, Scene scene, int count);
#endif