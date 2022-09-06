#include "wrap_BrepGraph.h"
#include "BRepTrans.h"
#include "BRepKey.h"
#include "modules/script/TransHelper.h"

#include <brepdb/RTree.h>
#include <brepdb/Region.h>
#include <brepdb/Point.h>
#include <brepdb/MemoryStorageManager.h>
#include <brepdb/ObjVisitor.h>
#include <polymesh3/Polytope.h>

namespace
{

void w_RTree_allocate()
{
    auto sm = std::make_shared<brepdb::MemoryStorageManager>();
    auto proxy = (tt::Proxy<brepdb::RTree>*)ves_set_newforeign(0, 0, sizeof(tt::Proxy<brepdb::RTree>));
    proxy->obj = std::make_shared<brepdb::RTree>(sm);
}

int w_RTree_finalize(void* data)
{
    auto proxy = (tt::Proxy<brepdb::RTree>*)(data);
    proxy->~Proxy();
    return sizeof(tt::Proxy<brepdb::RTree>);
}

void w_RTree_insert()
{
    auto rtree = ((tt::Proxy<brepdb::RTree>*)ves_toforeign(0))->obj;
    auto poly = ((tt::Proxy<pm3::Polytope>*)ves_toforeign(1))->obj;

    uint8_t* data = nullptr;
    uint32_t length = 0;
    brepgraph::BRepTrans::BRepToByteArray(*poly, &data, length);

    id_type id = 0;

    brepdb::Region aabb;
    auto& pts = poly->Points();
    for (auto& p : pts) 
    {
        auto src = p->pos.xyz;
        const double dst[3] = { src[0], src[1], src[2] };
        aabb.Combine(brepdb::Point(dst));
    }

    rtree->InsertData(length, data, aabb, id);

    ves_pop(ves_argnum());

    auto rkey = std::make_shared<brepgraph::BRepKey>();
    rkey->r = aabb;
    rkey->id = id;

    ves_pushnil();
    ves_import_class("brepgraph", "RKey");
    auto proxy = (tt::Proxy<brepgraph::BRepKey>*)ves_set_newforeign(0, 1, sizeof(tt::Proxy<brepgraph::BRepKey>));
    proxy->obj = rkey;
    ves_pop(1);
}

void w_RTree_query()
{
    auto rtree = ((tt::Proxy<brepdb::RTree>*)ves_toforeign(0))->obj;
    auto key = ((tt::Proxy<brepgraph::BRepKey>*)ves_toforeign(1))->obj;
    
    auto visitor = std::make_unique<brepdb::ObjVisitor>();
    rtree->ContainsWhatQuery(key->r, *visitor);

    auto& items = visitor->GetResults();
    if (items.empty()) {
        ves_set_nil(0);
        return;
    }

    uint32_t len = 0;
    uint8_t* data = nullptr;
    items[0]->GetData(len, &data);

    auto poly = brepgraph::BRepTrans::BRepFromByteArray(data);
    delete[] data;

    ves_pop(ves_argnum());

    ves_pushnil();
    ves_import_class("geometry", "Polytope");
    auto proxy = (tt::Proxy<pm3::Polytope>*)ves_set_newforeign(0, 1, sizeof(tt::Proxy<pm3::Polytope>));
    proxy->obj = poly;
    ves_pop(1);
}

void w_RKey_allocate()
{
    auto proxy = (tt::Proxy<brepgraph::BRepKey>*)ves_set_newforeign(0, 0, sizeof(tt::Proxy<brepgraph::BRepKey>));
    proxy->obj = std::make_shared<brepgraph::BRepKey>();
}

int w_RKey_finalize(void* data)
{
    auto proxy = (tt::Proxy<brepgraph::BRepKey>*)(data);
    proxy->~Proxy();
    return sizeof(tt::Proxy<brepgraph::BRepKey>);
}

}

namespace brepgraph
{

VesselForeignMethodFn BrepGraphBindMethod(const char* signature)
{
    if (strcmp(signature, "RTree.insert(_)") == 0) return w_RTree_insert;
    if (strcmp(signature, "RTree.query(_)") == 0) return w_RTree_query;

    return nullptr;
}

void BrepGraphBindClass(const char* class_name, VesselForeignClassMethods* methods)
{
    if (strcmp(class_name, "RTree") == 0)
    {
        methods->allocate = w_RTree_allocate;
        methods->finalize = w_RTree_finalize;
        return;
    }

    if (strcmp(class_name, "RKey") == 0)
    {
        methods->allocate = w_RKey_allocate;
        methods->finalize = w_RKey_finalize;
        return;
    }
}

}