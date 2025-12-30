/** 
 * @file main.cpp
 * @brief affiche l'image
 * @details Initialise SDL, crée de la scène (objets, caméra, lumière)
 * calcul d'ombres 



*/

// Bibliothèque
#include <stdio.h>
#include <stdbool.h>
#include <iostream>
#include "rayon.h"

/** 
 * @brief fonction main
 * @return 0 si succès -1 si erreur



*/


int main(void) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    //taille de l'écran
    int y_taille = 640;
    int x_taille = y_taille;

    // fenetre , SDL_WINDOWPOS_CENTERED 2 fois pour dire la position x et y où je veux que ma fenêtre soit, taille de la fenêtre large
    // puis haut, et 0 pour 0 options supplémentaires
    SDL_Window* window = SDL_CreateWindow("Ray Tracing MVP0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x_taille, y_taille, 0);
    
    // Création du renderer pour pouvoir dessiner des points
    // renderer = pinceau , -1 c'est un truc par défaut, c'est une sorte d'outil utilisé pour dessiner, 
    //genre pinceau ou crayon, et SDL_RENDERER_ACCELERATED c'est pour utiliser le GPU au lieu du CPU
    //en fait c'est pour aller plus vite
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    


    // avant la boucle while 
    

    //Vector3f h = Vector3f(0, 0.5, 0.425f);
    // Vector3f h = Vector3f(0, 0.5, 0.5f);
    // Vector3f w = Vector3f(0.5, 0.0, 0);
    Vector3f h = Vector3f(-0.15, 0.5, 0.5f);
    Vector3f w = Vector3f(0.5, 0.15, 0);
    try{

        if (prod_scal(h,w) != 0){
            throw "les axes largeurs et hauteurs du quadrilatère ne sont pas perpendiculaires !";
        } 
    }
    catch(const char* msg) {
        std::cout << "Erreur : " << msg << "\n";
        return -1;
    }

    Material rouge(255, 0, 0, 0.4f);
    Material bleu(0, 0, 255, 0.5f);
    Material vert(0, 255, 0, 0);
    Material jaune(255,255,0,0);

    Cube* c = new Cube(Vector3f(0.1f, 0.0f, 1.0f), h, w, bleu);
    Sphere* s = new Sphere(Vector3f(-0.8f, 0.0f, 0.3f), 0.5f, rouge);
    Quad* q = new Quad(Vector3f(0.6f, -0.1f, -1.0f), Vector3f(0.1f, 0.2f, 0.1f), Vector3f(0.4f, 0.1f, 0.1f), jaune);
    
    //if (camera_.position_ && source_.origin  not in box){}
    //throw 

    Scene scene;
    scene.box(Vector3f(-0.45f,0.0f,2.5f), 4.0f, 2.0f, 2.0f, vert);

/*

    Quad* qback = new Quad(
        Vector3f(0.0f, 0.0f, 2.0f), // Centre du quad
        Vector3f(0.0f, 2.0f, 0.0f),  // Sa hauteur (Y)
        Vector3f(2.0f, 0.0f, 0.0f),  // Sa longueur/profondeur (Z)
        vert
    );
    
    Quad* qleft = new Quad(
        Vector3f(-1.0f, 0.0f, 1.0f), 
        Vector3f(0.0f, 2.0f, 0.0f),  
        Vector3f(0.0f, 0.0f, 2.0f),  
        vert
    );

    Quad* qright = new Quad(
        Vector3f(1.0f, 0.0f, 1.0f), 
        //inversion des coordonnées par rapport à qleft
        // pour orienter le vecteur normal vers l'intérieur du "cube"
        Vector3f(0.0f, 0.0f, 2.0f),  
        Vector3f(0.0f, 2.0f, 0.0f),  
        vert
    );
    Quad* qup = new Quad(
        Vector3f(0.0f, 1.0f, 1.0f), 
        Vector3f(2.0f, 0.0f, 0.0f),  
        Vector3f(0.0f, 0.0f, 2.0f),  
        vert
    );
    Quad* qdown = new Quad(
        Vector3f(0.0f, -1.0f, 1.0f), 
        Vector3f(0.0f, 0.0f, 2.0f),  
        Vector3f(2.0f, 0.0f, 0.0f),  
        vert
    );
*/

    scene.camera_ = Camera(Vector3f(0.4, -0.5f, -5), Vector3f(0, 0, 1));
    scene.shapes_.push_back(c);
    scene.shapes_.push_back(s);
    scene.shapes_.push_back(q);


    /*
    scene.shapes_.push_back(qup);
    scene.shapes_.push_back(qdown);
    scene.shapes_.push_back(qleft);
    scene.shapes_.push_back(qright);
    scene.shapes_.push_back(qback);

*/    
    scene.source_ = Ray3f(Vector3f(0.4, -0.5f, -5), Vector3f(0, 0, 1));
    //scene.source_ = Ray3f(Vector3f(5, 5, -5), Vector3f(0, 0.7, 1));
    //scene.source_ = Ray3f(Vector3f(0, 10.99, 1), Vector3f(0, 0, 0));

    bool running = true;
    while (running) {
        SDL_Event e;
        //tant qu'il y a un evenement dans la fenetre sdl
        while (SDL_PollEvent(&e) != 0) {
            // si jamais il se passe un truc de type quit c est à dire la croix et bien on quitte
            if (e.type == SDL_QUIT) running = false;
        }
        
        // double boucle
        for (int y = 0; y < y_taille; y++) {
            for (int x = 0; x < x_taille; x++) {
                
                // On transforme x et y (0 à 640) en coordonnées centrées (-1.0 à 1.0)
                // pour que le centre de l'écran soit (0,0)
                
                float coord_x = (x - x_taille / 2.0f) / (x_taille / 2.0f);
                float coord_y = (y - y_taille / 2.0f) / (y_taille / 2.0f);

                       

                // Création du rayon : 
                // La direction est : PointSurEcran - PositionCaméra
                
                Vector3f direction = (Vector3f(coord_x, coord_y, -2.0f) - scene.camera_.position_).normalise();
                Ray3f ray(scene.camera_.position_, direction);
                // par défaut on met en noir
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

                float dist_min = INFINITY;
                Shape* shape_proche = nullptr; 
                answer pt_proche;

                for (size_t i =0; i< scene.shapes_.size(); i++){
                    answer rep = scene.shapes_[i]->is_hit(ray);
                    if (rep.hit) {
                        float dist = (rep.pt_inter - scene.camera_.position_).norme();
                        if (dist < dist_min) {
                            dist_min = dist;
                            shape_proche = scene.shapes_[i];
                            pt_proche = rep;
                        }
                    }
                }
                
                if (shape_proche != nullptr) {
                    // rayon d'ombre
                    Vector3f versLumiere = (scene.source_.origin_ - pt_proche.pt_inter).normalise();
                    //distance de mon objet à la source de lumière
                    float distSource = (scene.source_.origin_ - pt_proche.pt_inter).norme();
                    
                    // je rajoute un bout du vect norm pour éviter d'être hit par moi-même
                    Ray3f rayOmbre(pt_proche.pt_inter + (pt_proche.norm * 0.001f), versLumiere);
                    
                    bool estDansLombre = false;

                    // est ce que je hit un autre objet avant la lumière 
                    for (size_t j = 0; j < scene.shapes_.size(); j++) {
                        answer hitOmbre = scene.shapes_[j]->is_hit(rayOmbre);
                        //distance d'un obstacle à la source de lumière
                        float distObstacle = (scene.source_.origin_ - hitOmbre.pt_inter).norme();
                        if (hitOmbre.hit) {
                            if (distObstacle > 0.001f && distObstacle < distSource ) {
                                estDansLombre = true;
                                break; 
                            }
                            
                            
                        }
                    }

                    
                    if (estDansLombre) {
                        // intensité faible 
                        draw_color(renderer, shape_proche->matter_, 0.3f);
                    } else {
                        Vector3f direction = (Vector3f(coord_x, coord_y, -2.0f) - scene.camera_.position_).normalise();
                        Ray3f ray_init(scene.camera_.position_, direction);
                        Material c = recursive(ray_init, scene, 0);
                        SDL_SetRenderDrawColor(renderer,c.r_, c.g_, c.b_, 255);
                    }
                    
                }
                
                // je veux que le pinceau se pose en x y
                SDL_RenderDrawPoint(renderer, x, y);
                
            }
        }
        // montrer le dessin ( il était caché pour les calculs)
        SDL_RenderPresent(renderer); 
        

    }

    SDL_DestroyRenderer(renderer); // free
    SDL_DestroyWindow(window); // free
    SDL_Quit(); 
    return 0;
}


    

