#pragma once

#include <stdint.h>

#include <string>

namespace brepfw
{

class Component;

class Object
{
public:
	~Object();

	template <typename T>
	bool HasComponent() const;

	template <typename T, typename... TArgs>
	T& AddComponent(TArgs&&... args);

	template <typename T>
	bool RemoveComponent();

	template <typename T>
	T& GetComponent() const;

	//void TraverseComponent(std::function<bool(const std::unique_ptr<NodeComp>&)> func);

	size_t GetComponentCount() const { return m.comp_sz; }

private:
	template <typename T>
	static int QueryIndexByID(const T* array, size_t array_sz, size_t id);

	void Clear();

private:
	static const size_t MAX_COMPONENTS = 27;

	struct M
	{
		M() { Clear(); }

		void Clear() {
			memset(this, 0, sizeof(M));
		}

		uint32_t comp_bitset : MAX_COMPONENTS;
		uint32_t comp_sz : 5;
	};

	Component** m_comps = nullptr;
	M m;

}; // Object

}

#include "Object.inl"