#pragma once

#include "TopoShape.h"

#include <memory>
#include <vector>

namespace brepfw
{

class TopoEdge;

class TopoLoop : public TopoShape
{
public:
	TopoLoop() {}
	TopoLoop(const std::vector<std::shared_ptr<TopoEdge>>& edges) 
		: m_edges(edges) {}
	
	virtual TopoShapeType GetType() const override { return TOPO_LOOP; }

	auto& GetEdges() const { return m_edges; }

private:
	std::vector<std::shared_ptr<TopoEdge>> m_edges;

}; // TopoLoop

}