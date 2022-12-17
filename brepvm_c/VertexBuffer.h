#pragma once

#include <vector>

namespace brepvmgraph
{

struct Body;

class VertexBuffer
{
public:
	VertexBuffer(const Body& body);

	auto& GetVBuf() const { return m_vbuf; }
	auto& GetIBuf() const { return m_ibuf; }

private:
	void Build(const Body& body);

private:
	std::vector<float>    m_vbuf;
	std::vector<uint32_t> m_ibuf;

}; // VertexBuffer

}