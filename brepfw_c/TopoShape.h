#pragma once

#include "Object.h"
#include "TopoShapeType.h"

namespace brepfw
{

class TopoShape : public Object
{
public:
	virtual ~TopoShape() {}

	virtual TopoShapeType GetType() const = 0;

}; // TopoShape

}