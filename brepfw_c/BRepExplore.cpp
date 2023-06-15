#include "BRepExplore.h"
#include "TopoVertex.h"
#include "TopoEdge.h"
#include "TopoLoop.h"
#include "TopoFace.h"
#include "TopoShell.h"

#include <map>

namespace brepfw
{

void BRepExplore::Dump(const std::shared_ptr<TopoShape>& shape, 
	                   std::vector<sm::vec3>& points, 
	                   std::vector<std::vector<uint32_t>>& faces)
{
	if (shape->GetType() != TOPO_SHELL) {
		return;
	}

	std::map<std::shared_ptr<TopoVertex>, uint32_t> vert2idx;

	auto shell = std::static_pointer_cast<TopoShell>(shape);
	for (auto& src_f : shell->GetFaces())
	{
		auto& loops = src_f->GetLoops();
		if (loops.empty()) {
			continue;
		}

		std::vector<uint32_t> face;
		for (auto& edge : loops[0]->GetEdges())
		{
			auto p = edge->GetPos1();
			auto itr = vert2idx.find(p);
			if (itr != vert2idx.end())
			{
				face.push_back(itr->second);
			}
			else
			{
				uint32_t idx = static_cast<uint32_t>(points.size());
				face.push_back(idx);

				points.push_back(p->GetPos());
				vert2idx.insert({ p, idx });
			}
		}
		faces.push_back(face);
	}
}

void BRepExplore::Dump(const std::shared_ptr<TopoShape>& shape, std::vector<std::shared_ptr<TopoVertex>>& points,
	                   std::vector<std::shared_ptr<TopoFace>>& faces, std::vector<std::vector<uint32_t>>& faces_idx)
{
	if (shape->GetType() != TOPO_SHELL) {
		return;
	}

	std::map<std::shared_ptr<TopoVertex>, uint32_t> vert2idx;

	auto shell = std::static_pointer_cast<TopoShell>(shape);
	for (auto& src_f : shell->GetFaces())
	{
		auto& loops = src_f->GetLoops();
		if (loops.empty()) {
			continue;
		}

		std::vector<uint32_t> face;
		for (auto& edge : loops[0]->GetEdges())
		{
			auto p = edge->GetPos1();
			auto itr = vert2idx.find(p);
			if (itr != vert2idx.end())
			{
				face.push_back(itr->second);
			}
			else
			{
				uint32_t idx = static_cast<uint32_t>(points.size());
				face.push_back(idx);

				points.push_back(p);
				vert2idx.insert({ p, idx });
			}
		}
		faces.push_back(src_f);
		faces_idx.push_back(face);
	}
}

}