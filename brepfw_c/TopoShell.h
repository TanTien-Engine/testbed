#pragma once

#include "TopoShape.h"

#include <memory>
#include <vector>

namespace brepfw
{

class TopoFace;

class TopoShell : public TopoShape
{
public:
	TopoShell() {}
	TopoShell(const std::vector<std::shared_ptr<TopoFace>>& faces)
		: m_faces(faces) {}

	virtual TopoShapeType GetType() const override { return TOPO_SHELL; }

	auto& GetFaces() const { return m_faces; }
	void SetFaces(const std::vector<std::shared_ptr<TopoFace>>& faces) {
		m_faces = faces;
	}

private:
	std::vector<std::shared_ptr<TopoFace>> m_faces;

}; // TopoShell

}