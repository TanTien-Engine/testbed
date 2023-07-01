#include "TopoAdapter.h"
#include "BRepExplore.h"
#include "BRepBuilder.h"
#include "TopoShell.h"
#include "TopoFace.h"
#include "TopoVertex.h"
#include "TopoEdge.h"
#include "TopoLoop.h"

#include <SM_Vector.h>

#include <set>
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

void TopoAdapter::Topo2BRep(const std::shared_ptr<he::Polyhedron>& topo,
	                        std::shared_ptr<TopoShape>& brep)
{
	if (brep->GetType() != TOPO_SHELL) {
		return;
	}

	// old brep
	std::vector<std::shared_ptr<TopoVertex>> points;
	std::vector<std::shared_ptr<TopoFace>> faces;
	std::vector<std::vector<uint32_t>> faces_idx;
	BRepExplore::Dump(brep, points, faces, faces_idx);
	if (faces.empty()) {
		return;
	}

	// point to old vert idx
	for (int i = 0, n = static_cast<int>(points.size()); i < n; ++i) {
		points[i]->SetID(i);
	}

	std::vector<std::shared_ptr<TopoVertex>> dst_verts;

	// convert verts topo to brep
    auto& src_pts = topo->GetVerts();
	dst_verts.reserve(src_pts.Size());
    auto first_p = src_pts.Head();
    auto curr_p = first_p;
    do {
		switch (curr_p->type)
		{
		case he::EditType::Unmod:
		{
			int idx = reinterpret_cast<TopoVertex*>(curr_p->id)->GetID();
			dst_verts.push_back(points[idx]);
		}
			break;
		case he::EditType::Add:
		{
			auto v = std::make_shared<TopoVertex>(curr_p->position);
			curr_p->id = reinterpret_cast<uint64_t>(v.get());
			dst_verts.push_back(v);
		}
			break;
		case he::EditType::Mod:
		{
			int idx = reinterpret_cast<TopoVertex*>(curr_p->id)->GetID();
			points[idx]->SetPos(curr_p->position);
			dst_verts.push_back(points[idx]);
		}
			break;
		}

        curr_p = curr_p->linked_next;
    } while (curr_p != first_p);

	// point to new vert idx
	for (int i = 0, n = static_cast<int>(dst_verts.size()); i < n; ++i) {
		dst_verts[i]->SetID(i);
	}

	// point to old face idx
	for (int i = 0, n = static_cast<int>(faces.size()); i < n; ++i) {
		faces[i]->SetID(i);
	}

	std::vector<std::shared_ptr<TopoFace>> dst_faces;
	dst_faces.reserve(faces.size());

	auto& src_faces = topo->GetFaces();
	for (auto& s_f : src_faces)
	{
		switch (s_f.border->type)
		{
		case he::EditType::Unmod:
		{
			int idx = reinterpret_cast<TopoFace*>(s_f.border->id)->GetID();
			dst_faces.push_back(faces[idx]);
		}
			break;
		case he::EditType::Add:
		{
			auto edges = Topo2BRep(s_f, dst_verts);
			auto loop = std::make_shared<TopoLoop>(edges);
			dst_faces.push_back(std::make_shared<TopoFace>(loop));
		}
			break;
		case he::EditType::Mod:
		{
			auto edges = Topo2BRep(s_f, dst_verts);
			auto loop = std::make_shared<TopoLoop>(edges);

			int idx = reinterpret_cast<TopoFace*>(s_f.border->id)->GetID();
			faces[idx]->SetLoops({ loop });

			dst_faces.push_back(faces[idx]);
		}
			break;
		}
	}

	auto shell = std::static_pointer_cast<TopoShell>(brep);
	shell->SetFaces(dst_faces);
}

std::vector<std::shared_ptr<TopoEdge>> 
TopoAdapter::Topo2BRep(const he::Polyhedron::Face& face,
	                   const std::vector<std::shared_ptr<TopoVertex>>& verts)
{
	std::vector<uint32_t> loop_indices;

	auto first_e = face.border->edge;
	auto curr_e = first_e;
	do {
		int idx = reinterpret_cast<TopoVertex*>(curr_e->vert->id)->GetID();
		loop_indices.push_back(idx);

		curr_e = curr_e->next;
	} while (curr_e != first_e);

	std::vector<std::shared_ptr<TopoEdge>> edges;
	for (size_t i = 0, n = loop_indices.size(); i < n; ++i)
	{
		auto p1 = verts[loop_indices[i]];
		auto p2 = verts[loop_indices[(i + 1) % n]];
		auto edge = std::make_shared<TopoEdge>(p1, p2);
		edges.push_back(edge);
	}

	return edges;
}

}