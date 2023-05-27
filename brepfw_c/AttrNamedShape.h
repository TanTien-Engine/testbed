#pragma once

#include "Attribute.h"

namespace brepfw
{

class TopoShape;

class AttrNamedShape : public Attribute
{
public:

	virtual const char* Type() const override { return "named_shape"; }
	virtual CompID TypeID() const override { return GetCompTypeID<AttrNamedShape>(); }
	virtual Attribute* Clone() const override { return nullptr; }

private:
	std::shared_ptr<TopoShape> m_src = nullptr, m_dst = nullptr;

}; // AttrNamedShape

}