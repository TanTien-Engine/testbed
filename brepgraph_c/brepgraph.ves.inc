#define QUOTE(...) #__VA_ARGS__
static const char* brepgraphModuleSource = QUOTE(

foreign class RTree
{
	init() {}
	init(db) {}

	foreign insert(poly)
	foreign query(key)

	foreign get_regions()
}

foreign class RKey
{
	init() {}
	init(aabb) {}
}

foreign class RFile
{
	init(filename) {}
}

);