#include "rayon.h"
#include <iostream>

bool Quad::est_dans_surf(Vector3f v)
{
    // On projette le vecteur qui va du centre du quad au point d'intersection sur les deux axes du quad (width_ et height_)
    float dist_u = prod_scal(v, width_.normalise());
    float dist_v = prod_scal(v, height_.normalise());
    // On regarde si cette projection est dans les limites du quad
    float max_u = width_.norme() * 0.5f;
    float max_v = height_.norme() * 0.5f;
    return (((std::abs(dist_u) <= max_u) &&
             (std::abs(dist_v) <= max_v))); // Le point est à l'intérieur du quad ou non
    // LIB
}

answer Quad::is_hit(Ray3f ray)
{

    Vector3f depth_ = prod_vect(width_, height_);
    Vector3f depth_norm = depth_.normalise();
    answer reponse = {false, Vector3f(), Vector3f()}; // DEFAUT
    float prod_D = prod_scal(ray.direction_, depth_norm);

    /*
    si le produit scalaire entre la direction du rayon et la normale à une des 6 surfaces vaut 0 
    alors ces deux vecteurs sont perpendiculaires et donc le rayon est parallèle à la surface en question.
    */
    if (prod_D == 0)
    {
        return reponse;
    }
    // formule expliquée dans le rapport
    float t = prod_scal((origin_ - ray.origin_), depth_norm) / (prod_D); // distance entre l'origine du rayon et le point d'impact sur la face infinie du quad
    if (t <= 0) // intersection derrière la caméra
    { 
        return reponse;
    }
    Vector3f pt_inter = ray.origin_ +ray.direction_ *t; // point d'intersection entre le rayon et le plan infini contenant le quad
    Vector3f vect = pt_inter - origin_; // vecteur qui va du centre du quad au point d'intersection
    reponse.hit = est_dans_surf(vect);
    if (reponse.hit == true)
    {
        reponse.pt_inter = pt_inter;
        reponse.norm = depth_norm;
    }
    return (reponse);
}

answer Sphere::is_hit(Ray3f ray)
{
    answer reponse = {false, Vector3f(), Vector3f()}; // DEFAUT
    /*
    calcul du discrimant du polynome du second degré (a*x^2 + b*x + c = 0) que l'on obtient en
    combinant 2 égalités sur comment s'écrit un point appartenant à un rayon et la condtion pour
    que ce point appartienne à la sphère. 
    Formules expliquées dans le rapport.
    */
    float a = prod_scal(ray.direction_, ray.direction_);
    float b = 2 * prod_scal(ray.direction_, (ray.origin_ - origin_));
    float c = -2 * prod_scal(origin_, ray.origin_) + prod_scal(origin_, origin_) + prod_scal(ray.origin_, ray.origin_) - radius_ * radius_;
    float discriminent = b * b - 4 * a * c;
    if (discriminent == 0)
    { // un seul point d'intersection
        reponse.hit = true;
        float t = -b / (2 * a);
        reponse.pt_inter = Vector3f(ray.origin_ + ray.direction_ * t);
    }
    if (discriminent > 0) // deux points d'intersection
    { 
        reponse.hit = true;
        float t1 = (-b - std::sqrt(discriminent)) / (2 * a); // LIB
        float t2 = (-b + std::sqrt(discriminent)) / (2 * a); // LIB
        Vector3f p1 = Vector3f(ray.origin_ + ray.direction_ * t1);
        Vector3f p2 = Vector3f(ray.origin_ + ray.direction_ * t2);
        if ((p1 - ray.origin_).norme() > (p2 - ray.origin_).norme())
        {
            reponse.pt_inter = p2;
        }
        else
        {
            reponse.pt_inter = p1;
        }
    }
    if (discriminent >= 0)
    {
        reponse.norm =
            (reponse.pt_inter - origin_).normalise(); // le vecteur normal au plan de la sphère contenant le point d'intersection
    }
    return reponse;
}

