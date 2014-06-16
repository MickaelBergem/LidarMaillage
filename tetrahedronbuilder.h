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

#ifndef TETRAHEDRONBUILDER_H
#define TETRAHEDRONBUILDER_H

#include "types.h"
#include <CGAL/Polyhedron_incremental_builder_3.h>

typedef CGAL::Polyhedron_3<Kernel>         Polyhedron;
typedef Polyhedron::HalfedgeDS             HalfedgeDS;


// A modifier creating a triangle with the incremental builder.
template <class HDS>
class TetrahedronBuilder : public CGAL::Modifier_base<HDS> {
private:
    float *elevation;
    int nb_pts_x;
    int nb_pts_y;
public:
    TetrahedronBuilder(float* elevation, int nb_pts_x, int nb_pts_y) {
        this->elevation = elevation;
        this->nb_pts_x = nb_pts_x;
        this->nb_pts_y = nb_pts_y;
    }
    void operator()( HDS& hds) {
        
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
        
        B.begin_surface( this->nb_pts_x * this->nb_pts_y // Nombre de vertices
                        ,  (this->nb_pts_x-1) * (this->nb_pts_y-1) // Nombre de surfaces
                       );
        
        typedef typename HDS::Vertex   Vertex;
        typedef typename Vertex::Point Point;
        
        
        for(int idx = 0; idx < nb_pts_x; idx++) {
            for(int idy = 0; idy < nb_pts_y; idy++) {
                B.add_vertex( Point( idx, idy, elevation[idx*this->nb_pts_y + idy]));
            }
        }
//         B.begin_facet();
//         B.add_vertex_to_facet( 0);
//         B.add_vertex_to_facet( 1);
//         B.add_vertex_to_facet( 2);
//         B.end_facet();
        
        B.end_surface();
    };
};

#endif // TETRAHEDRONBUILDER_H
