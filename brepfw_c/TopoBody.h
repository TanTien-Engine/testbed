#pragma once

#include "TopoShape.h"

#include <memory>
#include <vector>

namespace brepfw
{

class TopoShell;

class TopoBody : public TopoShape
{
public:
	TopoBody() {}

	virtual TopoShapeType GetType() const override { return TOPO_BODY; }

private:
	std::vector<std::shared_ptr<TopoShell>> m_shells;

}; // TopoBody

}