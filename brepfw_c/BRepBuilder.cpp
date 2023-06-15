#include "BRepBuilder.h"
#include "TopoVertex.h"
#include "TopoEdge.h"
#include "TopoLoop.h"
#include "TopoFace.h"
#include "TopoShell.h"

#include <polymesh3/Polytope.h>

namespace brepfw
{

std::shared_ptr<TopoShell> BRepBuilder::BuildShell(std::vector<sm::vec3>& points, std::vector<std::vector<uint32_t>>& faces)
{
	std::vector<std::shared_ptr<TopoVertex>> dst_points;
	dst_points.reserve(points.size());
	for (auto& p : points) {
		dst_points.push_back(std::make_shared<TopoVertex>(p));
	}

	std::vector<std::shared_ptr<TopoFace>> dst_faces;
	for (auto& f : faces)
	{
		if (f.size() < 3) {
			continue;
		}

		std::vector<std::shared_ptr<TopoEdge>> edges;
		for (size_t i = 0, n = f.size(); i < n; ++i)
		{
			auto p1 = dst_points[f[i]];
			auto p2 = dst_points[f[(i + 1) % n]];
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