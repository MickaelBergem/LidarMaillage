
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
using namespace std;

#include "types.h"
#include "plysimpleloader.h"
#include "polyhedronbuilder.h"

#include <CGAL/IO/Polyhedron_iostream.h>

int main(int argc, char **argv)
{
    cout << "LidarMaillage - Reconstruction d'un environnement 3D à partir de données LIDAR" << endl;

    string ply_file = "/home/mickael/Projets/MaillagesApplications/LidarMaillage/data.ply";
    string export_file = "export.off";

    // Création d'une grille
    const int nb_pts_x = 200; // Nombre de points sur l'axe
    const double extrem_x = 100; // Borne (+/-) de l'axe
    const int nb_pts_y = 200; // Nombre de points sur l'axe
    const double extrem_y = 100; // Borne (+/-) de l'axe

    // Chargement du nuage de points
    PLYSimpleLoader cloud(ply_file, nb_pts_x, nb_pts_y, extrem_x, extrem_y);

    // Alignement du nuage sur la grille
    cloud.aligncloud();

    // Récupération du nuage
    vector <Point> nuage = cloud.getcloud();
    cout << "Chargé " << nuage.size() << " points." << endl;

    // == Calcul de la carte d'élévation pour avoir un unique point sur chaque noeud de la grille ==
    // Création d'une matrice 2D de vecteurs (grille de vector des points correspondant au noeud)
    vector <float> matrice[nb_pts_x][nb_pts_y];
    float elevation[nb_pts_x][nb_pts_y];
    // Remplissage de la matrice
    int ix, iy;
    for(std::vector<Point>::iterator it = nuage.begin(); it != nuage.end(); ++it) {
        // TODO: la gestion des coordonnées est vraiment approximative, il suffit d'une coordonnée un peu en dehors et c'est fini...
        ix = it->x() + extrem_x;
        iy = it->y() + extrem_y;
        matrice[ ix ][ iy ].push_back(it->z());
    }
    // Création de la matrice finale d'élévation
    for(int idx = 0; idx < nb_pts_x; idx++) {
        for(int idy = 0; idy < nb_pts_y; idy++) {
            if(matrice[idx][idy].size() == 0){
                // TODO
                elevation[idx][idy] = 0;
            }
            else{
                elevation[idx][idy] = std::accumulate(matrice[idx][idy].begin(),matrice[idx][idy].end(), 0.0) / matrice[idx][idy].size();
            }
        }
    }
    
    // Création du Polyhedron
    Polyhedron P;
    PolyhedronBuilder<HalfedgeDS> map(&elevation[0][0], nb_pts_x, nb_pts_y);
    
    P.delegate( map);
    
    // Export du maillage au format .off dans le fichier spécifié
    ofstream fichier(export_file.c_str());
    fichier << P;
    fichier.close();
    
    return 0;
}