bool egal(Vector3f v1, Vector3f v2)
{
    return ((v1.x_ == v2.x_) && (v1.y_ == v2.y_) && (v1.z_ == v2.z_));
};

answer Cube::is_hit(Ray3f ray)
{

    Vector3f depth = prod_vect(width_, height_);
    std::vector<Quad> q;
    // un cube est formé de 6 quads
    // l'ordre des vecteurs est chosi de sorte à ce que le produit vectoriel donne un vecteur normal orienté vers l'extérieur du cube
    q.push_back(Quad(origin_ + depth * (-0.5f), height_, width_, matter_)); // face de devant
    q.push_back(Quad(origin_ + depth * 0.5f, width_, height_, matter_));    // face de derrière
    q.push_back(Quad(origin_ + width_ * 0.5f, height_, depth, matter_));    // face de droite
    q.push_back(Quad(origin_ + width_ * (-0.5f), depth, height_, matter_)); // face de gauche
    q.push_back(Quad(origin_ + height_ * (-0.5f), width_, depth, matter_)); // face du haut
    q.push_back(Quad(origin_ + height_ * 0.5f, depth, width_, matter_));    // face du bas

    /*
    on pourrait se contenter de tester les faces jusqu'à ce qu'une soit en collision avec le
    rayon mais on veut aussi avoir les coordonnées du point le plus proche donc on va tester pour
    toutes les faces
    */
    answer pt_proche = {false, Vector3f(), Vector3f()}; // DEFAUT
    float dist_min = INFINITY;                          // LIB
    for (size_t i = 0; i < q.size(); i++)
    {
        answer pt_inter = q[i].is_hit(ray); // stocke s'il a intersection entre le rayon et le quadrilatère et en cas de collision contient aussi les coordonnées du point le plus proche ((0,0,0) sinon) ainsi que la nomale à la surface où il y a collision ((0,0,0) sinon))
        if (pt_inter.hit)
        {
            float dist = (pt_inter.pt_inter - ray.origin_).norme();
            // on cherche le point d'intersection le plus  proche de la caméra
            if (dist < dist_min)
            {
                dist_min = dist;
                pt_proche = pt_inter;
            }
        }
    }
    return pt_proche;
}

float prod_scal(Vector3f v1, Vector3f v2)
{
    float n = v1.x_ * v2.x_ + v1.y_ * v2.y_ + v1.z_ * v2.z_;
    return n;
}

Vector3f prod_vect(Vector3f v1, Vector3f v2)
{
    Vector3f v;
    v.x_ = v1.y_ * v2.z_ - v1.z_ * v2.y_;
    v.y_ = v1.z_ * v2.x_ - v1.x_ * v2.z_;
    v.z_ = v1.x_ * v2.y_ - v1.y_ * v2.x_;
    return v;
}

float max(float a, float b)
{
    if (a <= b)
    {
        return b;
    }
    return a;
}

float Scene::intensite(answer a)
{
    if (a.hit)
    {
        Vector3f v1 = a.norm;
        Vector3f v2 = (source_.origin_ - a.pt_inter).normalise(); // on considère la lumière comme un point ponctuel éclairant uniformément dans toutes les directions
        return max(0.0f,prod_scal(v1,v2)); // le produit scalaire de vecteurs normalisés est compris entre -1 et 1
        // si le produit scalaire est négatif, alors la lumière est derrière la surface donc l'objet est  noir.
        // v1 et v2 etant normalisés, le produit scalaire entre les deux est donc égal au cosinus de l'angle entre les 2 vecteurs et plus le cosinus est proche de 1 et plus le point d'intersection est éclairé
    }
    return 0;
}

void draw_color(SDL_Renderer* rend, Material col, float intens)
{
    // rend= pinceau change de couleur
    SDL_SetRenderDrawColor(rend, col.r_ * intens, col.g_ * intens, col.b_ * intens, 255);
}

