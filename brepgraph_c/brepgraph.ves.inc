#define QUOTE(...) #__VA_ARGS__
static const char* brepgraphModuleSource = QUOTE(

foreign class RTree
{
	init() {}
	init(db) {}

	foreign insert(poly)
	foreign query(key)

	foreign insert_with_time(poly, time)
	foreign query_with_time(key, tmin, tmax)

	foreign get_all_leaves()
	foreign query_leaves(region)
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