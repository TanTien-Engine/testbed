#pragma once

#include <SM_Vector.h>

#include <memory>
#include <vector>

namespace brepfw
{

class TopoShell;

class BRepBuilder
{
public:
	static std::shared_ptr<TopoShell> BuildShell(
		std::vector<sm::vec3>& points, std::vector<std::vector<uint32_t>>& faces
	);

}; // BRepBuilder

}