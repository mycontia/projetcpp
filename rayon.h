#ifndef RAYON_H
#define RAYON_H
#include <cmath>
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

/**
 * @class Vector3f
 * @brief Classe pour gérer les vecteurs en 3D.
 * * Contient les coordonées x, y, z et les opérations de base.
 */
class Vector3f
{
  public:
    float x_;
    float y_;
    float z_;
    Vector3f()
    {
        x_ = 0;
        y_ = 0;
        z_ = 0;
    };
    Vector3f(float x, float y, float z)
    {
        x_ = x;
        y_ = y;
        z_ = z;
    }

    /**
     * @return le vecteur résultant de l'addition de deux vecteurs.
     */
    Vector3f operator+(Vector3f v1) const ///< const car on ne veut pas modifier le vecteur
    { 
        Vector3f v;
        v.x_ = x_ + v1.x_;
        v.y_ = y_ + v1.y_;
        v.z_ = z_ + v1.z_;
        return v;
    }

    /**
     * @return le vecteur résultant de la soustraction de deux vecteurs.
     */
    Vector3f operator-(Vector3f v1) const ///< const car on ne veut pas modifier le vecteur
    {
        Vector3f v;
        v.x_ = x_ - v1.x_;
        v.y_ = y_ - v1.y_;
        v.z_ = z_ - v1.z_;
        return v;
    }

    /**
     * @return le vecteur résultant de la multiplication d'un vecteur avec un flottant.
     */
    Vector3f operator*(float a) const ///< const car on ne veut pas modifier le vecteur
    {
        Vector3f v;
        v.x_ = a * x_;
        v.y_ = a * y_;
        v.z_ = a * z_;
        return v;
    }

    /**
     * @return le vecteur résultant de la multiplication de deux vecteurs.
     */
    Vector3f operator*(Vector3f v1) const ///< const car on ne veut pas modifier le vecteur
    {
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
    float norme() const ///< const car on ne veut pas modifier le vecteur
    {
        float n;
        n = std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
        return n;
    }

    /**
     * @brief renvoie un vecteur de norme 1.
     */
    Vector3f normalise() const ///< const car on ne veut pas modifier l'objet courant 
    {
        float n = norme();
        Vector3f v = Vector3f(0, 0, 0);
        if (n > 0)
        {
            float m = 1.0f / n;
            v.x_ = x_ * m;
            v.y_ = y_ * m;
            v.z_ = z_ * m;
        }
        return v;
    }
};

/**
 * @struct answer
 * @brief Stock les informations sur l'intersection.
 */
struct answer
{
    bool hit; ///<  vrai si le rayon intersecte une shape
    Vector3f pt_inter; ///< coordonnées du point d'intersection s'il existe, (0,0,0) sinon
    Vector3f norm; ///< normale à la surface, (0,0,0) sinon
};

//  Prototypes des fonctions outils
/**
 * @brief Calcul le produit scalaire entre deux vecteurs.
 */
float prod_scal(Vector3f v1, Vector3f v2);

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
bool egal(Vector3f v1, Vector3f v2);

/**
 * @brief Renvoie le maximum entre deux flottants.
 */
float max(float a, float b);

/** * @brief Capture l'ecran actuel et le sauve en .bmp.
 * @param rend le pinceau SDL.
 * @param x largeur de la fenetre.
 * @param y hauteur.
 */
void sauvegarder_image(SDL_Renderer* rend, int x, int y, std::string fichier);

/**
 * @class Ray3f
 * @brief Represente un rayon lumineux.
 */
class Ray3f
{
  public:
    Vector3f origin_;
    Vector3f direction_;
    Ray3f()
    {
        origin_ = Vector3f();
        direction_ = Vector3f();
    }
    Ray3f(Vector3f o, Vector3f d)
    {
        origin_ = o;
        direction_ = d;
    }
};

/**
 * @class Camera
 * @brief Gère le point de vue de l'utilisateur.
 */
class Camera
{
  public:
    Vector3f position_;
    Vector3f direction_;
    Camera()
    {
        position_ = Vector3f();
        direction_ = Vector3f();
    }

