#pragma once

#include "Attribute.h"

#include <cstdint>

namespace brepfw
{

class AttrNaming : public Attribute
{
public:
	AttrNaming() {}
	AttrNaming(uint32_t tag) : m_tag(tag) {}

	virtual const char* Type() const override { return "naming"; }
	virtual CompID TypeID() const override { return GetCompTypeID<AttrNaming>(); }
	virtual Attribute* Clone() const override { return nullptr; }

	//void SetTag(uint32_t tag) { m_tag = tag; }
	uint32_t GetTag() const { return m_tag; }

private:
	uint32_t m_tag;

}; // AttrNaming

}