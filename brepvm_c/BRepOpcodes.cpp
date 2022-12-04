#include "BRepOpcodes.h"
#include "BRepVM.h"

#include <polymesh3/Polytope.h>

namespace
{

void* read_pointer(brepvmgraph::BRepVM* vm)
{
    void* pointer = nullptr;

    const int sz = sizeof(void*);
    uint8_t data[sz];
    for (int i = 0; i < sz; ++i) {
        data[i] = vm->NextByte();
    }
    memcpy(&pointer, data, sz);

    return pointer;
}

}

namespace brepvmgraph
{

void OpCodeImpl::StorePoly(BRepVM* vm)
{
    uint8_t reg = vm->NextByte();
    assert(reg >= 0 && reg < REGISTER_COUNT);

    pm3::Polytope* poly = (pm3::Polytope*)read_pointer(vm);

    vm->m_registers[reg].type = ValueType::POLYTOPE;
    vm->m_registers[reg].as.poly = poly;
}

void OpCodeImpl::StoreMat4(BRepVM* vm)
{
    uint8_t reg = vm->NextByte();
    assert(reg >= 0 && reg < REGISTER_COUNT);

    sm::mat4* mat4 = (sm::mat4*)read_pointer(vm);

    vm->m_registers[reg].type = ValueType::MATRIX4;
    vm->m_registers[reg].as.mat4 = mat4;
}

void OpCodeImpl::ClonePoly(BRepVM* vm)
{
    uint8_t r_src = vm->NextByte();
    assert(r_src >= 0 && r_src < REGISTER_COUNT
        && vm->m_registers[r_src].type == ValueType::POLYTOPE);

    auto src = vm->m_registers[r_src].as.poly;

    auto dst = new pm3::Polytope();
    *dst = *src;

    uint8_t r_dst = vm->NextByte();
    vm->m_registers[r_dst].type = ValueType::POLYTOPE;
    vm->m_registers[r_dst].as.poly = dst;

    vm->m_tmp_vals.push_back(vm->m_registers[r_dst]);
}

void OpCodeImpl::TransformPoly(BRepVM* vm)
{
    uint8_t r_poly = vm->NextByte();
    assert(r_poly >= 0 && r_poly < REGISTER_COUNT 
        && vm->m_registers[r_poly].type == ValueType::POLYTOPE);
    uint8_t r_mat4 = vm->NextByte();
    assert(r_mat4 >= 0 && r_mat4 < REGISTER_COUNT
        && vm->m_registers[r_mat4].type == ValueType::MATRIX4);

    auto poly = vm->m_registers[r_poly].as.poly;
    auto mat4 = vm->m_registers[r_mat4].as.mat4;

    auto& pts = poly->Points();
    for (auto& p : pts) {
        p->pos = *mat4 * p->pos;
    }

    poly->SetTopoDirty();
}

void OpCodeImpl::OpCodeInit(BRepVM* vm)
{
	vm->m_opcodes[OP_STORE_POLY] = StorePoly;
    vm->m_opcodes[OP_STORE_MAT4] = StoreMat4;

    vm->m_opcodes[OP_CLONE_POLY] = ClonePoly;
    vm->m_opcodes[OP_TRANSFORM_POLY] = TransformPoly;
}

}