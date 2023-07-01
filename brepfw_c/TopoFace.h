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

	void SetLoops(const std::vector<std::shared_ptr<TopoLoop>>& loops) { m_loops = loops; }
	auto& GetLoops() const { return m_loops; }

	void SetID(int id) { m_id = id; }
	int GetID() const { return m_id; }

private:
	std::vector<std::shared_ptr<TopoLoop>> m_loops;

	int m_id = -1;

}; // TopoFace

}