Ray3f Shape::reflect(Ray3f r, answer a)
{
    Ray3f reflet = Ray3f();
    if (a.hit && (matter_.shininess_ != 0)) // si le coefficient de brillande vaut 0, l'objet ne réfléchit pas le rayon
    { 
        Vector3f direction_reflet = (r.direction_ + a.norm * ((-2) * prod_scal(r.direction_, a.norm))).normalise(); // formule expliquée dans le rapport
        reflet = Ray3f(a.pt_inter + a.norm * 0.001f,direction_reflet); 
        /*
        si avec les arrondis de calculs on a un pt_iter qui est légèrement décalé et à l'interieur de la shape, la
        réflection va donc avoir lieu sur la même shape donc on décale légèrement le point d'intersection pour
        compenser les erreurs de calculs sur les flottants
        */
    }
    return reflet;
}

Material Material::couleur(Material reflet)
{
    return Material((r_ * (1 - shininess_) + reflet.r_ * shininess_),(g_ * (1 - shininess_) + reflet.g_ * shininess_),(b_ * (1 - shininess_) + reflet.b_ * shininess_), 0);
}

Material Scene::recursive(Ray3f ray, int count)
{
    if (count > 7)
    {
        return Material(0, 0, 0, 0); // s'il a trop de rebonds
    }

    float dist_min = INFINITY; //LIB
    Shape* shape_proche = nullptr;
    answer pt_proche;
    /*
    Avant de colorier un point sur l'écran on veut vérifier si celui-ci n'est pas à l'ombre.
    Pour cela on regarde le rayon de la source vers ce point et on voit s'il intersecte un autre point que ce point.
    Si c'est le cas ce point est à l'ombre.
    De même pour l'intensité si le rayon ray (ayant par la suite pour direction : PointSurEcran - PositionCaméra et pour origine: celle de la caméra) traverse une shape puis une autre, on ne veut voir que la première shape touchée par le rayon, celle cachée ne devant pas apparaitre sur la fenêtre 
    c'est pourquoi nous allons chercher la première shape heurtée par ray. 
    */

    for (size_t i = 0; i < shapes_.size(); i++)
    {
        answer rep = shapes_[i]->is_hit(ray);
        if (rep.hit)
        {
            float dist = (rep.pt_inter - ray.origin_).norme();
            if (dist < dist_min && dist > 0.01f)
            { 
                dist_min = dist;
                shape_proche = shapes_[i];
                pt_proche = rep;
            }
        }
    }

    if (shape_proche != nullptr)
    {
        // rayon d'ombre
        Vector3f versLumiere = (source_.origin_ - pt_proche.pt_inter).normalise();
        // distance de mon objet à la source de lumière
        float distlum = (source_.origin_ - pt_proche.pt_inter).norme();

        // je rajoute un bout du vect norm pour éviter d'être hit par moi-même
        Ray3f rayOmbre(pt_proche.pt_inter + (pt_proche.norm * 0.001f), versLumiere);

        float intens = intensite(pt_proche);

        // est ce que je hit un autre objet avant la lumière
        for (size_t j = 0; j < shapes_.size(); j++)
        {
            answer hitOmbre = shapes_[j]->is_hit(rayOmbre);
            // distance d'un obstacle à la source de lumière
            float distobst = (source_.origin_ - hitOmbre.pt_inter).norme();
            if (hitOmbre.hit)
            {
                if (distobst > 0.001f && distobst < distlum)
                {
                    return Material(shape_proche->matter_.r_ * 0.3f * intens, shape_proche->matter_.g_ * 0.3f * intens, shape_proche->matter_.b_ * 0.3f * intens, 0.0f);
                }
            }
        }

        // On calcule la couleur de base avec l'intensité lumineuse
        float shininess = shape_proche->matter_.shininess_;
        Material couleur_obj(shape_proche->matter_.r_ * intens, shape_proche->matter_.g_ * intens,shape_proche->matter_.b_ * intens, shininess);

        if (shininess != 0) // il y a un rayon réfléchi
        {
            Ray3f rayon_reflet = shape_proche->reflect(ray, pt_proche);
            Material couleur_reflet = recursive(rayon_reflet, count + 1);

            // On mélange la couleur de l'objet et celle du reflet
            return couleur_obj.couleur(couleur_reflet);
        }

        return couleur_obj;
    }


    return Material(0, 0, 0, 0);//Si on ne touche rien, on renvoie la couleur noir
}

