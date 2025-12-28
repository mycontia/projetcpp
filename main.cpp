
// qui affiche un dégradé : 
#include <stdio.h>
#include <stdbool.h>
#include <iostream>
#include "rayon.h"

int main(void) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    //taille de l'écran
    int y_taille = 640;
    int x_taille = y_taille;
    SDL_Window* window = SDL_CreateWindow("Ray Tracing MVP0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x_taille, y_taille, 0);
    // fenetre , SDL_WINDOWPOS_CENTERED 2 fois pour dire la position x et y où je veux que ma fenêtre soit, taille de la fenêtre large
    // puis haut, et 0 pour 0 options supplémentaires
    // Création du renderer pour pouvoir dessiner des points
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // renderer = pinceau , -1 c'est un truc par défaut, c'est une sorte d'outil utilisé pour dessiner, 
    //genre pinceau ou crayon, et SDL_RENDERER_ACCELERATED c'est pour utiliser le GPU au lieu du CPU
    //en fait c'est pour aller plus vite


    // avant while 
    //Camera cam(Vector3f(0,0,-100),Vector3f());

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

    Material rouge(255, 0, 0, 0);
    Material bleu(0, 0, 255, 0);
    Material vert(0, 255, 0, 0);

    Cube* c = new Cube(Vector3f(0.4f, 0.0f, 0.3f), h, w, bleu);
    Sphere* s = new Sphere(Vector3f(-0.5f, 0.0f, 1.0f), 0.5f, rouge);
    Quad* q = new Quad(Vector3f(0.5f, -0.5f, 2.0f), Vector3f(0.1f, 0.2f, 0.1f), Vector3f(0.4f, 0.1f, 0.1f), vert);
    Quad* qback = new Quad(
        Vector3f(0.0f, 0.0f, 2.0f), // Centre du quad
        Vector3f(0.0f, 2.0f, 0.0f),  // Sa hauteur (Y)
        Vector3f(2.0f, 0.0f, 0.0f),  // Sa longueur/profondeur (Z)
        vert
    );
    
    Quad* qleft = new Quad(
        Vector3f(-1.0f, 0.0f, 1.0f), // Centre du quad
        Vector3f(0.0f, 2.0f, 0.0f),  // Sa hauteur (Y)
        Vector3f(0.0f, 0.0f, 2.0f),  // Sa longueur/profondeur (Z)
        vert
    );
    Quad* qright = new Quad(
        Vector3f(1.0f, 0.0f, 1.0f), // Centre du quad
        Vector3f(0.0f, 2.0f, 0.0f),  // Sa hauteur (Y)
        Vector3f(0.0f, 0.0f, 2.0f),  // Sa longueur/profondeur (Z)
        vert
    );
    Quad* qup = new Quad(
        Vector3f(0.0f, 1.0f, 1.0f), // Centre du quad
        Vector3f(2.0f, 0.0f, 0.0f),  // Sa largeur (X)
        Vector3f(0.0f, 0.0f, 2.0f),  // Sa longueur/profondeur (Z)
        vert
    );
    Quad* qdown = new Quad(
        Vector3f(0.0f, -1.0f, 1.0f), // Centre du quad
        Vector3f(2.0f, 0.0f, 0.0f),  // Sa largeur (X)
        Vector3f(0.0f, 0.0f, 2.0f),  // Sa longueur/profondeur (Z)
        vert
    );
    Scene scene;
    scene.camera_ = Camera(Vector3f(0, 0, -5), Vector3f(0, 0, 1));
    scene.shapes_.push_back(c);
    scene.shapes_.push_back(s);
    scene.shapes_.push_back(q);
    scene.shapes_.push_back(qup);
    scene.shapes_.push_back(qdown);
    scene.shapes_.push_back(qleft);
    scene.shapes_.push_back(qright);
    scene.shapes_.push_back(qback);
    scene.source_ = Ray3f(Vector3f(5, 5, -5), Vector3f(0, 0, 1));


    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) running = false;
        }
        //je note tout ce qui se passe dans la fenetre sdl
        // si jamais il se passe un truc de type quit c est à dire la croix et bien on quitte


        // --- DEBUT DU DESSIN ---
        for (int y = 0; y < y_taille; y++) {
            for (int x = 0; x < x_taille; x++) {
                // Ici, je remplace plus tard par le calcul du rayon
                // On transforme x et y (0 à 640) en coordonnées centrées (-1.0 à 1.0)
                // pour que le centre de l'écran soit (0,0)
                //float coord_x = (x - x_taille/2) / 320.0f;
                //float coord_y = (y - y_taille/2) / 240.0f;
                float coord_x = (x - x_taille / 2.0f) / (x_taille / 2.0f);
                float coord_y = -(y - y_taille / 2.0f) / (y_taille / 2.0f);

/*
                int lignes = 400;
                int colonnes = 350;

                stv::vector<std::vector<pair>> v(lignes, std::vector<pair>(colonnes, 0.0f));
                for (int x = 0; x < x_taille; x++) {
                        v[x][0].first = x_taille/2;
                    for (int y = 1; y < y_taille; y++) {
                        v[x][y] = v[x][y-1] +1;
                

                for (int y = 0; y < y_taille; y++){
                    v[0][y].second = y_taille/2;
                    for (int x = 0; x < x_taille; x++) {
                        v[y][x] = v[y][x-1] +1;

                    }

                }        
*/                        

                // Création du rayon : 
                
                //Ray3f ray(cam.position_, Vector3f(coord_x, coord_y, 0));

                // La direction est : PointSurEcran - PositionCaméra
                //Vector3f direction = Vector3f(coord_x, coord_y, 0) - cam.position_;
                Vector3f direction = (Vector3f(coord_x, coord_y, -2.0f) - scene.camera_.position_).normalise();
                Ray3f ray(scene.camera_.position_, direction);

                // ça ça va changer, c'est à dire que le rayon va être projeté de la caméra ves le pixel.
                // donc l'origine sera bien (x,y,-50) par exemple
                // comme ça on ne change pas l'endroit 
                //bon en fait là je place la caméra en origine x y -10
                //bref c'est le 2.a ici
                

                // je veux en x en y et en z en fonction de sphere

                // si un rayon traverse une forme puis une autre à la sortie de la première on ne veut voir que la première touchée par le rayon, celle cachée ne devant pas apparaitre sur la fenetre 
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                //SDL_RenderClear(renderer); //JONATHAN 
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
                    draw_color(renderer, shape_proche->matter_, scene.intensite(pt_proche)); 
                }
                //SDL_SetRenderDrawColor(renderer, (x % 255), (y % 255), 150, 255);
                //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                // je veux que l'outil renderer = pinceau change de couleur
                // pour Rouge Vert Bleu et transparence
                SDL_RenderDrawPoint(renderer, x, y);
                // je veux que le pinceau se pose en x y
            }
        }
        SDL_RenderPresent(renderer); 
        // montrer le dessin ( il était caché)

        // --- FIN DU DESSIN ---
    }

    SDL_DestroyRenderer(renderer); // free
    SDL_DestroyWindow(window); // free
    SDL_Quit(); 
    return 0;
}


    

