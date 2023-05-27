#pragma once

#include "Label.h"

#include <memory>

namespace brepfw
{

class Label;

class LabelTools
{
public:
	static std::shared_ptr<Label> FindOrAddChild(const std::shared_ptr<Label>& parent, uint32_t tag);

	static uint32_t GetTag(const std::shared_ptr<Label>& lbl);

private:
	template <typename T>
	static T& FindOrAddAttr(const std::shared_ptr<Label>& label)
	{
		return label->HasComponent<T>() ?
			label->GetComponent<T>() : label->AddComponent<T>();
	}

}; // LabelTools

}