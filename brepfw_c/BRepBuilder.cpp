#include "BRepBuilder.h"
#include "TopoVertex.h"
#include "TopoEdge.h"
#include "TopoLoop.h"
#include "TopoFace.h"
#include "TopoShell.h"

#include <polymesh3/Polytope.h>

namespace brepfw
{

std::shared_ptr<TopoShell> BRepBuilder::BuildShell(const pm3::Polytope& poly)
{
	auto& src_points = poly.Points();
	std::vector<std::shared_ptr<TopoVertex>> dst_points;
	dst_points.reserve(src_points.size());
	for (auto& p : src_points) {
		dst_points.push_back(std::make_shared<TopoVertex>(p->pos));
	}

	auto& src_faces = poly.Faces();
	std::vector<std::shared_ptr<TopoFace>> dst_faces;
	for (auto& f : src_faces)
	{
		if (f->border.size() < 3) {
			continue;
		}

		std::vector<std::shared_ptr<TopoEdge>> edges;
		for (size_t i = 0, n = f->border.size(); i < n; ++i)
		{
			auto p1 = dst_points[f->border[i]];
			auto p2 = dst_points[f->border[(i + 1) % n]];
			auto edge = std::make_shared<TopoEdge>(p1, p2);
			edges.push_back(edge);
		}

		auto loop = std::make_shared<TopoLoop>(edges);
		auto face = std::make_shared<TopoFace>(loop);
		dst_faces.push_back(face);
	}

	return std::make_shared<TopoShell>(dst_faces);
}

}