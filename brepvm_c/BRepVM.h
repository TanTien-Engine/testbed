#pragma once

#include <sm/SM_Vector.h>
#include <sm/SM_Matrix.h>

#include <vector>

namespace pm3 { class Polytope; }
namespace gs { class Shape2D; }

namespace brepvmgraph
{

enum class ValueType
{
    NIL,
    NUMBER,
    VECTOR3,
    MATRIX4,
    POLYTOPE,
    SHAPE2D,
};

struct Value
{
    ValueType type;
    union
    {
        double num;
        sm::vec3* vec3;
        sm::mat4* mat4;
        pm3::Polytope* poly;
        gs::Shape2D* shape2;
    } as;
};

#define REGISTER_COUNT 10

class BRepVM
{
public:
    BRepVM(const char* code, size_t size);
    ~BRepVM();

    void Run();
    void Run(int max_instructions);

    unsigned char NextByte();

    Value Load(int reg);

private:
    void InitOpcodes();

private:
    Value m_registers[REGISTER_COUNT];

    unsigned int m_ip = 0;

    const char* m_code = nullptr;
    size_t m_size = 0;

    typedef void OpcodeImpl(BRepVM* vm);
    OpcodeImpl* m_opcodes[256];

    std::vector<Value> m_tmp_vals;

    friend class OpCodeImpl;

}; // BRepVM

}