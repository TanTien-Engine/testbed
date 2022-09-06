#pragma once

#include <brepdb/Region.h>

namespace brepgraph
{

class BRepKey
{
public:
	brepdb::Region r;
	int64_t id = 0;

}; // BRepKey

}