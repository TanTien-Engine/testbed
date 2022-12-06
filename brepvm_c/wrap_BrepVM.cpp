#include "wrap_BrepVM.h"
#include "BRepCode.h"
#include "BRepOpcodes.h"
#include "BRepVM.h"
#include "modules/script/TransHelper.h"

#include <polymesh3/Polytope.h>

#include <string>

namespace
{

void w_BrepCode_allocate()
{
    auto proxy = (tt::Proxy<brepvmgraph::BRepCode>*)ves_set_newforeign(0, 0, sizeof(tt::Proxy<brepvmgraph::BRepCode>));
    proxy->obj = std::make_shared<brepvmgraph::BRepCode>();
}

int w_BrepCode_finalize(void* data)
{
    auto proxy = (tt::Proxy<brepvmgraph::BRepCode>*)(data);
    proxy->~Proxy();
    return sizeof(tt::Proxy<brepvmgraph::BRepCode>);
}

void w_BrepCode_store_polytope()
{
    auto code = ((tt::Proxy<brepvmgraph::BRepCode>*)ves_toforeign(0))->obj;

    uint8_t op = brepvmgraph::OP_STORE_POLY;
    code->Write(reinterpret_cast<const char*>(&op), sizeof(uint8_t));
     
    uint8_t reg = (uint8_t)ves_tonumber(1);
    code->Write(reinterpret_cast<const char*>(&reg), sizeof(uint8_t));

    auto poly = ((tt::Proxy<pm3::Polytope>*)ves_toforeign(2))->obj.get();
    code->Write(reinterpret_cast<const char*>(&poly), sizeof(void*));
}

void w_BrepCode_store_matrix4()
{
    auto code = ((tt::Proxy<brepvmgraph::BRepCode>*)ves_toforeign(0))->obj;

    uint8_t op = brepvmgraph::OP_STORE_MAT4;
    code->Write(reinterpret_cast<const char*>(&op), sizeof(uint8_t));

    uint8_t reg = (uint8_t)ves_tonumber(1);
    code->Write(reinterpret_cast<const char*>(&reg), sizeof(uint8_t));

    sm::mat4* mt = (sm::mat4*)ves_toforeign(2);
    code->Write(reinterpret_cast<const char*>(&mt), sizeof(void*));
}

void w_BrepCode_store_shape()
{
    auto code = ((tt::Proxy<brepvmgraph::BRepCode>*)ves_toforeign(0))->obj;

    uint8_t op = brepvmgraph::OP_STORE_SHAPE;
    code->Write(reinterpret_cast<const char*>(&op), sizeof(uint8_t));

    uint8_t reg = (uint8_t)ves_tonumber(1);
    code->Write(reinterpret_cast<const char*>(&reg), sizeof(uint8_t));

    auto shape = ((tt::Proxy<gs::Shape2D>*)ves_toforeign(2))->obj.get();
    code->Write(reinterpret_cast<const char*>(&shape), sizeof(void*));
}

void w_BrepCode_shape_to_polytope()
{
    auto code = ((tt::Proxy<brepvmgraph::BRepCode>*)ves_toforeign(0))->obj;

    uint8_t op = brepvmgraph::OP_SHAPE_TO_POLY;
    code->Write(reinterpret_cast<const char*>(&op), sizeof(uint8_t));

    uint8_t r_shape = (uint8_t)ves_tonumber(1);
    code->Write(reinterpret_cast<const char*>(&r_shape), sizeof(uint8_t));

    uint8_t r_poly = (uint8_t)ves_tonumber(2);
    code->Write(reinterpret_cast<const char*>(&r_poly), sizeof(uint8_t));
}

void w_BrepCode_clone_polytope()
{
    auto code = ((tt::Proxy<brepvmgraph::BRepCode>*)ves_toforeign(0))->obj;

    uint8_t op = brepvmgraph::OP_CLONE_POLY;
    code->Write(reinterpret_cast<const char*>(&op), sizeof(uint8_t));

    uint8_t r_src = (uint8_t)ves_tonumber(1);
    code->Write(reinterpret_cast<const char*>(&r_src), sizeof(uint8_t));

    uint8_t r_dst = (uint8_t)ves_tonumber(2);
    code->Write(reinterpret_cast<const char*>(&r_dst), sizeof(uint8_t));
}

void w_BrepCode_transform_polytope()
{
    auto code = ((tt::Proxy<brepvmgraph::BRepCode>*)ves_toforeign(0))->obj;

    uint8_t op = brepvmgraph::OP_TRANSFORM_POLY;
    code->Write(reinterpret_cast<const char*>(&op), sizeof(uint8_t));

    uint8_t r_poly = (uint8_t)ves_tonumber(1);
    code->Write(reinterpret_cast<const char*>(&r_poly), sizeof(uint8_t));

    uint8_t r_mat4 = (uint8_t)ves_tonumber(2);
    code->Write(reinterpret_cast<const char*>(&r_mat4), sizeof(uint8_t));
}

void w_BrepCode_extrude_polytope()
{
    auto code = ((tt::Proxy<brepvmgraph::BRepCode>*)ves_toforeign(0))->obj;

    uint8_t op = brepvmgraph::OP_EXTRUDE_POLY;
    code->Write(reinterpret_cast<const char*>(&op), sizeof(uint8_t));

    uint8_t r_poly = (uint8_t)ves_tonumber(1);
    code->Write(reinterpret_cast<const char*>(&r_poly), sizeof(uint8_t));

    float dist = (float)ves_tonumber(2);
    code->Write(reinterpret_cast<const char*>(&dist), sizeof(float));
}

void w_BrepVM_allocate()
{
    auto proxy = (tt::Proxy<brepvmgraph::BRepVM>*)ves_set_newforeign(0, 0, sizeof(tt::Proxy<brepvmgraph::BRepCode>));
    auto b_code = ((tt::Proxy<brepvmgraph::BRepCode>*)ves_toforeign(1))->obj;
    auto& code = b_code->GetCode();
    proxy->obj = std::make_shared<brepvmgraph::BRepVM>((const char*)code.data(), code.size());
}

int w_BrepVM_finalize(void* data)
{
    auto proxy = (tt::Proxy<brepvmgraph::BRepVM>*)(data);
    proxy->~Proxy();
    return sizeof(tt::Proxy<brepvmgraph::BRepVM>);
}

void w_BrepVM_run()
{
    auto vm = ((tt::Proxy<brepvmgraph::BRepVM>*)ves_toforeign(0))->obj;
    vm->Run();
}

void w_BrepVM_load_register()
{
    auto vm = ((tt::Proxy<brepvmgraph::BRepVM>*)ves_toforeign(0))->obj;
    uint8_t reg = (uint8_t)ves_tonumber(1);
    auto val = vm->Load(reg);
    if (val.type == brepvmgraph::ValueType::POLYTOPE) 
    {
        ves_pop(ves_argnum());
        auto proxy = (tt::Proxy<pm3::Polytope>*)ves_set_newforeign(0, 0, sizeof(tt::Proxy<pm3::Polytope>));
        proxy->obj = std::shared_ptr<pm3::Polytope>(val.as.poly);
    } 
    else 
    {
        ves_set_nil(0);
    }
}

}

