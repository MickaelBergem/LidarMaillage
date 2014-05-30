
#include <vector>
#include <list>
#include <iostream>
using namespace std;

#include "plysimpleloader.h"

int main(int argc, char **argv)
{
    cout << "Coucou" << endl;
    
    string ply_file = "/home/mickael/Projets/MaillagesApplications/LidarMaillage/data.ply";
    
    PLYSimpleLoader cloud(ply_file);
  
    return 0;
}
