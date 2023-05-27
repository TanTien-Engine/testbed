#pragma once

#include "Attribute.h"

#include <vector>
#include <memory>

namespace brepfw
{

class Label;

class AttrTreeNode : public Attribute
{
public:

	virtual const char* Type() const override { return "tree_node"; }
	virtual CompID TypeID() const override { return GetCompTypeID<AttrTreeNode>(); }
	virtual Attribute* Clone() const override { return nullptr; }

	void AddChild(const std::shared_ptr<Label>& child);
	bool RemoveChild(const std::shared_ptr<Label>& child);
	void RemoveAllChildren();

	const std::vector<std::shared_ptr<Label>>& GetAllChildren() const {
		return m_children;
	}
	void SetChildren(const std::vector<std::shared_ptr<Label>>& children);

private:
	std::shared_ptr<Label> m_parent;
	std::vector<std::shared_ptr<Label>> m_children;

}; // AttrTreeNode

}