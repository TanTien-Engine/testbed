#include "CompHalfEdge.h"
#include "BRepExplore.h"

#include <halfedge/Polyhedron.h>

#include <vector>
#include <iterator>

namespace brepfw
{

CompHalfEdge::CompHalfEdge(const std::shared_ptr<TopoShape>& shape)
{
	BuildTopo(shape);
}

void CompHalfEdge::BuildTopo(const std::shared_ptr<TopoShape>& shape)
{
	std::vector<sm::vec3> points;
	std::vector<std::vector<uint32_t>> in_faces;
    BRepExplore::Dump(shape, points, in_faces);

    std::vector<he::Polyhedron::in_vert> verts;
    verts.reserve(points.size());
    for (auto& point : points) {
        verts.push_back({ -1, point });
    }

    std::vector<he::Polyhedron::in_face> faces;
    faces.reserve(in_faces.size());
    std::vector<size_t> loop_n;
    for (auto& in_face : in_faces)
    {
        he::Polyhedron::in_loop border;
        std::vector<he::Polyhedron::in_loop> holes;

        std::copy(in_face.begin(), in_face.end(), std::back_inserter(border));

        faces.push_back({ -1, border, holes });
        loop_n.push_back(1);
    }

	m_topo = std::make_shared<he::Polyhedron>(verts, faces);

    assert(points.size() == m_topo->GetVerts().Size());
    auto first_vert = m_topo->GetVerts().Head();
    auto curr_vert = first_vert;
    size_t idx_vert = 0;
    do {
        assert(points[idx_vert] == curr_vert->position);

        ++idx_vert;
        curr_vert = curr_vert->linked_next;
    } while (curr_vert != first_vert);

    assert(loop_n.size() == faces.size());
    auto first_face = m_topo->GetLoops().Head();
    auto curr_face = first_face;
    size_t idx_face = 0;
    do {
        for (size_t i = 0; i < loop_n[idx_face] - 1; ++i) {
            curr_face = curr_face->linked_next;
            assert(curr_face != first_face);
        }

        ++idx_face;

        curr_face = curr_face->linked_next;
    } while (curr_face != first_face);
}

}