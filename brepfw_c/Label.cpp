#include "Label.h"
#include "LabelTools.h"

#include <algorithm>

namespace brepfw
{

Label::~Label()
{
	for (auto& child : m_children) {
		child->m_parent = nullptr;
	}
}

void Label::AddChild(const std::shared_ptr<Label>& child)
{
	//m_children.push_back(child);

	if (m_children.size() > 10)
	{
		int zz = 0;
	}

	child->m_parent = this;

	auto itr = std::upper_bound(m_children.begin(), m_children.end(), child, 
		[](const std::shared_ptr<Label>& lbl_1, const std::shared_ptr<Label>& lbl_2)
	{
		return LabelTools::GetTag(lbl_1) < LabelTools::GetTag(lbl_2);
	});
	m_children.insert(itr, child);
}

bool Label::RemoveChild(const std::shared_ptr<Label>& child)
{
	for (auto itr = m_children.begin(); itr != m_children.end(); ++itr)
	{
		if (*itr == child)
		{
			(*itr)->m_parent = nullptr;
			m_children.erase(itr);
			return true;
		}
	}
	return false;
}

void Label::RemoveAllChildren()
{
	for (auto& child : m_children) {
		child->m_parent = nullptr;
	}
	m_children.clear();
}

void Label::SetChildren(const std::vector<std::shared_ptr<Label>>& children)
{
	m_children = children;

	for (auto& child : m_children) {
		child->m_parent = this;
	}
}

}