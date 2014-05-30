/*
 * Copyright 2014 Mickaël Bergem <mickael.bergem@ponts.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "plysimpleloader.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <boost/algorithm/string.hpp>
#include <vector>

using namespace std;

PLYSimpleLoader::PLYSimpleLoader(std::string ply_filename)
{
    // Initialisation des attributs
    nb_faces = 0;
    nb_vertices = 0;
    points;
    
    // Chargement du fichier en mémoire
    loadfile(ply_filename);
}

const vector< Point > PLYSimpleLoader::getcloud()
{
    return points;
}


// Charger un fichier PLY
bool PLYSimpleLoader::loadfile(std::string ply_filename)
{
    ifstream fichier(ply_filename.c_str());

    if(fichier)
    {
        
        string ligne;
        
        if(getline(fichier, ligne) and ligne!="ply"){
            cerr << "Ce fichier n'est pas un fichier PLY (en-tête ply manquante) !" << endl;
            return false;
        }
        
        // On mouline les en-têtes
        while(getline(fichier, ligne) and parse_header(ligne));
        
        cout << "Fichier contenant " << nb_vertices << " vertices pour " << nb_faces << " face(s)." << endl;
        points.reserve(nb_vertices); // Allocation en mémoire du nombre de points prescrit
        
        vector <string> coords;
        while(getline(fichier, ligne)) // Lecture ligne par ligne
        {
            boost::split( coords, ligne, boost::is_any_of( " " ) );
            if(coords.size()!=4){
                cerr << "Coordonnée invalide trouvée, arrêt : " << ligne << endl;
                return false;
            }
            points.push_back(Point(strtof(coords[0].c_str(), NULL)
                                  ,strtof(coords[1].c_str(), NULL)
                                  ,strtof(coords[2].c_str(), NULL)));
        }
        
        fichier.close();  // on ferme le fichier
    }
    else  // Erreur à l'ouverture du fichier
        cerr << "Impossible d'ouvrir le fichier \"" << ply_filename << "\" !" << endl;


}

// Lecture et analyse des en-têtes
bool PLYSimpleLoader::parse_header(string ligne)
{
    char *tok = NULL;
    char *nom = NULL;
    char *valeur = NULL;
    tok = strtok(const_cast<char*>(ligne.c_str()), " ");
    
    if(strcmp(tok,"end_header") == 0){
//         cout << "Fin HEAD" << endl;
        return false;
    }
    
    if(strcmp(tok,"format") == 0){
        tok = strtok(NULL, " ");
        if(!tok or strcmp(tok,"ascii") != 0) {
            cerr << "Le format doit être ASCII !" << endl;
            return false;
        }
        return true;
    }
    
    if(strcmp(tok,"comment") == 0){
        cout << "Commentaire du fichier PLY : " << ligne.substr(8) << endl;
        return true;
    }
    
    if(strcmp(tok,"element") == 0){
        if((nom = strtok(NULL, " ")) and (valeur = strtok(NULL, " "))) {
            if(strcmp(nom,"face")==0){
                nb_faces = atoi(valeur);
            }
            if(strcmp(nom,"vertex")==0){
                nb_vertices = atoi(valeur);
            }
//             cout << "Balise élément inconnue : " << nom << " (" << valeur << ")..." << endl;
            return true;
        }
        cerr << "Balise element malformée : " << ligne << endl;
        return false;
    }
    
//     cout << "HEAD non pris en compte : " << tok << endl;
    return true;
}
