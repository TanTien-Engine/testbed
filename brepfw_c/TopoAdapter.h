#pragma once

#include <memory>

namespace he { class Polyhedron; }

namespace brepfw
{

class TopoShape;

class TopoAdapter
{
public:
	static std::shared_ptr<he::Polyhedron>
		BRep2Topo(const std::shared_ptr<TopoShape>& brep);
	static std::shared_ptr<TopoShape> 
		Topo2BRep(const std::shared_ptr<he::Polyhedron>& topo);

	static void Topo2BRep(const std::shared_ptr<TopoShape>& brep, 
		const std::shared_ptr<he::Polyhedron>& topo);

}; // TopoAdapter

}