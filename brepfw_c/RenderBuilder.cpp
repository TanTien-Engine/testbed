#include "RenderBuilder.h"
#include "BRepExplore.h"

#include <SM_Calc.h>
#include <SM_Triangulation.h>
#include <unirender/Device.h>
#include <unirender/VertexBuffer.h>
#include <unirender/VertexArray.h>
#include <unirender/IndexBuffer.h>
#include <unirender/ComponentDataType.h>
#include <unirender/VertexInputAttribute.h>

#include <map>

namespace brepfw
{

std::shared_ptr<ur::VertexArray> RenderBuilder::
BuildVAO(const ur::Device& dev, const std::shared_ptr<TopoShape>& shape, const sm::vec3& color)
{
	std::vector<sm::vec3> points;
	std::vector<std::vector<uint32_t>> faces;
	BRepExplore::Dump(shape, points, faces);

	std::vector<float> buf;
	buf.reserve(points.size() * (3 + 3));
	for (auto& p : points) 
    {
		for (int i = 0; i < 3; ++i) {
			buf.push_back(p[i]);
		}
        for (int i = 0; i < 3; ++i) {
            buf.push_back(color[i]);
        }
	}

    std::vector<unsigned short> indices;
    for (auto& f : faces) 
    {

        if (f.size() == 3)
        {
            for (auto idx : f) {
                indices.push_back(idx);
            }
        }
        else
        {
            auto tris_idx = Triangulation(points, f, {});
            for (auto& idx : tris_idx) {
                indices.push_back(idx);
            }
        }
    }

    auto va = dev.CreateVertexArray();

    int vbuf_sz = sizeof(float) * static_cast<int>(buf.size());
    auto vbuf = dev.CreateVertexBuffer(ur::BufferUsageHint::StaticDraw, vbuf_sz);
    vbuf->ReadFromMemory(buf.data(), vbuf_sz, 0);
    va->SetVertexBuffer(vbuf);

    auto ibuf = dev.CreateIndexBuffer(ur::BufferUsageHint::StaticDraw, 0);
    int ibuf_sz = sizeof(unsigned short) * static_cast<int>(indices.size());
    ibuf->SetCount(static_cast<int>(indices.size()));
    ibuf->Reserve(ibuf_sz);
    ibuf->ReadFromMemory(indices.data(), ibuf_sz, 0);
    ibuf->SetDataType(ur::IndexBufferDataType::UnsignedShort);
    va->SetIndexBuffer(ibuf);

    std::vector<std::shared_ptr<ur::VertexInputAttribute>> vbuf_attrs = {
        // pos
        std::make_shared<ur::VertexInputAttribute>(0, ur::ComponentDataType::Float, 3, 0, 24),
        // color
        std::make_shared<ur::VertexInputAttribute>(1, ur::ComponentDataType::Float, 3, 12, 24)
    };
    va->SetVertexBufferAttrs(vbuf_attrs);

    return va;
}

std::vector<uint32_t>
RenderBuilder::Triangulation(const std::vector<sm::vec3>& verts, const std::vector<uint32_t>& border,
                             const std::vector<std::vector<uint32_t>>& holes)
{
    std::vector<sm::vec3> border3;
    border3.reserve(border.size());
    for (auto& idx : border) {
        border3.push_back(verts[idx]);
    }
    auto norm = sm::calc_face_normal(border3);
    auto rot = sm::mat4(sm::Quaternion::CreateFromVectors(norm, sm::vec3(0, 1, 0)));

    std::map<sm::vec2, uint32_t> pos2idx;
    auto trans_loop3to2 = [&](const std::vector<sm::vec3>& verts,
        const std::vector<uint32_t>& loop3) -> std::vector<sm::vec2>
    {
        std::vector<sm::vec2> loop2;
        loop2.reserve(loop3.size());
        for (auto& idx : loop3)
        {
            auto& pos3 = verts[idx];
            auto p3_rot = rot * pos3;
            sm::vec2 pos2(p3_rot.x, p3_rot.z);
            auto status = pos2idx.insert({ pos2, idx });
//            assert(status.second);

            loop2.push_back(pos2);
        }
        return loop2;
    };

    auto border2 = trans_loop3to2(verts, border);
    if (border2.empty() || border2.front().x != border2.front().x) {
        return std::vector<uint32_t>();
    }

    std::vector<std::vector<sm::vec2>> holes2;
    holes2.resize(holes.size());
    for (size_t i = 0, n = holes.size(); i < n; ++i) {
        holes2[i] = trans_loop3to2(verts, holes[i]);
    }

    std::vector<uint32_t> ret;

    std::vector<sm::vec2> tris;
    try {
        sm::triangulate_holes(border2, holes2, tris);
    } catch (...) {
        return {};
    }
    assert(tris.size() % 3 == 0);
    ret.reserve(tris.size());
    for (size_t i = 0, n = tris.size(); i < n; )
    {
        std::vector<sm::vec2*> tri(3);
        for (size_t j = 0; j < 3; ++j) {
            tri[j] = &tris[i++];
        }
        if (sm::is_turn_left(*tri[0], *tri[1], *tri[2])) {
            std::reverse(tri.begin(), tri.end());
        }
        for (size_t j = 0; j < 3; ++j)
        {
            auto itr = pos2idx.find(*tri[j]);
            if (itr == pos2idx.end())
            {
                float min_dist = FLT_MAX;
                int min_idx = -1;
                for (auto itr = pos2idx.begin(); itr != pos2idx.end(); ++itr) 
                {
                    float d = sm::dis_pos_to_pos(*tri[j], itr->first);
                    if (d < min_dist) {
                        min_dist = d;
                        min_idx = itr->second;
                    }
                }
                assert(min_idx >= 0);
                ret.push_back(static_cast<size_t>(min_idx));
            }
            else
            {
                ret.push_back(itr->second);
            }
        }
    }

    return ret;
}

}