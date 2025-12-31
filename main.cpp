/** * @file main.cpp
 * @brief affiche l'image
 * @details Initialise SDL, crée de la scène (objets, caméra, lumière)
 * calcul d'ombres 



*/

// Bibliothèque
#include <stdio.h>
#include <stdbool.h>
#include <iostream>
#include "rayon.h"



/** * @brief fonction main
 * @return 0 si succès 
*/


int main(void) {
    
    //taille de l'écran
    int y_taille = 640;
    int x_taille = y_taille;
    std::string fichier = "scene.bmp" ;

    Scene scene;
    scene.render(x_taille, y_taille, fichier);

    return 0;
}