#ifndef _My_C3T3_
#define _My_C3T3_

#include <CGAL/basic.h>

#include <QtOpenGL>
#include <QtOpenGL/qgl.h>

#include <CGAL/Mesh_complex_3_in_triangulation_3.h>


#undef min
#undef max


template < class Kernel, class Tr >
class C3T3 : public CGAL::Mesh_complex_3_in_triangulation_3<Tr> 
{
public:
	typedef C3T3<Kernel, Tr> C3t3;

// 	typedef typename Kernel::FT            FT;
	typedef typename Kernel::Point_3       Point;
// 	typedef typename Kernel::Vector_3      Vector;
// 	typedef typename Kernel::Segment_3     Segment;
// 	typedef typename Kernel::Line_3        Line;
// 	typedef typename Kernel::Triangle_3    Triangle;
// 	typedef typename Kernel::Tetrahedron_3 Tetrahedron;
// 
// 	typedef typename CGAL::Mesh_complex_3_in_triangulation_3<Tr> Mesh3;
// 	typedef typename C3t3::Triangulation Triangulation;
// 	typedef typename Triangulation::Vertex_handle Vertex_handle;
// 	typedef typename Triangulation::Edge Edge;
// 	typedef typename Triangulation::Cell_handle Cell_handle;

public:
	C3T3() {}
	~C3T3() {}

public:
/*
	Vertex_handle get_source_vertex(const Edge& edge) const
	{
		return edge.first->vertex(edge.second);
	}

	Vertex_handle get_target_vertex(const Edge& edge) const
	{
		return edge.first->vertex(edge.third);
	}

	// RENDERING
	void gl_vertex(const Point& p)
	{
		::glVertex3d(p.x(),p.y(),p.z());
	}

	void render_edges(const float line_width,
		const unsigned char red,
		const unsigned char green,
		const unsigned char blue)
	{
		::glLineWidth(line_width);        
		::glColor3ub(red,green,blue);
		::glBegin(GL_LINES);

		Triangulation& tr = Mesh3::triangulation();

		typename Triangulation::Finite_edges_iterator e;
		for (e = tr.finite_edges_begin(); e != tr.finite_edges_end(); e++)
		{
			typename Triangulation::Edge edge = *e;
			gl_vertex(get_source_vertex(edge)->point());
			gl_vertex(get_target_vertex(edge)->point());
		}
		::glEnd();
	}

	void render_vertices(const float point_size,
		const unsigned char red,
		const unsigned char green,
		const unsigned char blue)
	{
		::glPointSize(point_size);
		::glColor3ub(red, green, blue);

		::glBegin(GL_POINTS);
		Triangulation& tr = Mesh3::triangulation();
		typename Triangulation::Finite_vertices_iterator v;
		for (v = tr.finite_vertices_begin(); v != tr.finite_vertices_end(); v++)
			gl_vertex(v->point());
		::glEnd();
	}

	void render_cells(const FT xcut)
	{
		::glEnable(GL_CULL_FACE);
		::glCullFace(GL_BACK);
		::glColor3ub(128, 128, 200);
		::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
		::glEnable(GL_LIGHTING);

		::glBegin(GL_TRIANGLES);
		Triangulation& tr = Mesh3::triangulation();
		typename Triangulation::Finite_cells_iterator ci;
		for(ci = tr.finite_cells_begin(); ci != tr.finite_cells_end(); ci++)
		{
			if(!Mesh3::is_in_complex(ci))
				continue;

			const Point& a = ci->vertex(0)->point();
			const Point& b = ci->vertex(1)->point();
			const Point& c = ci->vertex(2)->point();
			const Point& d = ci->vertex(3)->point();

			Point cc = CGAL::centroid(a, b, c, d);
			if(cc.x() > xcut)
				continue;

			gl_shaded_triangle(a, b, c);
			gl_shaded_triangle(a, b, d);
			gl_shaded_triangle(a, d, c);
			gl_shaded_triangle(b, c, d);
		}
		::glEnd();

		::glDisable(GL_LIGHTING);
	}

	void gl_shaded_triangle(const Point& a, const Point& b, const Point& c)
	{
		// compute normal
		Vector n = CGAL::cross_product(c-a, b-a);
		n = n / std::sqrt(n*n);

		// draw one front facing
		::glNormal3d(-n.x(), -n.y(), -n.z());
		::glVertex3d(a.x(), a.y(), a.z());
		::glVertex3d(b.x(), b.y(), b.z());
		::glVertex3d(c.x(), c.y(), c.z());

		// and the other back facing
		::glNormal3d(n.x(), n.y(), n.z());
		::glVertex3d(a.x(), a.y(), a.z());
		::glVertex3d(c.x(), c.y(), c.z());
		::glVertex3d(b.x(), b.y(), b.z());
	}*/

};

#endif // _My_C3T3_


