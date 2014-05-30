#ifndef TYPES_H
#define TYPES_H

// kernel
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

// basic types from kernel
typedef Kernel::FT FT;
// typedef CGAL::Bbox_3 Bbox;
typedef Kernel::Point_3 Point;
// typedef Kernel::Sphere_3 Sphere;
// typedef Kernel::Vector_3 Vector;
// typedef Kernel::Segment_3 Segment;
// typedef Kernel::Triangle_3 Triangle;

// 3D mesh generation
// from http://www.cgal.org/Manual/latest/doc_html/cgal_manual/Mesh_3/Chapter_main.html
#include "c3t3.h"
// #include <CGAL/Mesh_triangulation_3.h>
// #include <CGAL/Mesh_complex_3_in_triangulation_3.h>
// #include <CGAL/Mesh_criteria_3.h>
// #include <CGAL/Implicit_mesh_domain_3.h>
// #include <CGAL/make_mesh_3.h>
// 
// 
// // implicit function
// typedef FT (Function) (const Point&);
// typedef CGAL::Implicit_mesh_domain_3<Function, Kernel> Mesh_domain;
// 
// typedef CGAL::Mesh_triangulation_3<Mesh_domain>::type Tr;
// typedef C3T3<Kernel, Tr> C3t3;
// 
// typedef CGAL::Mesh_criteria_3<Tr> Mesh_criteria;

// using namespace CGAL::parameters;

#endif // TYPES_H