void Scene::box(Vector3f centre, float w, float h, float d, Material m)
{
    // repère othonormé
    Vector3f x(1, 0, 0);
    Vector3f y(0, 1, 0);
    Vector3f z(0, 0, 1);

    Quad* qback = new Quad(
        centre + z * (d * 0.5f), // Centre du quad
        y * h,
        x * w, // on fait cet ordre pour orienter le vecteur normal vers l'intérieur de la boite
        m);

    Quad* qleft = new Quad(
        centre + x * (-w * 0.5f),
        y * h,
        z * d,
        m);

    Quad* qright = new Quad(
        centre + x * (w * 0.5f),
        z * d, // inversion des coordonnées par rapport à qleft pour orienter le vecteur normal vers l'intérieur de la boite
        y * h, 
        m);

    Quad* qup = new Quad(
        centre + y * (-h * 0.5f),
        z * d,
        x * w,
        m);

    Quad* qdown = new Quad(
        centre + y * (h * 0.5f),
        x * w,
        z * d,
        m);

    shapes_.push_back(qup);
    shapes_.push_back(qdown);
    shapes_.push_back(qleft);
    shapes_.push_back(qright);
    shapes_.push_back(qback);
}

/** * @brief Capture l'écran actuel et le sauve en .bmp
 * @param rend le pinceau SDL
 * @param x largeur de la fenetre
 * @param y hauteur
 */
void sauvegarder_image(SDL_Renderer* rend, int x, int y, std::string fichier)
{
    // on crée une surface temporaire en RAM
    SDL_Surface* surface = SDL_CreateRGBSurface(0, x, y, 32, 0, 0, 0, 0);

    if (surface == nullptr)
    {
        std::cout << "Erreur surface : " << SDL_GetError() << "\n";
        return;
    }

    // on lit les pixels du GPU vers notre surface
    // le format ARGB8888 est souvent utilisé par SDL2
    SDL_RenderReadPixels(rend, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);

    // écriture du fichier (on met .bmp car c'est natif a SDL)
    if (SDL_SaveBMP(surface, fichier.c_str()) != 0)// c_str pour avoir le str du string
    { 
        std::cout << "Echec de la sauvegarde... : " << SDL_GetError() << "\n";
    }
    else
    {
        std::cout << "Image enregistrée avec succes dans scene.bmp !\n";
    }

    SDL_FreeSurface(surface);
}

