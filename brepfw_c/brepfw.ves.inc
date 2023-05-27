#define QUOTE(...) #__VA_ARGS__
static const char* brepfwModuleSource = QUOTE(

foreign class TopoShape
{
}

foreign class TopoVertex is TopoShape
{
	init() {}
}

foreign class TopoEdge is TopoShape
{
	init() {}
}

foreign class TopoLoop is TopoShape
{
	init() {}
}

foreign class TopoFace is TopoShape
{
	init() {}
}

foreign class TopoShell is TopoShape
{
	init() {}
}

foreign class TopoBody is TopoShape
{
	init() {}
}

foreign class BRepTools
{
	foreign static poly2shape(poly)
	foreign static shape2vao(shape)	
}

);