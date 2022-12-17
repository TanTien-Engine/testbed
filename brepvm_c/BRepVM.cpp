#include "BRepVM.h"
#include "BRepOpcodes.h"
#include "BRep.h"
#include "VBO.h"

#include <polymesh3/Polytope.h>

#include <thread>

namespace brepvmgraph
{

BRepVM::BRepVM(const char* code, size_t size)
    : m_code(code)
    , m_size(size)
{
	InitOpcodes();
}

BRepVM::~BRepVM()
{
    for (auto& val : m_tmp_vals)
    {
        switch (val.type)
        {
        case ValueType::VECTOR3:
            delete val.as.vec3;
            break;
        case ValueType::MATRIX4:
            delete val.as.mat4;
            break;
        case ValueType::POLYTOPE:
            delete val.as.poly;
            break;
        case ValueType::BODY:
            delete val.as.body;
            break;
        case ValueType::VBO:
            delete val.as.vbo;
            break;
        default:
            assert(0);
        }
    }
}

void BRepVM::Run()
{
    Run(0);
}

void BRepVM::Run(int max_instructions)
{
    m_ip = 0;

    int iterations = 0;
    while (true)
    {
        //if (m_ip >= m_size) {
        //    m_ip = 0;
        //}

        int opcode = m_code[m_ip];

        if (m_opcodes[opcode]) {
            m_opcodes[opcode](this);
            NextByte();
        }

        //if (++iterations >= max_instructions) {
        //    break;
        //}
        if (m_ip >= m_size) {
            break;
        }
    }
}

unsigned char BRepVM::NextByte()
{
    m_ip += 1;

    if (m_ip >= 0xFFFF) {
        m_ip = 0;
    }

    return m_code[m_ip];
}

Value BRepVM::Load(int reg)
{
    auto ret = m_registers[reg];

    for (auto itr = m_tmp_vals.begin(); itr != m_tmp_vals.end(); ++itr) {
        if (std::memcmp(&*itr, &ret, sizeof(brepvmgraph::Value)) == 0) {
            m_tmp_vals.erase(itr);
            break;
        }
    }

    return ret;
}

void BRepVM::InitOpcodes()
{
    OpCodeImpl::OpCodeInit(this);
}

}