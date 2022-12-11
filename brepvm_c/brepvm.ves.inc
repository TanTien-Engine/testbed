#define QUOTE(...) #__VA_ARGS__
static const char* brepvmModuleSource = QUOTE(

foreign class BrepCode
{
	init() {}

	foreign store_number(reg, num)
	foreign store_polytope(reg, poly)
	foreign store_matrix4(reg, mat4)
	foreign store_shape(reg, shape)

	foreign shape_to_polytope(shape, poly)
	foreign poly_to_vbuf(poly, vbuf)

	foreign clone_polytope(src, dst)
	foreign transform_polytope(poly, mat4)
	foreign extrude_polytope(poly, dist)
	foreign pattern_polytope(poly, dx, dy, nx, ny)
}

foreign class BrepVM
{
	init(code) {}

	foreign run()
	foreign load(reg)
}

foreign class BrepTools
{
	foreign static run_vm(vm)
	foreign static build_vao(val)
}

);