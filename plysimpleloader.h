/*
 * Copyright 2014 Mickaël Bergem <mickael.bergem@ponts.org>
 */

#ifndef PLYSIMPLELOADER_H
#define PLYSIMPLELOADER_H

#include <string>

class PLYSimpleLoader
{
public:
    PLYSimpleLoader(std::string ply_filename);
    
private:
    void loadfile(std::string ply_filename);
    
};

#endif // PLYSIMPLELOADER_H
