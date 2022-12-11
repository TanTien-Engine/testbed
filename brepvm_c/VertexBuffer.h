#pragma once

#include <vector>

namespace pm3 { class Polytope; }

namespace brepvmgraph
{

class VertexBuffer
{
public:
	VertexBuffer(const pm3::Polytope& poly);

	auto& GetVBuf() const { return m_vbuf; }
	auto& GetIBuf() const { return m_ibuf; }

private:
	void Build(const pm3::Polytope& poly);

private:
	std::vector<float>    m_vbuf;
	std::vector<uint32_t> m_ibuf;

}; // VertexBuffer

}