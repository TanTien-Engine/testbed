#include "wrap_BrepFW.h"
#include "Label.h"
#include "TopoShape.h"
#include "TopoVertex.h"
#include "TopoEdge.h"
#include "TopoLoop.h"
#include "TopoFace.h"
#include "TopoShell.h"
#include "TopoBody.h"
#include "BRepBuilder.h"
#include "RenderBuilder.h"
#include "AttrNamedShape.h"
#include "AttrRenderObj.h"
#include "AttrColor.h"
#include "TopoAdapter.h"
#include "modules/script/TransHelper.h"
#include "modules/render/Render.h"

#include <polymesh3/Polytope.h>

#include <memory>
#include <iterator>

namespace
{

void w_BRepTools_poly2shape()
{
    auto poly = ((tt::Proxy<pm3::Polytope>*)ves_toforeign(1))->obj;

    std::vector<sm::vec3> points;
    auto& src_points = poly->Points();
    points.reserve(src_points.size());
    for (auto& p : src_points) {
        points.push_back(p->pos);
    }

    std::vector<std::vector<uint32_t>> faces;
    auto& src_faces = poly->Faces();
    faces.reserve(src_faces.size());
    for (auto& s_f : src_faces) 
    {
        std::vector<uint32_t> f;
        std::copy(s_f->border.begin(), s_f->border.end(), std::back_inserter(f));
        faces.push_back(f);
    }

    auto shape = brepfw::BRepBuilder::BuildShell(points, faces);

    ves_pop(ves_argnum());

    ves_pushnil();
    ves_import_class("brepfw", "TopoShell");
    auto proxy = (tt::Proxy<brepfw::TopoShell>*)ves_set_newforeign(0, 1, sizeof(tt::Proxy<brepfw::TopoShell>));
    proxy->obj = shape;
    ves_pop(1);
}

void w_BRepTools_shape2vao()
{
    auto shape = ((tt::Proxy<brepfw::TopoShape>*)ves_toforeign(1))->obj;
    auto color = tt::map_to_vec3(2);

    auto dev = tt::Render::Instance()->Device();
    auto vao = brepfw::RenderBuilder::BuildVAO(*dev, shape, color);

    ves_pop(ves_argnum());

    ves_pushnil();
    ves_import_class("render", "VertexArray");
    auto proxy = (tt::Proxy<ur::VertexArray>*)ves_set_newforeign(0, 1, sizeof(tt::Proxy<ur::VertexArray>));
    proxy->obj = vao;
    ves_pop(1);
}

void w_BRepAlgos_clip()
{
    auto shape = ((tt::Proxy<brepfw::TopoShape>*)ves_toforeign(1))->obj;
    sm::Plane* plane = (sm::Plane*)ves_toforeign(2);

    auto keep_str = ves_tostring(3);
    auto keep = he::Polyhedron::KeepType::KeepAbove;
    if (strcmp(keep_str, "above") == 0) {
        keep = he::Polyhedron::KeepType::KeepAbove;
    } else if (strcmp(keep_str, "below") == 0) {
        keep = he::Polyhedron::KeepType::KeepBelow;
    } else if (strcmp(keep_str, "all") == 0) {
        keep = he::Polyhedron::KeepType::KeepAll;
    }

    auto seam_face = ves_toboolean(4);

    auto topo = brepfw::TopoAdapter::BRep2Topo(shape);
    if (!topo->Clip(*plane, keep, seam_face)) {
        ves_set_nil(0);
        return;
    }

    brepfw::TopoAdapter::Topo2BRep(shape, topo);

    auto ret = brepfw::TopoAdapter::Topo2BRep(topo);

    ves_pop(ves_argnum());

    ves_pushnil();
    ves_import_class("brepfw", "TopoShape");
    auto proxy = (tt::Proxy<brepfw::TopoShape>*)ves_set_newforeign(0, 1, sizeof(tt::Proxy<brepfw::TopoShape>));
    proxy->obj = ret;
    ves_pop(1);
}

void w_Label_allocate()
{
    auto proxy = (tt::Proxy<brepfw::Label>*)ves_set_newforeign(0, 0, sizeof(tt::Proxy<brepfw::Label>));
        proxy->obj = std::make_shared<brepfw::Label>();
}

int w_Label_finalize(void* data)
{
    auto proxy = (tt::Proxy<brepfw::Label>*)(data);
    proxy->~Proxy();
    return sizeof(tt::Proxy<brepfw::Label>);
}

void w_Label_set_shape()
{
    auto label = ((tt::Proxy<brepfw::Label>*)ves_toforeign(0))->obj;
    auto shape = ((tt::Proxy<brepfw::TopoShape>*)ves_toforeign(1))->obj;
    label->AddComponent<brepfw::AttrNamedShape>(nullptr, shape);
}

void w_Label_get_shape()
{
    auto label = ((tt::Proxy<brepfw::Label>*)ves_toforeign(0))->obj;
    if (!label->HasComponent<brepfw::AttrNamedShape>()) {
        ves_set_nil(0);
        return;
    }

    auto& named_shape = label->GetComponent<brepfw::AttrNamedShape>();
    auto shape = named_shape.GetDst();

    ves_pop(ves_argnum());

    ves_pushnil();
    ves_import_class("brepfw", "TopoShape");
    auto proxy = (tt::Proxy<brepfw::TopoShape>*)ves_set_newforeign(0, 1, sizeof(tt::Proxy<brepfw::TopoShape>));
    proxy->obj = shape;
    ves_pop(1);
}

void w_Label_set_render_obj()
{
    auto label = ((tt::Proxy<brepfw::Label>*)ves_toforeign(0))->obj;
    auto va = ((tt::Proxy<ur::VertexArray>*)ves_toforeign(1))->obj;
    label->AddComponent<brepfw::AttrRenderObj>(va);
}

void w_Label_get_render_obj()
{
    auto label = ((tt::Proxy<brepfw::Label>*)ves_toforeign(0))->obj;
    if (!label->HasComponent<brepfw::AttrRenderObj>()) {
        ves_set_nil(0);
        return;
    }

    auto& render_obj = label->GetComponent<brepfw::AttrRenderObj>();
    auto va = render_obj.GetVA();

    ves_pop(ves_argnum());

    ves_pushnil();
    ves_import_class("render", "VertexArray");
    auto proxy = (tt::Proxy<ur::VertexArray>*)ves_set_newforeign(0, 1, sizeof(tt::Proxy<ur::VertexArray>));
    proxy->obj = va;
    ves_pop(1);
}

void w_Label_set_color()
{
    auto label = ((tt::Proxy<brepfw::Label>*)ves_toforeign(0))->obj;
    sm::vec3 rgb = tt::map_to_vec3(1);
    label->AddComponent<brepfw::AttrColor>(rgb);
}

void w_Label_get_color()
{
    auto label = ((tt::Proxy<brepfw::Label>*)ves_toforeign(0))->obj;
    if (!label->HasComponent<brepfw::AttrColor>()) {
        ves_set_nil(0);
        return;
    }

    auto& color = label->GetComponent<brepfw::AttrColor>();
    tt::return_vec(color.GetColor());
}

void w_Label_build_vao()
{
    auto label = ((tt::Proxy<brepfw::Label>*)ves_toforeign(0))->obj;

}

void w_TopoVertex_allocate()
{
    auto proxy = (tt::Proxy<brepfw::TopoVertex>*)ves_set_newforeign(0, 0, sizeof(tt::Proxy<brepfw::TopoVertex>));
    proxy->obj = std::make_shared<brepfw::TopoVertex>();
}

int w_TopoVertex_finalize(void* data)
{
    auto proxy = (tt::Proxy<brepfw::TopoVertex>*)(data);
    proxy->~Proxy();
    return sizeof(tt::Proxy<brepfw::TopoVertex>);
}

void w_TopoEdge_allocate()
{
    auto proxy = (tt::Proxy<brepfw::TopoEdge>*)ves_set_newforeign(0, 0, sizeof(tt::Proxy<brepfw::TopoEdge>));
    proxy->obj = std::make_shared<brepfw::TopoEdge>();
}

int w_TopoEdge_finalize(void* data)
{
    auto proxy = (tt::Proxy<brepfw::TopoEdge>*)(data);
    proxy->~Proxy();
    return sizeof(tt::Proxy<brepfw::TopoEdge>);
}

void w_TopoLoop_allocate()
{
    auto proxy = (tt::Proxy<brepfw::TopoLoop>*)ves_set_newforeign(0, 0, sizeof(tt::Proxy<brepfw::TopoLoop>));
    proxy->obj = std::make_shared<brepfw::TopoLoop>();
}

int w_TopoLoop_finalize(void* data)
{
    auto proxy = (tt::Proxy<brepfw::TopoLoop>*)(data);
    proxy->~Proxy();
    return sizeof(tt::Proxy<brepfw::TopoLoop>);
}

void w_TopoFace_allocate()
{
    auto proxy = (tt::Proxy<brepfw::TopoFace>*)ves_set_newforeign(0, 0, sizeof(tt::Proxy<brepfw::TopoFace>));
    proxy->obj = std::make_shared<brepfw::TopoFace>();
}

int w_TopoFace_finalize(void* data)
{
    auto proxy = (tt::Proxy<brepfw::TopoFace>*)(data);
    proxy->~Proxy();
    return sizeof(tt::Proxy<brepfw::TopoFace>);
}

void w_TopoShell_allocate()
{
    auto proxy = (tt::Proxy<brepfw::TopoShell>*)ves_set_newforeign(0, 0, sizeof(tt::Proxy<brepfw::TopoShell>));
    proxy->obj = std::make_shared<brepfw::TopoShell>();
}

int w_TopoShell_finalize(void* data)
{
    auto proxy = (tt::Proxy<brepfw::TopoShell>*)(data);
    proxy->~Proxy();
    return sizeof(tt::Proxy<brepfw::TopoShell>);
}

void w_TopoBody_allocate()
{
    auto proxy = (tt::Proxy<brepfw::TopoBody>*)ves_set_newforeign(0, 0, sizeof(tt::Proxy<brepfw::TopoBody>));
    proxy->obj = std::make_shared<brepfw::TopoBody>();
}

int w_TopoBody_finalize(void* data)
{
    auto proxy = (tt::Proxy<brepfw::TopoBody>*)(data);
    proxy->~Proxy();
    return sizeof(tt::Proxy<brepfw::TopoBody>);
}

}

