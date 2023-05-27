#pragma once

#include <boost/noncopyable.hpp>

#include <memory>

namespace brepfw
{

using CompID = size_t;

namespace Internal
{
inline size_t GetUniqueCompID() noexcept
{
    static CompID id{ 0u };
    return id++;
}
}

class Component;

template <typename T>
inline CompID GetCompTypeID() noexcept
{
    static_assert(std::is_base_of<Component, T>::value,
        "T must inherit from Component");

    static CompID type_id{Internal::GetUniqueCompID()};
    return type_id;
}

class Component : boost::noncopyable
{
public:
	virtual ~Component() {}

	virtual const char* Type() const = 0;
	virtual CompID TypeID() const = 0;
	//virtual std::unique_ptr<Component> Clone() const = 0;
    virtual Component* Clone() const = 0;

}; // Component

}