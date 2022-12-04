#pragma once

#include <vector>
#include <iterator>

namespace brepvmgraph
{

class BRepCode
{
public:
	void Write(const char* data, size_t size) { 
		std::copy(data, data + size, std::back_inserter(m_code));
	}

	auto& GetCode() const { return m_code; }

private:
	std::vector<uint8_t> m_code;

}; // BRepCode

}