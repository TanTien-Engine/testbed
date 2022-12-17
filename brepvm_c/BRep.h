#pragma once

#include <sm/SM_Vector.h>

#include <vector>
#include <memory>

namespace brepvmgraph
{

struct Lump
{
	Lump() {}
	Lump(const Lump& l) 
		: points(l.points), faces(l.faces), faces_num(l.faces_num) 
	{
	}

	std::vector<sm::vec3> points;
	std::vector<size_t> faces, faces_num;
};

struct Body
{
	std::vector<std::shared_ptr<Lump>> lumps;
};

}