
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

// Calcule l'élévation d'un noeud à partir de celle de ses éventuels voisins
float compute_elevation_neighboor(vector <float> noeud1, vector <float> noeud2, vector <float> noeud3, vector <float> noeud4, float elevation_mini){
    float elevation = 0;
    int nb_pts = 0;
    
    if(noeud1.size() > 0){
        nb_pts += noeud1.size();
        elevation += std::accumulate(noeud1.begin(),noeud1.end(), 0.0);
    }
    if(noeud2.size() > 0){
        nb_pts += noeud2.size();
        elevation += std::accumulate(noeud2.begin(),noeud2.end(), 0.0);
    }
    if(noeud3.size() > 0){
        nb_pts += noeud3.size();
        elevation += std::accumulate(noeud3.begin(),noeud3.end(), 0.0);
    }
    if(noeud4.size() > 0){
        nb_pts += noeud4.size();
        elevation += std::accumulate(noeud4.begin(),noeud4.end(), 0.0);
    }
    
    if(nb_pts > 0){
        return (elevation / nb_pts);
    }
    else{
        return 0;
    }
}

int main(int argc, char **argv)
{
    cout << "LidarMaillage - Reconstruction d'un environnement 3D à partir de données LIDAR" << endl;
    
    if(argc != 3){ // On attend deux arguments
        cout << "Usage : " << argv[0] << " inputdata.ply max_number_edges" << endl;
        exit(0);
    }
    
    // Assignation des paramètres
    string ply_file = argv[1]; // Fichier PLY depuis lequel lire l'objet
    int N_max_edges = atoi(argv[2]); // Nombre maximal d'arrêtes
    
    // Variables statiques
    string pre_export_file = "pre-export.off";
    string final_export_file = "final-export.off";

    // Création d'une grille
    const int nb_pts_x = 170; // Nombre de points sur l'axe
    double extrem_x; // Borne (+/-) de l'axe
    const int nb_pts_y = 170; // Nombre de points sur l'axe
    double extrem_y; // Borne (+/-) de l'axe
    
    // Chargement du nuage de points
    PLYSimpleLoader cloud(ply_file, nb_pts_x, nb_pts_y);

    // Alignement du nuage sur la grille
    cloud.aligncloud();

    // Récupération du nuage
    vector <Point> nuage = cloud.getcloud();
    cout << "Chargé " << nuage.size() << " points." << endl;
    
    // Récupération de la forme du nuage
    ShapeCloud sc = cloud.getshape();
    extrem_x = sc.extrem_x;
    extrem_y = sc.extrem_y;
    double pas_x = sc.pas_x;
    double pas_y = sc.pas_y;
    cout << "Forme du nuage : -" << extrem_x << ":" << pas_x << ":" << extrem_x << endl;
    
    // Recherche du minimum
    float elevation_mini = min_element(nuage.begin(), nuage.end(), altitudeLess)->z();
    cout << "Altitude du plancher : " << elevation_mini << endl;
    
    // == Calcul de la carte d'élévation pour avoir un unique point sur chaque noeud de la grille ==
    // Création d'une matrice 2D de vecteurs (grille de vector des points correspondant au noeud)
    vector <float> matrice[nb_pts_x][nb_pts_y];
    float elevation[nb_pts_x][nb_pts_y];
    // Remplissage de la matrice
    int ix, iy;
    for(std::vector<Point>::iterator it = nuage.begin(); it != nuage.end(); ++it) {
        // TODO: la gestion des coordonnées est vraiment approximative, il suffit d'une coordonnée un peu en dehors et c'est fini...
        ix = it->x() + (nb_pts_x-1) / 2 ;
        iy = it->y() + (nb_pts_y-1) / 2 ;
//         cout << "(" << it->x() << ":" << it->y() << ") -> [" << ix << ":" << iy << "]" << endl; 
        matrice[ ix ][ iy ].push_back(it->z());
    }
    
    cout << "Création de la matrice d'élévation" << endl;
    // Création de la matrice finale d'élévation
    for(int idx = 0; idx < nb_pts_x; idx++) {
        for(int idy = 0; idy < nb_pts_y; idy++) {
            if(matrice[idx][idy].size() == 0){
                // Si on est pas sur un bord, on moyenne à partir des élévations voisines
                elevation[idx][idy] = elevation_mini;
                if(idx != 0 and idx != nb_pts_x-1 and idy != 0 and idy != nb_pts_y-1){
                    elevation[idx][idy] = compute_elevation_neighboor(matrice[idx+1][idy]
                                           ,matrice[idx][idy+1]
                                           ,matrice[idx-1][idy]
                                           ,matrice[idx][idy-1]
                                           , elevation_mini);
                }
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
    cout << "Simplification du maillage, jusqu'à " << N_max_edges << " arrêtes (" << (P.size_of_halfedges()/2) << " initialement)." << endl;
    SMS::Count_stop_predicate<Polyhedron> stop(N_max_edges); // On s'arrête quand il y a moins de N arrêtes
    int r = SMS::edge_collapse( P
                      , stop
                      , CGAL::vertex_index_map(boost::get(CGAL::vertex_external_index,P)).edge_index_map(boost::get(CGAL::edge_external_index  ,P)) 
    );
    
    std::cout << "Simplification terminée.\n" << r << " arrêtes ont été supprimées.\n" 
                << (P.size_of_halfedges()/2) << " arrêtes restantes.\n" ;
            
    // On exporte de nouveau pour le maillage simplifié
    ofstream fichier2(final_export_file.c_str());
    fichier2 << P;
    fichier2.close();
    
    std::cout << "Maillages exportés dans les fichiers " << pre_export_file << " et " << final_export_file << "." << std::endl;
    return 0;
}
