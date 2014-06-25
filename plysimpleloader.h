/*
 * Copyright 2014 Mickaël Bergem <mickael.bergem@ponts.org>
 */

#ifndef PLYSIMPLELOADER_H
#define PLYSIMPLELOADER_H

#include <string>
#include <vector>
#include "types.h"

// Forme du nuage
struct ShapeCloud{
    int nb_pts_x;
    int nb_pts_y;
    int extrem_x;
    int extrem_y;
    double pas_x;
    double pas_y;
    int nb_faces;
    int nb_vertices;
};

class PLYSimpleLoader
{
public:
    PLYSimpleLoader(std::string ply_filename
               , int _nb_pts_x, int _nb_pts_y);
    const std::vector< Point > getcloud();
    void aligncloud();
    ShapeCloud getshape();
private:
    bool loadfile(std::string ply_filename);
    bool parse_header(std::string fichier);
    int nb_faces;
    int nb_vertices;
    std::vector <Point> points;
    int nb_pts_x;
    int nb_pts_y;
    int extrem_x;
    int extrem_y;
    double pas_x;
    double pas_y;
};

#endif // PLYSIMPLELOADER_H
