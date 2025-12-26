
// // qui affiche un dégradé : 
// #include <stdio.h>
// #include <stdbool.h>
// #include <SDL2/SDL.h>

// int main(void) {
//     SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
       

//     SDL_Window* window = SDL_CreateWindow("Ray Tracing MVP0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
//     // fenetre , SDL_WINDOWPOS_CENTERED 2 fois pour dire la position x et y où je veux que ma fenêtre soit, taille de la fenêtre large
//     // puis haut, et 0 pour 0 options supplémentaires
//     // Création du renderer pour pouvoir dessiner des points
//     SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//     // renderer = pinceau , -1 c'est un truc par défaut, c'est une sorte d'outil utilisé pour dessiner, 
//     //genre pinceau ou crayon, et SDL_RENDERER_ACCELERATED c'est pour utiliser le GPU au lieu du CPU
//     //en fait c'est pour aller plus vite

//     bool running = true;
//     while (running) {
//         SDL_Event e;
//         while (SDL_PollEvent(&e) != 0) {
//             if (e.type == SDL_QUIT) running = false;
//         }
//         //je note tout ce qui se passe dans la fenetre sdl
//         // si jamais il se passe un truc de type quit c est à dire la croix et bien on quitte


//         // --- DEBUT DU DESSIN ---
//         for (int y = 0; y < 480; y++) {
//             for (int x = 0; x < 640; x++) {
//                 // Ici, on remplace plus tard par le calcul du rayon
//                 SDL_SetRenderDrawColor(renderer, (x % 255), (y % 255), 150, 255);
//                 //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//                 //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
//                 // je veux que l'outil renderer = pinceau change de couleur
//                 // pour Rouge Vert Bleu et transparence
//                 SDL_RenderDrawPoint(renderer, x, y);
//                 // je veux que le pinceau se pose en x y
//             }
//         }
//         SDL_RenderPresent(renderer); 
//         // montrer le dessin ( il était caché)

//         // --- FIN DU DESSIN ---
//     }

//     SDL_DestroyRenderer(renderer); // free
//     SDL_DestroyWindow(window); // free
//     SDL_Quit(); 
//     return 0;
// }