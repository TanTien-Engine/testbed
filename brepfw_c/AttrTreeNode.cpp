#include "AttrTreeNode.h"
#include "AttrNaming.h"
#include "Label.h"
#include "LabelTools.h"

#include <algorithm>

namespace brepfw
{

void AttrTreeNode::AddChild(const std::shared_ptr<Label>& child)
{
	//m_children.push_back(child);

	auto itr = std::upper_bound(m_children.begin(), m_children.end(), child, 
		[](const std::shared_ptr<Label>& lbl_1, const std::shared_ptr<Label>& lbl_2)
	{
		return LabelTools::GetTag(lbl_1) < LabelTools::GetTag(lbl_2);
	});
	m_children.insert(itr, child);
}

bool AttrTreeNode::RemoveChild(const std::shared_ptr<Label>& child)
{
	for (auto itr = m_children.begin(); itr != m_children.end(); ++itr)
	{
		if (*itr == child)
		{
			m_children.erase(itr);
			return true;
		}
	}
	return false;
}

void AttrTreeNode::RemoveAllChildren()
{
	m_children.clear();
}

void AttrTreeNode::SetChildren(const std::vector<std::shared_ptr<Label>>& children)
{
	m_children = children;
}

}