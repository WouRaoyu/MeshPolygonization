#pragma once

#include "Utils.h"
#include "Draw.h"


// Convert 3D segments to 2D segments
inline std::vector<Segment_2> project_segments(Plane_3* plane, std::vector<Segment_3>* segments) {
	std::vector<Segment_2> segments_2d;

	// Project segment to 2D
	Point_2 source, target;
	std::vector<Point_2> existing_points;
	for (auto segment : *segments) {
		// Project source-target on plane
		source = plane->to_2d(segment.source());
		target = plane->to_2d(segment.target());

		// Check for overlaps
		int exists = 0;
		for (auto point : existing_points) {
			if (CGAL::squared_distance(source, point) < 0.01) { exists++; }
			if (CGAL::squared_distance(target, point) < 0.01) { exists++; }
		}

		// Add 2D segment
		if (exists != 2) {
			segments_2d.push_back(Segment_2(source, target));
			existing_points.push_back(source);
			existing_points.push_back(target);
		}
	}

	return segments_2d;
}


// Divide segments to subsegments
inline std::vector<Segment_2> split_segments(std::vector<Segment_2>* segments) {
	std::vector<Segment_2> sub_segments;

	// Compute segment intersections
	std::list<Point_2> segment_points;
	Point_2 source, target;
	for (auto segment_1 : *segments) {
		for (auto segment_2 : *segments) {
			// Compute intersection
			auto intersection = CGAL::intersection(segment_1, segment_2);

			// Handle intersection
			if (intersection != boost::none) {
				if (const Point_2* point = boost::get<Point_2>(&(*intersection))) {
						segment_points.push_back(*point);
				}
			}
		}

		// Sort segment points by distance to source
		source = segment_1.source();
		target = segment_1.target();
		segment_points.sort([&](const Point_2 &pt1, const Point_2 &pt2) {
			return CGAL::squared_distance(source, pt1) < CGAL::squared_distance(source, pt2);
		});

		// Construct subsegments
		for (auto point : segment_points) {
			sub_segments.push_back(Segment_2(source, point));
			source = point;
		}
		sub_segments.push_back(Segment_2(source, target));

		// Clear
		segment_points.clear();
	}

	return sub_segments;
}


inline void segments_to_polygons(Plane_3* plane, std::vector<Segment_3>* segments, unsigned int id) {
	// Project segments on plane
	std::vector<Segment_2> segments_2d = project_segments(plane, segments);

	std::vector<Segment_2> sub_segments = split_segments(&segments_2d);
	draw_segments(&sub_segments, id);
}