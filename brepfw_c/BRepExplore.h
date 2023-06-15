#pragma once

#include <SM_Vector.h>

#include <memory>
#include <vector>

namespace brepfw
{

class TopoShape;
class TopoVertex;
class TopoFace;

class BRepExplore
{
public:
	static void Dump(const std::shared_ptr<TopoShape>& shape,
		std::vector<sm::vec3>& points, std::vector<std::vector<uint32_t>>& faces);

	static void Dump(const std::shared_ptr<TopoShape>& shape, std::vector<std::shared_ptr<TopoVertex>>& points, 
		std::vector<std::shared_ptr<TopoFace>>& faces, std::vector<std::vector<uint32_t>>& faces_idx);

}; // BRepExplore

}