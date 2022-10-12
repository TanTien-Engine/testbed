#pragma once

namespace brepdb { class RTree; }

namespace brepgraph
{

class RTree
{
public:
	static void LoadFromFile(brepdb::RTree& rtree, const char* filepath);

}; // RTree

}