#pragma once

#include <memory>

namespace pm3 { class Polytope; }

namespace brepfw
{

class TopoShell;

class BRepBuilder
{
public:
	static std::shared_ptr<TopoShell> BuildShell(const pm3::Polytope& poly);

}; // BRepBuilder

}