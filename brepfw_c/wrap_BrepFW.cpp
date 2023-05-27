#include "wrap_BrepFW.h"
#include "TopoShape.h"
#include "TopoVertex.h"
#include "TopoEdge.h"
#include "TopoLoop.h"
#include "TopoFace.h"
#include "TopoShell.h"
#include "TopoBody.h"
#include "BRepBuilder.h"
#include "RenderBuilder.h"
#include "modules/script/TransHelper.h"
#include "modules/render/Render.h"

#include <polymesh3/Polytope.h>

#include <memory>

namespace
{

void w_BRepTools_poly2shape()
{
    auto poly = ((tt::Proxy<pm3::Polytope>*)ves_toforeign(1))->obj;
    auto shape = brepfw::BRepBuilder::BuildShell(*poly);

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

    auto dev = tt::Render::Instance()->Device();
    auto vao = brepfw::RenderBuilder::BuildVAO(*dev, shape);

    ves_pop(ves_argnum());

    ves_pushnil();
    ves_import_class("render", "VertexArray");
    auto proxy = (tt::Proxy<ur::VertexArray>*)ves_set_newforeign(0, 1, sizeof(tt::Proxy<ur::VertexArray>));
    proxy->obj = vao;
    ves_pop(1);
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
    if (strcmp(signature, "static BRepTools.shape2vao(_)") == 0) return w_BRepTools_shape2vao;

	return nullptr;
}

void BrepFWBindClass(const char* class_name, VesselForeignClassMethods* methods)
{
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