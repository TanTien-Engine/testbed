#pragma once

#include <memory>

namespace brepfw
{

class Label;
class TopoShape;

class LabelBuilder
{
public:
	static void BuildFromShape(const std::shared_ptr<Label>& label,
		const std::shared_ptr<TopoShape>& shape);

}; // LabelBuilder

}