namespace brepfwgraph
{

VesselForeignMethodFn BrepFWBindMethod(const char* signature)
{
    if (strcmp(signature, "static BRepTools.poly2shape(_)") == 0) return w_BRepTools_poly2shape;
    if (strcmp(signature, "static BRepTools.shape2vao(_,_)") == 0) return w_BRepTools_shape2vao;

    if (strcmp(signature, "static BRepAlgos.clip(_,_,_,_)") == 0) return w_BRepAlgos_clip;

    if (strcmp(signature, "Label.set_shape(_)") == 0) return w_Label_set_shape;
    if (strcmp(signature, "Label.get_shape()") == 0) return w_Label_get_shape;

    if (strcmp(signature, "Label.set_render_obj(_)") == 0) return w_Label_set_render_obj;
    if (strcmp(signature, "Label.get_render_obj()") == 0) return w_Label_get_render_obj;

    if (strcmp(signature, "Label.set_color(_)") == 0) return w_Label_set_color;
    if (strcmp(signature, "Label.get_color()") == 0) return w_Label_get_color;

    if (strcmp(signature, "Label.build_vao()") == 0) return w_Label_build_vao;

	return nullptr;
}

void BrepFWBindClass(const char* class_name, VesselForeignClassMethods* methods)
{
    if (strcmp(class_name, "Label") == 0)
    {
        methods->allocate = w_Label_allocate;
        methods->finalize = w_Label_finalize;
        return;
    }

    if (strcmp(class_name, "TopoVertex") == 0)
    {
        methods->allocate = w_TopoVertex_allocate;
        methods->finalize = w_TopoVertex_finalize;
        return;
    }

    if (strcmp(class_name, "TopoEdge") == 0)
    {
        methods->allocate = w_TopoEdge_allocate;
        methods->finalize = w_TopoEdge_finalize;
        return;
    }

    if (strcmp(class_name, "TopoLoop") == 0)
    {
        methods->allocate = w_TopoLoop_allocate;
        methods->finalize = w_TopoLoop_finalize;
        return;
    }

    if (strcmp(class_name, "TopoFace") == 0)
    {
        methods->allocate = w_TopoFace_allocate;
        methods->finalize = w_TopoFace_finalize;
        return;
    }

    if (strcmp(class_name, "TopoShell") == 0)
    {
        methods->allocate = w_TopoShell_allocate;
        methods->finalize = w_TopoShell_finalize;
        return;
    }

    if (strcmp(class_name, "TopoBody") == 0)
    {
        methods->allocate = w_TopoBody_allocate;
        methods->finalize = w_TopoBody_finalize;
        return;
    }
}

}