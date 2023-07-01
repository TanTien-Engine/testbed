#pragma once

#include <halfedge/Polyhedron.h>

#include <memory>

namespace he { class Polyhedron; }

namespace brepfw
{

class TopoShape;
class TopoFace;
class TopoVertex;
class TopoEdge;

class TopoAdapter
{
public:
	static std::shared_ptr<he::Polyhedron>
		BRep2Topo(const std::shared_ptr<TopoShape>& brep);
	static std::shared_ptr<TopoShape> 
		Topo2BRep(const std::shared_ptr<he::Polyhedron>& topo);

	static void Topo2BRep(const std::shared_ptr<he::Polyhedron>& topo,
		std::shared_ptr<TopoShape>& brep);

private:
	static std::vector<std::shared_ptr<TopoEdge>> Topo2BRep(const he::Polyhedron::Face& face,
		const std::vector<std::shared_ptr<TopoVertex>>& verts);

}; // TopoAdapter

}