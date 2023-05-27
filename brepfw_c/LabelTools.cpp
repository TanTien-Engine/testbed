#include "LabelTools.h"
#include "Label.h"
#include "AttrNaming.h"
#include "AttrTreeNode.h"

namespace brepfw
{

std::shared_ptr<Label> LabelTools::FindOrAddChild(const std::shared_ptr<Label>& parent, uint32_t tag)
{
	auto& tree_node = FindOrAddAttr<AttrTreeNode>(parent);

	auto& children = tree_node.GetAllChildren();
	for (auto c : children) {
		if (c->HasComponent<AttrNaming>() && c->GetComponent<AttrNaming>().GetTag() == tag) {
			return c;
		}
	}

	std::shared_ptr<Label> child = std::make_shared<Label>();
	child->AddComponent<AttrNaming>(tag);
	tree_node.AddChild(child);

	return child;
}

uint32_t LabelTools::GetTag(const std::shared_ptr<Label>& lbl)
{
	return lbl->HasComponent<AttrNaming>() ? lbl->GetComponent<AttrNaming>().GetTag() : std::numeric_limits<uint32_t>::max();
}

}