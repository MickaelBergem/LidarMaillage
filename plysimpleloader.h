/*
 * Copyright 2014 Mickaël Bergem <mickael.bergem@ponts.org>
 */

#ifndef PLYSIMPLELOADER_H
#define PLYSIMPLELOADER_H

#include <string>
#include <vector>
#include "types.h"

class PLYSimpleLoader
{
public:
    PLYSimpleLoader(std::string ply_filename);
    const std::vector< Point > getcloud();
private:
    bool loadfile(std::string ply_filename);
    bool parse_header(std::string fichier);
    int nb_faces;
    int nb_vertices;
    std::vector <Point> points;
};

#endif // PLYSIMPLELOADER_H
