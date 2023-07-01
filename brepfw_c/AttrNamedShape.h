#pragma once

#include "Attribute.h"

namespace brepfw
{

class TopoShape;

class AttrNamedShape : public Attribute
{
public:
	AttrNamedShape(const std::shared_ptr<TopoShape>& src,
		const std::shared_ptr<TopoShape>& dst);

	virtual const char* Type() const override { return "named_shape"; }
	virtual CompID TypeID() const override { return GetCompTypeID<AttrNamedShape>(); }
	virtual Attribute* Clone() const override { return nullptr; }

	auto GetSrc() const { return m_src; }
	auto GetDst() const { return m_dst; }

	void SetShape(const std::shared_ptr<TopoShape>& src, const std::shared_ptr<TopoShape>& dst) {
		m_src = src; m_dst = dst;
	}

private:
	std::shared_ptr<TopoShape> m_src = nullptr, m_dst = nullptr;

}; // AttrNamedShape

}