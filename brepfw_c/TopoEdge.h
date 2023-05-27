#pragma once

#include "TopoShape.h"

#include <memory>

namespace brepfw
{

class TopoVertex;

class TopoEdge : public TopoShape
{
public:
	TopoEdge() {}
	TopoEdge(const std::shared_ptr<TopoVertex>& p1, 
		const std::shared_ptr<TopoVertex>& p2) 
		: m_p1(p1), m_p2(p2) {}

	virtual TopoShapeType GetType() const override { return TOPO_EDGE; }

	auto GetPos1() const { return m_p1; }
	auto GetPos2() const { return m_p2; }

private:
	std::shared_ptr<TopoVertex> m_p1, m_p2;

}; // TopoEdge

}