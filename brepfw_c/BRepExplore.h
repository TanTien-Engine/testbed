#pragma once

#include <SM_Vector.h>

#include <memory>
#include <vector>

namespace brepfw
{

class TopoShape;

class BRepExplore
{
public:
	static void Dump(const std::shared_ptr<TopoShape>& shape,
		std::vector<sm::vec3>& points, std::vector<std::vector<uint32_t>>& faces);

}; // BRepExplore

}