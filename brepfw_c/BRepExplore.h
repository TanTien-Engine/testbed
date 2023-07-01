#pragma once

#include <SM_Vector.h>

#include <memory>
#include <vector>
#include <map>

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

private:
	static void DumpFace(const std::shared_ptr<TopoFace>& face,
		std::vector<sm::vec3>& points, std::vector<uint32_t>& face_indices,
		std::map<std::shared_ptr<TopoVertex>, uint32_t>& vert2idx);

}; // BRepExplore

}