namespace brepvmgraph
{

VesselForeignMethodFn BrepVMBindMethod(const char* signature)
{
    if (strcmp(signature, "BrepCode.store_polytope(_,_)") == 0) return w_BrepCode_store_polytope;
    if (strcmp(signature, "BrepCode.store_matrix4(_,_)") == 0) return w_BrepCode_store_matrix4;
    if (strcmp(signature, "BrepCode.store_shape(_,_)") == 0) return w_BrepCode_store_shape;
    if (strcmp(signature, "BrepCode.shape_to_polytope(_,_)") == 0) return w_BrepCode_shape_to_polytope;
    if (strcmp(signature, "BrepCode.clone_polytope(_,_)") == 0) return w_BrepCode_clone_polytope;
    if (strcmp(signature, "BrepCode.transform_polytope(_,_)") == 0) return w_BrepCode_transform_polytope;
    if (strcmp(signature, "BrepCode.extrude_polytope(_,_)") == 0) return w_BrepCode_extrude_polytope;

    if (strcmp(signature, "BrepVM.run()") == 0) return w_BrepVM_run;
    if (strcmp(signature, "BrepVM.load_register(_)") == 0) return w_BrepVM_load_register;

	return nullptr;
}

void BrepVMBindClass(const char* class_name, VesselForeignClassMethods* methods)
{
    if (strcmp(class_name, "BrepCode") == 0)
    {
        methods->allocate = w_BrepCode_allocate;
        methods->finalize = w_BrepCode_finalize;
        return;
    }

    if (strcmp(class_name, "BrepVM") == 0)
    {
        methods->allocate = w_BrepVM_allocate;
        methods->finalize = w_BrepVM_finalize;
        return;
    }
}

}