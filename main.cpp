
#include <vector>
#include <list>
#include <iostream>
using namespace std;

#include "types.h"
#include "plysimpleloader.h"

int main(int argc, char **argv)
{
    cout << "LidarMaillage - Reconstruction d'un environnement 3D à partir de données LIDAR" << endl;
    
    string ply_file = "/home/mickael/Projets/MaillagesApplications/LidarMaillage/data.ply";
    
    // Création d'une grille
    int nb_pts_x = 200; // Nombre de points sur l'axe
    double extrem_x = 100; // Borne (+/-) de l'axe
    int nb_pts_y = 200; // Nombre de points sur l'axe
    double extrem_y = 100; // Borne (+/-) de l'axe
    
    // Chargement du nuage de points
    PLYSimpleLoader cloud(ply_file, nb_pts_x, nb_pts_y, extrem_x, extrem_y);
    
    // Alignement du nuage sur la grille
    cloud.aligncloud();
    
    // Récupération du nuage
    vector <Point> nuage = cloud.getcloud();
    cout << "Chargé " << nuage.size() << " points." << endl;
    
    
    return 0;
}
