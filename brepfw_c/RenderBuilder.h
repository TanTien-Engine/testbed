#pragma once

#include <SM_Vector.h>

#include <memory>
#include <vector>

namespace ur { 
	class VertexArray; 
	class Device;
}

namespace brepfw
{

class TopoShape;

class RenderBuilder
{
public:
	static std::shared_ptr<ur::VertexArray> BuildVAO(const ur::Device& dev, const std::shared_ptr<TopoShape>& shape, 
		const sm::vec3& color);

private:
	static std::vector<uint32_t> Triangulation(const std::vector<sm::vec3>& verts,
		const std::vector<uint32_t>& border, const std::vector<std::vector<uint32_t>>& holes);

}; // RenderBuilder

}