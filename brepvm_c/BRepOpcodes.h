#pragma once

namespace brepvmgraph
{

enum OpCode
{
	// 
	OP_STORE_POLY = 0,
	OP_STORE_MAT4,

	OP_CLONE_POLY,
	OP_TRANSFORM_POLY,
};

class BRepVM;

class OpCodeImpl
{
public:
	static void StorePoly(BRepVM* vm);
	static void StoreMat4(BRepVM* vm);

	static void ClonePoly(BRepVM* vm);
	static void TransformPoly(BRepVM* vm);

	static void OpCodeInit(BRepVM* vm);

}; // OpCodeImpl

}