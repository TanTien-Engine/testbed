#pragma once

namespace brepdb { class RTree; }

namespace brepdbgraph
{

class RTree
{
public:
	static void LoadFromFile(brepdb::RTree& rtree, const char* filepath);

}; // RTree

}