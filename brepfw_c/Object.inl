#include "Component.h"

#include <guard/check.h>

namespace brepfw
{

template <typename T>
bool Object::HasComponent() const
{
	static_assert(std::is_base_of<Component, T>::value,
		"T must inherit from Component");

	return (m.comp_bitset & (1 << GetCompTypeID<T>())) != 0;
}


template <typename T, typename... TArgs>
T& Object::AddComponent(TArgs&&... args)
{
	static_assert(std::is_base_of<Component, T>::value,
		"T must inherit from Component");

	GD_ASSERT(!HasComponent<T>(), "already has the component");

	auto comp_ptr = new T(std::forward<TArgs>(args)...);
	auto& comp = *comp_ptr;

	CompID id = GetCompTypeID<T>();
	GD_ASSERT(id < MAX_COMPONENTS, "too many components");
	m.comp_bitset |= (1 << id);

	auto new_comp_sz = m.comp_sz + 1;
	auto new_comp = new Component*[new_comp_sz];
	bool added = false;
	int ptr_new = 0, ptr_old = 0;
	for (size_t i = 0; i < m.comp_sz; ++i)
	{
		GD_ASSERT(m_comps[i]->TypeID() != id, "exists");
		if (m_comps[i]->TypeID() < id) {
			new_comp[ptr_new++] = std::move(m_comps[ptr_old++]);
		} else {
			added = true;
			new_comp[ptr_new++] = std::move(comp_ptr);
			while (i++ < m.comp_sz) {
				new_comp[ptr_new++] = std::move(m_comps[ptr_old++]);
			}
			break;
		}
	}
	if (!added) {
		new_comp[ptr_new++] = std::move(comp_ptr);
	}
	m.comp_sz = new_comp_sz;
	delete[] m_comps;
	m_comps = new_comp;

//	comp.Init();
	return comp;
}

template <typename T>
bool Object::RemoveComponent()
{
	if (!HasComponent<T>()) {
		return false;
	}

	CompID id = GetCompTypeID<T>();
	GD_ASSERT(id < MAX_COMPONENTS, "too many components");
	m.comp_bitset &= ~(1 << id);

	auto new_comp_sz = m.comp_sz - 1;
	auto new_comp = new Component*[new_comp_sz];
	int ptr_new = 0, ptr_old = 0;
	for (size_t i = 0; i < m.comp_sz; ++i) {
		if (m_comps[i]->TypeID() != id) {
			new_comp[ptr_new++] = std::move(m_comps[ptr_old++]);
		}
	}

	m.comp_sz = new_comp_sz;
	delete[] m_comps;
	m_comps = new_comp;

	return true;
}

template <typename T>
T& Object::GetComponent() const
{
	static_assert(std::is_base_of<Component, T>::value,
		"T must inherit from Component");

	GD_ASSERT(HasComponent<T>(), "no component");
	auto id = GetCompTypeID<T>();

	int idx = QueryIndexByID<Component*>(m_comps, m.comp_sz, id);
	return *reinterpret_cast<T*>(m_comps[idx]);
}

template <typename T>
int Object::QueryIndexByID(const T* array, size_t array_sz, size_t id)
{
	int idx = -1;
	int start = 0;
	int end = static_cast<int>(array_sz) - 1;
	while (start <= end)
	{
		int mid = (start + end) / 2;
		auto& comp = array[mid];
		if (id == comp->TypeID()) {
			idx = mid;
			break;
		} else if (id < comp->TypeID()) {
			end = mid - 1;
		} else {
			start = mid + 1;
		}
	}
	GD_ASSERT(idx != -1, "err idx");
	return idx;
}

}