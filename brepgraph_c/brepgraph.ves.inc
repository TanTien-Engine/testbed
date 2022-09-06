#define QUOTE(...) #__VA_ARGS__
static const char* brepgraphModuleSource = QUOTE(

foreign class RTree
{
	init() {}

	foreign insert(poly)
	foreign query(key)
}

foreign class RKey
{
	init() {}
}

);