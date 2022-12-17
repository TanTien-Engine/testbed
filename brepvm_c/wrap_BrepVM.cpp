#include "wrap_BrepVM.h"
#include "BRepCode.h"
#include "BRepOpcodes.h"
#include "BRepVM.h"
#include "VBO.h"
#include "modules/script/TransHelper.h"
#include "modules/render/Render.h"

#include <polymesh3/Polytope.h>
#include <unirender/Device.h>
#include <unirender/IndexBuffer.h>
#include <unirender/VertexBuffer.h>
#include <unirender/VertexInputAttribute.h>
#include <unirender/VertexArray.h>

#include <string>
#include <thread>

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

void w_BrepCode_store_number()
{
    auto code = ((tt::Proxy<brepvmgraph::BRepCode>*)ves_toforeign(0))->obj;

    uint8_t op = brepvmgraph::OP_STORE_NUMBER;
    code->Write(reinterpret_cast<const char*>(&op), sizeof(uint8_t));

    uint8_t reg = (uint8_t)ves_tonumber(1);
    code->Write(reinterpret_cast<const char*>(&reg), sizeof(uint8_t));

    double num = ves_tonumber(2);
    code->Write(reinterpret_cast<const char*>(&num), sizeof(double));
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

void w_BrepCode_poly_to_body()
{
    auto code = ((tt::Proxy<brepvmgraph::BRepCode>*)ves_toforeign(0))->obj;

    uint8_t op = brepvmgraph::OP_POLY_TO_BODY;
    code->Write(reinterpret_cast<const char*>(&op), sizeof(uint8_t));

    uint8_t r_poly = (uint8_t)ves_tonumber(1);
    code->Write(reinterpret_cast<const char*>(&r_poly), sizeof(uint8_t));

    uint8_t r_body = (uint8_t)ves_tonumber(2);
    code->Write(reinterpret_cast<const char*>(&r_body), sizeof(uint8_t));
}

void w_BrepCode_body_to_vbo()
{
    auto code = ((tt::Proxy<brepvmgraph::BRepCode>*)ves_toforeign(0))->obj;

    uint8_t op = brepvmgraph::OP_BODY_TO_VBUF;
    code->Write(reinterpret_cast<const char*>(&op), sizeof(uint8_t));

    uint8_t r_body = (uint8_t)ves_tonumber(1);
    code->Write(reinterpret_cast<const char*>(&r_body), sizeof(uint8_t));

    uint8_t r_vbo = (uint8_t)ves_tonumber(2);
    code->Write(reinterpret_cast<const char*>(&r_vbo), sizeof(uint8_t));
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

void w_BrepCode_transform_body()
{
    auto code = ((tt::Proxy<brepvmgraph::BRepCode>*)ves_toforeign(0))->obj;

    uint8_t op = brepvmgraph::OP_TRANSFORM_BODY;
    code->Write(reinterpret_cast<const char*>(&op), sizeof(uint8_t));

    uint8_t r_body = (uint8_t)ves_tonumber(1);
    code->Write(reinterpret_cast<const char*>(&r_body), sizeof(uint8_t));

    uint8_t r_mat4 = (uint8_t)ves_tonumber(2);
    code->Write(reinterpret_cast<const char*>(&r_mat4), sizeof(uint8_t));
}

void w_BrepCode_pattern_body()
{
    auto code = ((tt::Proxy<brepvmgraph::BRepCode>*)ves_toforeign(0))->obj;

    uint8_t op = brepvmgraph::OP_PATTERN_BODY;
    code->Write(reinterpret_cast<const char*>(&op), sizeof(uint8_t));

    uint8_t r_poly = (uint8_t)ves_tonumber(1);
    code->Write(reinterpret_cast<const char*>(&r_poly), sizeof(uint8_t));

    float dx = (float)ves_tonumber(2);
    code->Write(reinterpret_cast<const char*>(&dx), sizeof(float));
    float dy = (float)ves_tonumber(3);
    code->Write(reinterpret_cast<const char*>(&dy), sizeof(float));

    int nx = (int)ves_tonumber(4);
    code->Write(reinterpret_cast<const char*>(&nx), sizeof(int));
    int ny = (int)ves_tonumber(5);
    code->Write(reinterpret_cast<const char*>(&ny), sizeof(int));
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

void w_BrepVM_load()
{
    auto vm = ((tt::Proxy<brepvmgraph::BRepVM>*)ves_toforeign(0))->obj;
    uint8_t reg = (uint8_t)ves_tonumber(1);
    auto val = vm->Load(reg);

    ves_pop(ves_argnum());
    auto proxy = (tt::Proxy<brepvmgraph::Value>*)ves_set_newforeign(0, 0, sizeof(tt::Proxy<brepvmgraph::Value>));
    proxy->obj = std::make_shared<brepvmgraph::Value>(val);
}

void w_BrepTools_run_vm()
{
    std::vector<std::shared_ptr<brepvmgraph::BRepVM>> vms;
    tt::list_to_foreigns(1, vms);

    //for (auto& vm : vms)
    //{
    //    vm->Run();
    //}

    std::vector<std::thread> ts;
    for (auto& vm : vms)
    {
        ts.push_back(std::thread([&]() {
            vm->Run();
        }));
    }
    for (auto& t : ts) {
        t.join();
    }
}

void w_BrepTools_build_vao()
{
    std::vector<std::shared_ptr<brepvmgraph::Value>> vals;
    tt::list_to_foreigns(1, vals);

    size_t v_size = 0, i_size = 0;
    for (auto& v : vals) {
        v_size += v->as.vbo->GetVBuf().size();
        i_size += v->as.vbo->GetIBuf().size();
    }

    std::vector<float>    vdata;
    std::vector<uint32_t> idata;
    vdata.reserve(v_size);
    idata.reserve(i_size);
    for (auto& v : vals)
    {
        uint32_t offset = vdata.size() / 8;

        auto& _vdata = v->as.vbo->GetVBuf();
        std::copy(_vdata.begin(), _vdata.end(), std::back_inserter(vdata));

        auto& _idata = v->as.vbo->GetIBuf();
        for (auto& i : _idata) {
            idata.push_back(offset + i);
        }
    }

    auto dev = tt::Render::Instance()->Device();

    auto va = dev->CreateVertexArray();

    auto vbuf = dev->CreateVertexBuffer(ur::BufferUsageHint::StaticDraw, 0);
    auto vbuf_sz = sizeof(float) * vdata.size();
    vbuf->Reserve(vbuf_sz);
    vbuf->ReadFromMemory(vdata.data(), vbuf_sz, 0);
    va->SetVertexBuffer(vbuf);

    auto ibuf = dev->CreateIndexBuffer(ur::BufferUsageHint::StaticDraw, 0);
    auto ibuf_sz = sizeof(uint32_t) * idata.size();
    ibuf->SetCount(idata.size());
    ibuf->Reserve(ibuf_sz);
    ibuf->ReadFromMemory(idata.data(), ibuf_sz, 0);
    ibuf->SetDataType(ur::IndexBufferDataType::UnsignedInt);
    va->SetIndexBuffer(ibuf);

    std::vector<std::shared_ptr<ur::VertexInputAttribute>> vbuf_attrs;
    vbuf_attrs.resize(4);
    // pos
    vbuf_attrs[0] = std::make_shared<ur::VertexInputAttribute>(
        0, ur::ComponentDataType::Float, 3, 0, 32
        );
    // col
    vbuf_attrs[1] = std::make_shared<ur::VertexInputAttribute>(
        1, ur::ComponentDataType::Float, 3, 12, 32
        );
    // mat_id
    vbuf_attrs[2] = std::make_shared<ur::VertexInputAttribute>(
        2, ur::ComponentDataType::Float, 1, 24, 32
        );
    // offset
    vbuf_attrs[3] = std::make_shared<ur::VertexInputAttribute>(
        3, ur::ComponentDataType::Float, 1, 28, 32
        );
    va->SetVertexBufferAttrs(vbuf_attrs);

    ves_pop(ves_argnum());
    auto proxy = (tt::Proxy<ur::VertexArray>*)ves_set_newforeign(0, 0, sizeof(tt::Proxy<ur::VertexArray>));
    proxy->obj = va;
}

}

namespace brepvmgraph
{

VesselForeignMethodFn BrepVMBindMethod(const char* signature)
{
    if (strcmp(signature, "BrepCode.store_number(_,_)") == 0) return w_BrepCode_store_number;
    if (strcmp(signature, "BrepCode.store_polytope(_,_)") == 0) return w_BrepCode_store_polytope;
    if (strcmp(signature, "BrepCode.store_matrix4(_,_)") == 0) return w_BrepCode_store_matrix4;
    if (strcmp(signature, "BrepCode.store_shape(_,_)") == 0) return w_BrepCode_store_shape;
    if (strcmp(signature, "BrepCode.shape_to_polytope(_,_)") == 0) return w_BrepCode_shape_to_polytope;
    if (strcmp(signature, "BrepCode.poly_to_body(_,_)") == 0) return w_BrepCode_poly_to_body;
    if (strcmp(signature, "BrepCode.body_to_vbo(_,_)") == 0) return w_BrepCode_body_to_vbo;
    if (strcmp(signature, "BrepCode.clone_polytope(_,_)") == 0) return w_BrepCode_clone_polytope;
    if (strcmp(signature, "BrepCode.transform_polytope(_,_)") == 0) return w_BrepCode_transform_polytope;
    if (strcmp(signature, "BrepCode.extrude_polytope(_,_)") == 0) return w_BrepCode_extrude_polytope;
    if (strcmp(signature, "BrepCode.transform_body(_,_)") == 0) return w_BrepCode_transform_body;
    if (strcmp(signature, "BrepCode.pattern_body(_,_,_,_,_)") == 0) return w_BrepCode_pattern_body;

    if (strcmp(signature, "BrepVM.run()") == 0) return w_BrepVM_run;
    if (strcmp(signature, "BrepVM.load(_)") == 0) return w_BrepVM_load;

    if (strcmp(signature, "static BrepTools.run_vm(_)") == 0) return w_BrepTools_run_vm;
    if (strcmp(signature, "static BrepTools.build_vao(_)") == 0) return w_BrepTools_build_vao;

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