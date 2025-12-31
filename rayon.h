#ifndef RAYON_H
#define RAYON_H
#include <iostream>
#include <cmath>
//ajout d'un include
#include <vector>
#include <SDL2/SDL.h>

/**
 * @class Vector3f
 * @brief Classe pour gérer les vecteurs en 3D.
 * * Contient les coordonées x, y, z et les opérations de base.
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

        /**
         * @return le vecteur résultant de l'addition de deux vecteurs.
         */
        Vector3f operator+ (Vector3f v1) const {  // const car on ne veut pas modifier le vecteur
            Vector3f v;
            v.x_ =  x_ + v1.x_;  
            v.y_ = y_ + v1.y_;
            v.z_ =  z_ + v1.z_;
            return v;
        }

        /**
         * @return le vecteur résultant de la soustraction de deux vecteurs.
         */
        Vector3f operator- (Vector3f v1) const {  
            Vector3f v;
            v.x_ =  x_ - v1.x_;  
            v.y_ = y_ - v1.y_;
            v.z_ =  z_ - v1.z_;
            return v;
        }

        /**
         * @return le vecteur résultant de la multiplication d'un vecteur avec un flottant.
         */
        Vector3f operator* (float a) const {  
            Vector3f v;
            v.x_ = a * x_;
            v.y_ = a * y_;
            v.z_ = a * z_;
            return v;
        }

        /**
         * @return le vecteur résultant de la multiplication de deux vecteurs.
         */
        Vector3f operator* (Vector3f v1) const {  
            Vector3f v;
            v.x_ = v1.x_ * x_;  
            v.y_ = v1.y_ * y_;
            v.z_ = v1.z_ * z_;
            return v;
        }


        /**
         * @brief Calcul la norme du vecteur.
         * @return la longeur du vecteur.
         */
        float norme () const{  
            float n;
            n = std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
            return n; 

        }

        /**
         * @brief renvoie un vecteur de norme 1.
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
 * @brief Stock les informations sur l'intersection.
 */
struct answer {
    bool hit;  ///<  vrai si le rayon intersecte une shape
    Vector3f pt_inter; ///< coordonnées du point d'intersection
    Vector3f norm; ///< normale à la surface
};



//  Prototypes des fonctions outils
/**
 * @brief Calcul le produit scalaire entre deux vecteurs.
 */
float prod_scal (Vector3f v1, Vector3f v2);

/**
 * @brief Calcul le produit vectoriel entre deux vecteurs.
 * @param v1 Le premier vecteur.
 * @param v2 Le deuxième vecteur.
 * @return Le produit vectoriel entre v1 et v2.
 */
Vector3f prod_vect(Vector3f v1, Vector3f v2);

/**
 * @brief Indique si deux vecteurs ont les mêmes coordonnées.
 */
bool egal (Vector3f v1, Vector3f v2);

/**
 * @brief Renvoie le maximum entre deux flottants.
 */
float max(float a, float b);


/**
 * @class Ray3f
 * @brief Represente un rayon lumineux.
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
 * @brief Gère le point de vue de l'utilisateur.
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
 * @brief Contient les propriétés de couleur et de brillance.
 */
class Material {
    public: 
        float r_; ///<  red.
        float g_; ///<  green.
        float b_; ///<  blue.
        float shininess_; ///<   coefficient de luminosité entre 0 (pas de réflection (objet mat)) et 1 (rayon entièrement réfléchi (miroir)).

        /**
        * @brief Indique si deux vecteurs ont les mêmes coordonnées.
        */
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
 * @brief Classe mère abstraite pour tous les objets de la scène.
 */
class Shape {
    public: 
        Material matter_; 
        Vector3f origin_; 

        /**
         * @brief determine si le rayon touche l'objet
         * @param ray le rayon envoyé.
         * @return une structure answer indiquant si le rayon a insecté une shape, les coordonnées du point d'intersection 
         * le plus proche avec l'origine du rayon ((0,0,0) s'il n'y a pas d'intersection) et le vecteur normal à ce point 
         * d'intersection ((0,0,0) s'il n'y a pas d'intersection)
         */
        virtual answer is_hit(Ray3f ray) =0 ;  
        
        /**
         * @brief calcul le rayon de réflexion
         * @return le rayon réfléchi par une surface si le coefficient de brillance est non nul, le vecteur d'origine et de direction nulle sinon
         */
        Ray3f reflect(Ray3f r, answer a);

        virtual ~Shape() {} 
};



/**
 * @class Cube
 * @brief Forme de type cube ou parallélépipède.
 */
class Cube : public Shape{
    public: 
        Vector3f origin_; 
        /** depth_ est calculé avec le produit vectoriel de width_ et height_ */
        Vector3f width_;
        Vector3f height_;
        
        Cube(){
            origin_ = Vector3f();
            width_ = Vector3f();
            height_ = Vector3f();
            matter_ = Material();
        }

        Cube(Vector3f o, Vector3f w, Vector3f h, Material m){
            origin_ = o;
            width_ = w;
            height_ = h;
            matter_ = m;
        }

        answer is_hit(Ray3f ray);
};

/**
 * @class Quad
 * @brief Un simple quadrilatère dans l'espace.
 */
class Quad : public Shape{
    public: 
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
        
        /** @brief Vérifie si un point qui appartient au même plan (infini donc) que le quad est dans les limites du quad (fini).
         * En effet pour savoir s'il y a intersection entre un rayon et un quad on considère d'abord le quad comme un plan infini avec lequel
         * on calcul l'intersection avec le rayon puis on vérifie si ce point d'intersection est contenu dans les limites du quad fini. 
        */
        bool est_dans_surf(Vector3f v);

};

/**
 * @class Sphere
 * @brief objet de type sphere
 */
class Sphere : public Shape{
    public: 
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
 * @brief Contient tous les objets et la lumiere.
 */
class Scene {
    public: 
        Camera camera_; 
        std::vector<Shape*> shapes_;
        Ray3f source_;
        
        /**
         * @brief Crée une boite ouverte face à l'utilisateur de type cornell box.
         * @param centre Les coordonnées du centre de la boite.
         * @param w la taille de la largeur de la boite.
         * @param h la taille de la hauteur de la boite.
         * @param d la taille de la profondeur de la boite.
         */
        void box(Vector3f centre, float w, float h, float d, Material m);
        
        /** @brief Calcul l'intensité lumineuse au point d'intersection.
         */
        float intensite(answer a);
        
        /**
         * @brief 
         * @param width 
         * @param height 
         * @param filename 
         */
        void render(int width, int height, std::string filename);
};

// Fonctions de rendu globales
void draw_color(SDL_Renderer* rend, Material col, float intens);

/**
 * @brief Fonction recursive pour les reflets.
 * @param count nombre de rebonds maximums.
 * @return la couleur finale après que le rayon se soit réfléchit au maximum count fois
 */
Material recursive(Ray3f ray, Scene scene, int count);

#endif