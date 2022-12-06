#define QUOTE(...) #__VA_ARGS__
static const char* brepvmModuleSource = QUOTE(

foreign class BrepCode
{
	init() {}

	foreign store_polytope(reg, poly)
	foreign store_matrix4(reg, mat4)
	foreign store_shape(reg, shape)

	foreign shape_to_polytope(shape, poly)

	foreign clone_polytope(src, dst)
	foreign transform_polytope(poly, mat4)
	foreign extrude_polytope(poly, dist)
}

foreign class BrepVM
{
	init(code) {}

	foreign run()

	foreign load_register(reg)
}

);