    Camera(Vector3f p, Vector3f d)
    {
        position_ = p;
        direction_ = d;
    }
};

/**
 * @class Material
 * @brief Contient les propriétés de couleur et de brillance.
 */
class Material
{
  public:
    float r_; ///<  rouge
    float g_; ///<  vert
    float b_; ///<  bleu
    float shininess_; ///<   coefficient de luminosité entre 0 (pas de réflection (objet mat)) et 1 (rayon entièrement réfléchi (miroir)).

    /**
     * @return le mélange de couleurs en prenant en compte le phénomène de réflexion.
     */
    Material couleur(Material reflet);

    Material()
    {
        r_ = 0;
        g_ = 0;
        b_ = 0;
        shininess_ = 0;
    }
    Material(float r, float g, float b, float s)
    {
        r_ = r;
        g_ = g;
        b_ = b;
        shininess_ = s;
    }
};

/**
 * @class Shape
 * @brief Classe mère abstraite pour tous les objets de la scène.
 */
class Shape
{
  public:
    Material matter_;
    Vector3f origin_;

    /**
     * @brief determine si le rayon touche l'objet
     * @param ray le rayon envoyé.
     * @return une structure answer indiquant si le rayon a insecté une shape, les coordonnées du point d'intersection le plus proche avec l'origine du rayon ((0,0,0) s'il n'y a pas
     * d'intersection) et le vecteur normal à ce point d'intersection ((0,0,0) s'il n'y a pas d'intersection)
     */
    virtual answer is_hit(Ray3f ray) = 0;

    /**
     * @brief Calcul le rayon de réflexion.
     * @return le rayon réfléchi par une surface si le coefficient de brillance est non nul, le vecteur d'origine et de direction nulle sinon.
     */
    Ray3f reflect(Ray3f r, const answer& a); ///< a est passé par référence car c'est une structure plus grosse qu'un simple Vector3f

    virtual ~Shape() {}
};

/**
 * @class Cube
 * @brief Forme de type cube ou parallélépipède.
 */
class Cube : public Shape
{
  public:
    Vector3f origin_;
    /** depth_ est calculé avec le produit vectoriel de width_ et height_ */
    Vector3f width_;
    Vector3f height_;

    Cube()
    {
        origin_ = Vector3f();
        width_ = Vector3f();
        height_ = Vector3f();
        matter_ = Material();
    }

    Cube(Vector3f o, Vector3f w, Vector3f h, Material m)
    {
        origin_ = o;
        width_ = w;
        height_ = h;
        matter_ = m;
    }
/**
 * @copydoc Shape::is_hit
 * @details Un cube est composé de 6 quads donc on va se servir de ce que l'on a codé pour quad.
 */
    answer is_hit(Ray3f ray);
};

/**
 * @class Quad
 * @brief Un simple quadrilatère dans l'espace.
 */
class Quad : public Shape
{
  public:
    Vector3f width_;
    Vector3f height_;

    Quad()
    {
        origin_ = Vector3f();
        width_ = Vector3f();
        height_ = Vector3f();
        matter_ = Material();
    }

    Quad(Vector3f o, Vector3f w, Vector3f h, Material m)
    {
        origin_ = o;
        width_ = w;
        height_ = h;
        matter_ = m;
    }

    /**
     * @copydoc Shape::is_hit
     * @details On associe le quad à un plan infini et on calcule l'intersection entre le rayon et ce plan ; puis à l'aide de la méthode est_dans_surf
     * on regarde si ce point d'intersection trouvée respecte les constraintes spatiales du quad.
     */
    answer is_hit(Ray3f ray);

