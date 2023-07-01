#pragma once

#include "Attribute.h"

namespace ur { class VertexArray; }

namespace brepfw
{

class AttrRenderObj : public Attribute
{
public:
	AttrRenderObj(const std::shared_ptr<ur::VertexArray>& va);

	virtual const char* Type() const override { return "render_obj"; }
	virtual CompID TypeID() const override { return GetCompTypeID<AttrRenderObj>(); }
	virtual Attribute* Clone() const override { return nullptr; }

	void SetVA(const std::shared_ptr<ur::VertexArray>& va) { m_va = va; }
	auto GetVA() const { return m_va; }

private:
	std::shared_ptr<ur::VertexArray> m_va = nullptr;

}; // AttrRenderObj

}