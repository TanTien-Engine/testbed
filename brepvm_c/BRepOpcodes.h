#pragma once

namespace brepvmgraph
{

enum OpCode
{
	// 
	OP_STORE_POLY = 0,
	OP_STORE_MAT4,
	OP_STORE_SHAPE,

	OP_SHAPE_TO_POLY,

	OP_CLONE_POLY,
	OP_TRANSFORM_POLY,
	OP_EXTRUDE_POLY,
};

class BRepVM;

class OpCodeImpl
{
public:
	static void StorePoly(BRepVM* vm);
	static void StoreMat4(BRepVM* vm);
	static void StoreShape(BRepVM* vm);

	static void ShapeToPoly(BRepVM* vm);

	static void ClonePoly(BRepVM* vm);
	static void TransformPoly(BRepVM* vm);
	static void ExtrudePoly(BRepVM* vm);

	static void OpCodeInit(BRepVM* vm);

}; // OpCodeImpl

}