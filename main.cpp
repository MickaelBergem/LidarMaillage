
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
using namespace std;

#include "types.h"
#include "plysimpleloader.h"
#include "polyhedronbuilder.h"

#include <CGAL/IO/Polyhedron_iostream.h>

// Adaptor for Polyhedron_3
#include <CGAL/Surface_mesh_simplification/HalfedgeGraph_Polyhedron_3.h>
// Simplification function
#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
// Stop-condition policy
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_stop_predicate.h>

namespace SMS = CGAL::Surface_mesh_simplification ;

// Retourne vrai si pt1 est plus bas que pt2
bool altitudeLess (const Point pt1, const Point pt2)
{
    return pt1.z() < pt2.z();
}

int main(int argc, char **argv)
{
    cout << "LidarMaillage - Reconstruction d'un environnement 3D à partir de données LIDAR" << endl;

    string ply_file = "/home/mickael/Projets/MaillagesApplications/LidarMaillage/data.ply";
    string pre_export_file = "pre-export.off";
    string final_export_file = "final-export.off";

    // Création d'une grille
    const int nb_pts_x = 200; // Nombre de points sur l'axe
    const double extrem_x = 100; // Borne (+/-) de l'axe
    const int nb_pts_y = 200; // Nombre de points sur l'axe
    const double extrem_y = 100; // Borne (+/-) de l'axe

    // Nombre maximal d'arrêtes
    int N_max_edges = 10000;
    
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
    // Recherche du minimum
    float elevation_mini = min_element(nuage.begin(), nuage.end(), altitudeLess)->z();
    
    
    
//     bool elevation_mini_init = false;
//     for(int idx = 0; idx < nb_pts_x; idx++) {
//         for(int idy = 0; idy < nb_pts_y; idy++) {
//             cout << *min_element(matrice[idx][idy].begin() , matrice[idx][idy].end()) << endl;
// //             if(!elevation_mini_init or elevation_mini > *min_element(matrice[idx][idy].begin() , matrice[idx][idy].end()) ){
// //                 elevation_mini = *min_element(matrice[idx][idy].begin() , matrice[idx][idy].end());
// //                 elevation_mini_init = true;
// //             }
//         }
//     }
    cout << "Altitude du plancher : " << elevation_mini << endl;
    
    // Création de la matrice finale d'élévation
    for(int idx = 0; idx < nb_pts_x; idx++) {
        for(int idy = 0; idy < nb_pts_y; idy++) {
            if(matrice[idx][idy].size() == 0){
                
                elevation[idx][idy] = elevation_mini;
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
    ofstream fichier(pre_export_file.c_str());
    fichier << P;
    fichier.close();
    
    // Simplification du maillage
    cout << "Simplification du maillage, jusqu'à " << N_max_edges << " arrêtes." << endl;
    SMS::Count_stop_predicate<Polyhedron> stop(N_max_edges); // On s'arrête quand il y a moins de N arrêtes
    int r = SMS::edge_collapse( P
                      , stop
                      , CGAL::vertex_index_map(boost::get(CGAL::vertex_external_index,P)).edge_index_map(boost::get(CGAL::edge_external_index  ,P)) 
    );
    
    std::cout << "Simplification terminée.\n" << r << " edges removed.\n" 
                << (P.size_of_halfedges()/2) << " final edges.\n" ;
            
    // On exporte de nouveau pour le maillage simplifié
    ofstream fichier2(final_export_file.c_str());
    fichier2 << P;
    fichier2.close();
    
    std::cout << "Maillages exportés dans les fichiers " << pre_export_file << " et " << final_export_file << "." << std::endl;
    return 0;
}