    /** @brief Vérifie si un point qui appartient au même plan (infini donc) que le quad est dans les limites du quad (fini). 
     * En effet pour savoir s'il y a intersection entre un rayon et un quad on considère d'abord le quad comme un plan infini avec lequel on calcul l'intersection
     * avec le rayon puis on vérifie si ce point d'intersection est contenu dans les limites du quad fini.
     */
    bool est_dans_surf(Vector3f v);
};

/**
 * @class Sphere
 * @brief objet de type sphere
 */
class Sphere : public Shape
{
  public:
    float radius_;

    Sphere()
    {
        origin_ = Vector3f();
        radius_ = 0;
        matter_ = Material();
    }

    Sphere(Vector3f o, float r, Material m)
    {
        origin_ = o;
        radius_ = r;
        matter_ = m;
    }
    /**
    * @copydoc Shape::is_hit
    */
    answer is_hit(Ray3f ray);
};

/**
 * @class Scene
 * @brief Contient tous les objets et la lumiere.
 */
class Scene
{
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
    float intensite(const answer& a ); ///< a est passé par référence car c'est une structure plus grosse qu'un simple Vector3f

    /**
     * @brief Fonction recursive pour les reflets.
     * @param count nombre de rebonds maximums.
     * @details Nous avons codé dans la méthode recursive à la fois le calcul de l'ombre et celui de la réflexion car toutes deux passent par la recherche 
     * de la plus proche shape intersectée. Là où la réflexion s'appelle récursivement, ce n'est pas le cas pour l'ombre d'où la présence d'un compteur qui 
     * permet non seulement de ne pas appelé la fonction récursive un nombre infini de fois (dans le cas ou deux miroirs se feraient face par exemple) mais 
     * aussi de rajouter une condition sur ce compteur afin que seule la partie sur la réflexion soit récursive et que celle sur l'ombre ne soit appelé qu'une fois pour chaque rayon. 

     * @return la couleur finale après que le rayon se soit réfléchit au maximum count fois.
     */
    Material recursive(Ray3f ray, int count);

    /**
     * @brief
     * @param width
     * @param height
     * @param filename
     */
    void render(int x_taille, int y_taille, std::string fichier);
};



class Sdl {
public:
    
    //les paramètres pour la SDL
    SDL_Window* window;
    SDL_Renderer* renderer; 
    int x_taille;
    int y_taille;

    /**
     * @brief crée une fenêtre de la bonne taille
     * @param x La longueur de la fenêtre.
     * @param y La hauteur de la fenêtre.
     */
    Sdl(int x, int y) {
        x_taille =x;
        y_taille=y;
        init(); 
        // le premier argument est le nom de lafenetre , 
        // on a SDL_WINDOWPOS_CENTERED 2 fois pour dire la position de x et de y c'est à dire la position de la fenêtre, 
        //on a 2 arguments pour la taille de la fenêtre , et enfin 0 pour dire qu'il y a 0 options supplémentaires
        window = SDL_CreateWindow("Ray Tracing MVP0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x_taille, y_taille, 0);
        // Création du renderer pour pouvoir dessiner des points
        // le renderer est comme un pinceau c'est un outil pour dessiner des points
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }

    /**
     * @brief Destructeur, on libère la mémoire
     */
    ~Sdl() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    /**
     * @brief Initialise la SDL.
     */
    void init() { SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER); }
    
    /**
     * @brief On choisit les couleurs à mettre sur le renderer.
     */
    void drawcolor(int r, int g, int b, int a) { 
        SDL_SetRenderDrawColor(renderer, r, g, b, a); 
    }
    /**
     * @brief On choisit le point où le renderer va agir.
     */
    void drawpoint(int x, int y) { 
        SDL_RenderDrawPoint(renderer, x, y); 
    }

    /**
     * @brief Affiche ce qui a été dessiné.
     */
    void present() { SDL_RenderPresent(renderer); }
    

    /**
     * @brief Efface ce qui a été dessiné
     */
    void clear() {
        drawcolor(0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }
};

#endif