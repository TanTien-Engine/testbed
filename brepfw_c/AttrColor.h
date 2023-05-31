#pragma once

#include "Attribute.h"

#include <SM_Vector.h>

namespace brepfw
{

class TopoShape;

class AttrColor : public Attribute
{
public:
	AttrColor(const sm::vec3& color);

	virtual const char* Type() const override { return "color"; }
	virtual CompID TypeID() const override { return GetCompTypeID<AttrColor>(); }
	virtual Attribute* Clone() const override { return nullptr; }

	auto& GetColor() const { return m_color; }
	void SetColor(const sm::vec3& color) { m_color = color; }

private:
	sm::vec3 m_color;

}; // AttrColor

}