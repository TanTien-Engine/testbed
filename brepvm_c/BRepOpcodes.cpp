#include "BRepOpcodes.h"
#include "BRepVM.h"
#include "VBO.h"
#include "BRep.h"

#include <polymesh3/Polytope.h>
#include <geoshape/Shape2D.h>
#include <geoshape/Rect.h>

#include <iterator>

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

template<typename T>
T read_number(brepvmgraph::BRepVM* vm)
{
    T ret = 0;

    const int sz = sizeof(T);
    uint8_t data[sz];
    for (int i = 0; i < sz; ++i) {
        data[i] = vm->NextByte();
    }
    memcpy(&ret, data, sz);

    return ret;
}

}

namespace brepvmgraph
{

void OpCodeImpl::StoreNumber(BRepVM* vm)
{
    uint8_t reg = vm->NextByte();
    assert(reg >= 0 && reg < REGISTER_COUNT);

    double num = read_number<double>(vm);

    vm->m_registers[reg].type = ValueType::NUMBER;
    vm->m_registers[reg].as.num = num;
}

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

void OpCodeImpl::StoreShape(BRepVM* vm)
{
    uint8_t reg = vm->NextByte();
    assert(reg >= 0 && reg < REGISTER_COUNT);

    gs::Shape2D* shape = (gs::Shape2D*)read_pointer(vm);

    vm->m_registers[reg].type = ValueType::SHAPE2D;
    vm->m_registers[reg].as.shape2 = shape;
}

void OpCodeImpl::ShapeToPoly(BRepVM* vm)
{
    uint8_t r_src = vm->NextByte();
    assert(r_src >= 0 && r_src < REGISTER_COUNT
        && vm->m_registers[r_src].type == ValueType::SHAPE2D);

    pm3::Polytope* dst = nullptr;

    auto src = vm->m_registers[r_src].as.shape2;
    switch (src->GetType())
    {
    case gs::ShapeType2D::Rect:
    {
        auto rect = static_cast<gs::Rect*>(src);

        auto& r = rect->GetRect();

        std::vector<pm3::Polytope::PointPtr> points = {
            std::make_shared<pm3::Polytope::Point>(sm::vec3(r.xmin, 0, r.ymin)),
            std::make_shared<pm3::Polytope::Point>(sm::vec3(r.xmax, 0, r.ymin)),
            std::make_shared<pm3::Polytope::Point>(sm::vec3(r.xmax, 0, r.ymax)),
            std::make_shared<pm3::Polytope::Point>(sm::vec3(r.xmin, 0, r.ymax)),
        };

        auto f = std::make_shared<pm3::Polytope::Face>();
        f->border = { 0, 1, 2, 3 };

        dst = new pm3::Polytope(points, { f });
    }
        break;
    }

    uint8_t r_dst = vm->NextByte();
    if (dst) {
        vm->m_registers[r_dst].type = ValueType::POLYTOPE;
        vm->m_registers[r_dst].as.poly = dst;
    } else {
        vm->m_registers[r_dst].type = ValueType::NIL;
    }

    vm->m_tmp_vals.push_back(vm->m_registers[r_dst]);
}

void OpCodeImpl::PolyToBody(BRepVM* vm)
{
    uint8_t r_src = vm->NextByte();
    assert(r_src >= 0 && r_src < REGISTER_COUNT
        && vm->m_registers[r_src].type == ValueType::POLYTOPE);

    auto src = vm->m_registers[r_src].as.poly;

    auto lump = std::make_shared<Lump>();
    lump->points.reserve(src->Points().size());
    for (auto& p : src->Points()) {
        lump->points.push_back(p->pos);
    }
    for (auto& f : src->Faces()) {
        std::copy(f->border.begin(), f->border.end(), std::back_inserter(lump->faces));
        lump->faces_num.push_back(f->border.size());
    }

    auto dst = new Body;
    dst->lumps.push_back(lump);

    uint8_t r_dst = vm->NextByte();
    if (dst) {
        vm->m_registers[r_dst].type = ValueType::BODY;
        vm->m_registers[r_dst].as.body = dst;
    } else {
        vm->m_registers[r_dst].type = ValueType::NIL;
    }

    vm->m_tmp_vals.push_back(vm->m_registers[r_dst]);
}

void OpCodeImpl::BodyToVBuf(BRepVM* vm)
{
    uint8_t r_src = vm->NextByte();
    assert(r_src >= 0 && r_src < REGISTER_COUNT
        && vm->m_registers[r_src].type == ValueType::BODY);

    auto src = vm->m_registers[r_src].as.body;
    auto dst = new VBO(*src);

    uint8_t r_dst = vm->NextByte();
    if (dst) {
        vm->m_registers[r_dst].type = ValueType::VBO;
        vm->m_registers[r_dst].as.vbo = dst;
    } else {
        vm->m_registers[r_dst].type = ValueType::NIL;
    }

    vm->m_tmp_vals.push_back(vm->m_registers[r_dst]);
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

void OpCodeImpl::ExtrudePoly(BRepVM* vm)
{
    uint8_t r_poly = vm->NextByte();
    assert(r_poly >= 0 && r_poly < REGISTER_COUNT
        && vm->m_registers[r_poly].type == ValueType::POLYTOPE);
    float dist = read_number<float>(vm);

    auto poly = vm->m_registers[r_poly].as.poly;

    auto topo_poly = poly->GetTopoPoly();
    if (!topo_poly) {
        return;
    }

    bool create_face[he::Polyhedron::ExtrudeMaxCount];
    create_face[he::Polyhedron::ExtrudeFront] = true;
    create_face[he::Polyhedron::ExtrudeBack] = true;
    create_face[he::Polyhedron::ExtrudeSide] = true;

    std::vector<he::TopoID> face_ids;
    auto& faces = topo_poly->GetLoops();
    face_ids.reserve(faces.Size());
    auto first_f = faces.Head();
    auto curr_f = first_f;
    do {
        face_ids.push_back(curr_f->ids);
        curr_f = curr_f->linked_next;
    } while (curr_f != first_f);

    if (!topo_poly->Extrude(dist, face_ids, create_face)) {
        return;
    }

    poly->BuildFromTopo();
}

void OpCodeImpl::TransformBody(BRepVM* vm)
{
    uint8_t r_body = vm->NextByte();
    assert(r_body >= 0 && r_body < REGISTER_COUNT 
        && vm->m_registers[r_body].type == ValueType::BODY);
    uint8_t r_mat4 = vm->NextByte();
    assert(r_mat4 >= 0 && r_mat4 < REGISTER_COUNT
        && vm->m_registers[r_mat4].type == ValueType::MATRIX4);

    auto body = vm->m_registers[r_body].as.body;
    auto mat4 = vm->m_registers[r_mat4].as.mat4;

    for (auto& lump : body->lumps) {
        for (auto& p : lump->points) {
            p = *mat4 * p;
        }
    }
}

void OpCodeImpl::PatternBody(BRepVM* vm)
{
    uint8_t r_body = vm->NextByte();
    assert(r_body >= 0 && r_body < REGISTER_COUNT
        && vm->m_registers[r_body].type == ValueType::BODY);
    float dx = read_number<float>(vm);
    float dy = read_number<float>(vm);
    int nx = read_number<int>(vm);
    int ny = read_number<int>(vm);

    auto body = vm->m_registers[r_body].as.body;

    auto ret = new Body;
    for (int y = 0; y < ny; ++y)
    {
        for (int x = 0; x < nx; ++x)
        {
            auto mat = sm::mat4::Translated(x * dx, 0, y * dy);

            for (auto& lump : body->lumps)
            {
                auto new_lump = std::make_shared<brepvmgraph::Lump>(*lump);
                for (auto& p : new_lump->points) {
                    p = mat * p;
                }

                ret->lumps.push_back(new_lump);
            }
        }
    }

    vm->m_registers[r_body].as.body = ret;
}

void OpCodeImpl::OpCodeInit(BRepVM* vm)
{
    vm->m_opcodes[OP_STORE_NUMBER] = StoreNumber;
	vm->m_opcodes[OP_STORE_POLY] = StorePoly;
    vm->m_opcodes[OP_STORE_MAT4] = StoreMat4;
    vm->m_opcodes[OP_STORE_SHAPE] = StoreShape;

    vm->m_opcodes[OP_SHAPE_TO_POLY] = ShapeToPoly;
    vm->m_opcodes[OP_POLY_TO_BODY] = PolyToBody;
    vm->m_opcodes[OP_BODY_TO_VBUF] = BodyToVBuf;

    vm->m_opcodes[OP_CLONE_POLY] = ClonePoly;
    vm->m_opcodes[OP_TRANSFORM_POLY] = TransformPoly;
    vm->m_opcodes[OP_EXTRUDE_POLY] = ExtrudePoly;

    vm->m_opcodes[OP_TRANSFORM_BODY] = TransformBody;
    vm->m_opcodes[OP_PATTERN_BODY] = PatternBody;
}

}