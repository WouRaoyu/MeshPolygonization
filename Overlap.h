#pragma once

#include "Utils.h"
#include "Segment.h"
#include "Draw.h"


// Project segment faces to 2D
inline std::vector<Polygon_2> project_segment_faces(const Mesh* mesh, unsigned int id, Plane_3* plane) {
	std::vector<Polygon_2> polygons;

	std::vector<Vertex> vertices;
	VProp_geom geom = mesh->points();
	std::vector<Point_2> points;

	// Iterate segment faces
	for (auto face : select_segment(mesh, id)) {
		// Retrieve face vertices
		vertices = vertex_around_face(mesh, face);

		// Project face points to 2D
		for (auto vertex : vertices) {
			points.push_back(plane->to_2d(geom[vertex]));
		}

		// Construct 2D polygon
		Polygon_2 polygon(points.begin(), points.end());

		// Ensure COUNTERCLOCKWISE order
		if (polygon.is_clockwise_oriented()) { polygon.reverse_orientation(); }

		// Add triangle
		polygons.push_back(polygon);
		points.clear();
	}

	return polygons;
}


// Check overlap
inline bool check_overlap(Polygon_2* polygon, std::vector<Polygon_2>* faces) {
	double area = 0;

	// Iterate segment faces
	for (auto face : *faces) {
		
		// Check for bbox overlap
		if (do_overlap(polygon->bbox(), face.bbox())) {
			bool is_inside = true;
			std::vector<Point_2> points;

			// Check if face is inside polygon
			Polygon_2::Vertex_const_iterator v;
			// For every face vertex
			for (v = face.vertices_begin(); v != face.vertices_end(); ++v) {
				// Check its position with respect to the polygon
				auto check = CGAL::bounded_side_2(polygon->vertices_begin(), polygon->vertices_end(), *v);

				// If it is inside or on the boundary, store it
				if (check == CGAL::ON_BOUNDED_SIDE || check == CGAL::ON_BOUNDARY) { points.push_back(*v); }

				// If a vertex is out of polygon, mark whole face as outside
				else { is_inside = false; }
			}

			// If face is inside
			if (is_inside) {
				// Include in total area
				area += std::abs(face.area()); 
			}
			// If face intersects (at least one vertex is inside polygon)
			else {
				// Compute edge intersections of polygon and face
				Polygon_2::Edge_const_iterator ep, ef;
				// Polygon edge iterator
				for (ep = polygon->edges_begin(); ep != polygon->edges_end(); ++ep) {
					// Face edge iterator
					for (ef = face.edges_begin(); ef != face.edges_end(); ++ef) {
						// Compute intersection
						auto intersection = CGAL::intersection(*ep, *ef);

						// Handle intersection
						if (intersection != boost::none) {
							if (const Point_2* pt = boost::get<Point_2>(&(*intersection))) {
								// Store with existent points
								points.push_back(*pt); 
							}
						}
					}
				}
				
				// Order points (either CW or CCW)
				std::vector<Point_2> ordered;
				CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(ordered));

				// Construct polygon and include in total area
				Polygon_2 pol(ordered.begin(), ordered.end());
				area += std::abs(pol.area());
			}
		}
	}

	std::cout << area / std::abs(polygon->area()) << std::endl;
	if (area / std::abs(polygon->area()) >= 0.8) { return true; }
	return false;
}


// Define simplified face
inline void define_face(const Mesh* mesh, unsigned int id, Plane_3* plane, std::vector<Polygon_2>* polygons) {
	// Project segment faces to 2D polygons
	std::vector<Polygon_2> faces = project_segment_faces(mesh, id, plane);
	//draw_mesh_segment(&faces, id);

	// Check overlapping
	std::cout << "SEGMENT " << id << std::endl;
	for (auto polygon : *polygons) {
		// If face overlaps with segment
		check_overlap(&polygon, &faces);
	}
	std::cout << std::endl;
}

