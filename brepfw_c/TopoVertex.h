#pragma once

#include "TopoShape.h"

#include <SM_Vector.h>

#include <memory>
#include <vector>

namespace brepfw
{

//class TopoEdge;

class TopoVertex : public TopoShape
{
public:
	TopoVertex() {}
	TopoVertex(const sm::vec3& pos) : m_pos(pos) {}

	virtual TopoShapeType GetType() const override { return TOPO_VERTEX; }

	auto& GetPos() const { return m_pos; }

private:
	sm::vec3 m_pos;
	
	//std::vector<std::weak_ptr<TopoEdge>> m_edges;

}; // TopoVertex

}