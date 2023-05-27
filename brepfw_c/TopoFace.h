#pragma once

#include "TopoShape.h"

#include <memory>
#include <vector>

namespace brepfw
{

class TopoLoop;

class TopoFace : public TopoShape
{
public:
	TopoFace() {}
	TopoFace(const std::shared_ptr<TopoLoop>& loop) {
		m_loops.push_back(loop);
	}

	virtual TopoShapeType GetType() const override { return TOPO_FACE; }

	auto& GetLoops() const { return m_loops; }

private:
	std::vector<std::shared_ptr<TopoLoop>> m_loops;

}; // TopoFace

}