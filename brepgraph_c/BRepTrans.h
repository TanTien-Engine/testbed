#pragma once

#include <cstdint>
#include <memory>

namespace pm3 { class Polytope; }

namespace brepgraph
{

class BRepTrans
{
public:
	static void BRepToByteArray(const pm3::Polytope& brep, uint8_t** data, uint32_t& length);
	static std::shared_ptr<pm3::Polytope> BRepFromByteArray(const uint8_t* data);

}; // BRepTrans

}