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

	void SetPos(const sm::vec3& pos) { m_pos = pos; }
	auto& GetPos() const { return m_pos; }

	void SetID(int id) { m_id = id; }
	int GetID() const { return m_id; }

private:
	sm::vec3 m_pos;
	
	//std::vector<std::weak_ptr<TopoEdge>> m_edges;

	int m_id = -1;

}; // TopoVertex

}