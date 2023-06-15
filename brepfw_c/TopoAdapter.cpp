#include "TopoAdapter.h"
#include "BRepExplore.h"
#include "BRepBuilder.h"
#include "TopoShell.h"
#include "TopoFace.h"
#include "TopoVertex.h"

#include <SM_Vector.h>
#include <halfedge/Polyhedron.h>

#include <vector>
#include <iterator>

namespace brepfw
{

std::shared_ptr<he::Polyhedron>
TopoAdapter::BRep2Topo(const std::shared_ptr<TopoShape>& brep)
{
	std::vector<std::shared_ptr<TopoVertex>> points;
	std::vector<std::shared_ptr<TopoFace>> faces;
	std::vector<std::vector<uint32_t>> faces_idx;
	BRepExplore::Dump(brep, points, faces, faces_idx);
	if (faces.empty()) {
		return nullptr;
	}

	auto poly = std::make_shared<he::Polyhedron>();

	std::vector<he::vert3*> v_array;
    v_array.reserve(points.size());
    for (auto& point : points)
    {
		auto v = poly->AddVertex(point->GetPos());

		v->id = reinterpret_cast<uint64_t>(point.get());

        v_array.push_back(v);
    }

	he::Polyhedron::LoopBuilder builder;
	assert(faces.size() == faces_idx.size());
	for (size_t i = 0, n = faces.size(); i < n; ++i)
	{
		std::vector<size_t> loop_indices;
		std::copy(faces_idx[i].begin(), faces_idx[i].end(), std::back_inserter(loop_indices));
		auto loop = poly->AddFace(loop_indices, v_array, builder);

		loop->id = reinterpret_cast<uint64_t>(faces[i].get());
	}

	builder.Build();

	return poly;
}

std::shared_ptr<TopoShape>
TopoAdapter::Topo2BRep(const std::shared_ptr<he::Polyhedron>& topo)
{
	std::vector<sm::vec3> points;
	std::vector<std::vector<uint32_t>> faces;

	std::map<he::vert3*, size_t> vert2idx;

	auto& vertices = topo->GetVerts();
	if (vertices.Size() == 0) {
		return nullptr;
	}
	points.reserve(vertices.Size());

	auto curr_vert = vertices.Head();
	auto first_vert = curr_vert;
	do {
		vert2idx.insert({ curr_vert, points.size() });
		points.push_back(curr_vert->position);
		curr_vert = curr_vert->linked_next;
	} while (curr_vert != first_vert);

	auto& src_faces = topo->GetFaces();
	faces.reserve(src_faces.size());
	for (auto& s_f : src_faces)
	{
		std::vector<uint32_t> face;

		auto first_e = s_f.border->edge;
		auto curr_e = first_e;
		do {
			auto itr = vert2idx.find(curr_e->vert);
			assert(itr != vert2idx.end());
			face.push_back(static_cast<uint32_t>(itr->second));

			curr_e = curr_e->next;
		} while (curr_e != first_e);

		faces.push_back(face);
	}

	return BRepBuilder::BuildShell(points, faces);
}

void TopoAdapter::Topo2BRep(const std::shared_ptr<TopoShape>& brep, 
	                        const std::shared_ptr<he::Polyhedron>& topo)
{
	if (brep->GetType() != TOPO_SHELL) {
		return;
	}

	//auto& src_verts = topo->GetVerts();


	auto& src_faces = topo->GetFaces();

	for (auto& s_f : src_faces)
	{
		auto topo_face = reinterpret_cast<TopoVertex*>(s_f.border->id);
		if (s_f.border->type != he::EditType::Unmod)
		{
			int zz = 0;
		}

		std::vector<uint32_t> face;

		auto first_e = s_f.border->edge;
		auto curr_e = first_e;
		do {
			//auto itr = vert2idx.find(curr_e->point);
			//assert(itr != vert2idx.end());
			//face.push_back(itr->second);

			auto topo_vert = reinterpret_cast<TopoVertex*>(curr_e->vert->id);
			if (curr_e->vert->type != he::EditType::Unmod)
			{
				int zz = 0;
			}

			curr_e = curr_e->next;
		} while (curr_e != first_e);

		//faces.push_back(face);
	}

	auto shell = std::static_pointer_cast<TopoShell>(brep);
	auto& dst_faces = shell->GetFaces();

	int zz = 0;
}

}