void Scene::render(int x_taille, int y_taille, std::string fichier)
{
    
    Sdl ecran(x_taille, y_taille);

    Vector3f h = Vector3f(-0.15, 0.5, 0.5f);
    Vector3f w = Vector3f(0.5, 0.15, 0);
    try
    {

        if (prod_scal(h, w) != 0)
        { // on veut construire une boite en forme de parallélépipède rectangle donc avec des faces orthogonales
            throw "Les axes largeurs et hauteurs du quadrilatère ne sont pas perpendiculaires.";
        }
    }
    catch (const char* msg)
    {
        std::cout << "Erreur : " << msg << "\n";
        return;
    }

    // Création des couleurs avec un coefficient de brillance
    Material rouge(255, 0, 0, 0.5f);
    Material bleu(0, 0, 255, 0.4f);
    Material vert(0, 255, 0, 0.0f);
    Material jaune(255, 255, 0, 0.0f);

    // Création des shapes
    Cube* c = new Cube(Vector3f(0.1f, 0.0f, 0.0f), w, h, bleu);
    Sphere* s = new Sphere(Vector3f(-0.8f, 0.0f, 0.3f), 0.5f, rouge);
    Quad* q = new Quad(Vector3f(0.6f, -0.1f, 0.3f), Vector3f(0.1f, 0.2f, 0.1f), Vector3f(0.4f, 0.1f, 0.1f), jaune);

    shapes_.push_back(c);
    shapes_.push_back(s);
    shapes_.push_back(q);

    // Création de la boite
    box(Vector3f(-0.45f, 0.0f, 1.5f), 4.0f, 2.0f, 2.0f, vert);

    camera_ = Camera(Vector3f(0.4, -0.5f, -5.0f),Vector3f(0, 0,1)); // la caméra est devant la partie ouverte de la boite et regarde droit devant

    
    source_ = Ray3f(Vector3f(0, -0.9f, -1.0f), Vector3f(0, 1, 0)); // la source est à la position


    bool running = true;
    bool image_finale = false;
    while (running)
    {
        SDL_Event e;
        // tant qu'il y a un evenement dans la fenetre sdl
        while (SDL_PollEvent(&e) != 0)
        {
            // si jamais il se passe un évènement de type quit comme cliquer sur la croix , la fenetre se ferme
            if (e.type == SDL_QUIT)
                running = false;

            if (e.type == SDL_KEYDOWN)
            {
                float vitesse = 0.5f;
                switch (e.key.keysym.sym) // Déplacement de la LUMIÈRE (Source)
                {
                case SDLK_z:
                    source_.origin_.y_ -= vitesse;
                    break; // Monte (Y diminue)
                case SDLK_s:
                    source_.origin_.y_ += vitesse;
                    break; // Descend (Y augmente)
                case SDLK_q:
                    source_.origin_.x_ -= vitesse;
                    break; // Gauche (X diminue)
                case SDLK_d:
                    source_.origin_.x_ += vitesse;
                    break; // Droite (X augmente)
                case SDLK_e:
                    source_.origin_.z_ += vitesse;
                    break; // Fond (Z augmente)
                case SDLK_w:
                    source_.origin_.z_ -= vitesse;
                    break; // Vers nous (Z diminue)

                // mouvement des flèches pour la CAMÉRA
                case SDLK_UP:
                    camera_.position_.y_ += vitesse;
                    break;
                case SDLK_DOWN:
                    camera_.position_.y_ -= vitesse;
                    break;
                case SDLK_LEFT:
                    camera_.position_.x_ += vitesse;
                    break;
                case SDLK_RIGHT:
                    camera_.position_.x_ -= vitesse;
                    break;
                }
                image_finale = false;
                ecran.clear();
            }
        }

        if (!image_finale)
        {
            // double boucle
            for (int y = 0; y < y_taille; y++)
            {
                for (int x = 0; x < x_taille; x++)
                {

                    // On transforme x et y (0 à 640) en coordonnées centrées (-1.0 à 1.0) pour que le centre de l'écran soit (0,0)

                    float coord_x = (x - x_taille / 2.0f) / (x_taille / 2.0f);
                    float coord_y = (y - y_taille / 2.0f) / (y_taille / 2.0f);

                    // par défaut on met en noir
                    ecran.drawcolor(0, 0, 0, 255);

                    /*
                    Formule expliquée dans le rapport.
                    */

                    Vector3f direction = (Vector3f(coord_x, coord_y, -2.0f) - camera_.position_).normalise(); // Création du rayon dont la direction est : PointSurEcran - PositionCaméra
                    Ray3f ray_init(camera_.position_, direction);
                    // Material c = recursive(ray_init, this, 0);
                    Material c = recursive(ray_init, 0);
                    ecran.drawcolor(c.r_, c.g_, c.b_, 1);

                    // je veux que le pinceau se pose en x y
                    ecran.drawpoint(x, y);
                }
            }
            // montrer le dessin ( il était caché pour les calculs)
            sauvegarder_image(ecran.renderer, x_taille, y_taille, fichier);
            ecran.present();
            image_finale = true;
            SDL_Delay(10);
        }
    }
    //libérer la mémoire
    for (size_t i = 0; i < shapes_.size(); ++i)
    {
        delete shapes_[i];
    }
}