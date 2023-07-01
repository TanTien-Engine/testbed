#include "LabelBuilder.h"
#include "Label.h"
#include "AttrNamedShape.h"
#include "TopoShape.h"
#include "TopoShell.h"
#include "AttrTreeNode.h"
#include "TopoFace.h"

namespace brepfw
{

void LabelBuilder::BuildFromShape(const std::shared_ptr<Label>& label,
	                              const std::shared_ptr<TopoShape>& shape)
{
	label->AddComponent<AttrNamedShape>(nullptr, shape);

	if (shape->GetType() != TOPO_SHELL) {
		return;
	}

	auto shell = std::static_pointer_cast<TopoShell>(shape);
	auto& faces = shell->GetFaces();
	if (faces.empty()) {
		return;
	}

	auto& node_comp = label->AddComponent<AttrTreeNode>();
	for (auto& face : faces)
	{
		auto child = std::make_shared<Label>();
		child->AddComponent<AttrNamedShape>(nullptr, face);
		node_comp.AddChild(child);
	}
}

}