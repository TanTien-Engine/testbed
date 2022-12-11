#include "VertexBuffer.h"

#include <polymesh3/Polytope.h>

namespace brepvmgraph
{

VertexBuffer::VertexBuffer(const pm3::Polytope& poly)
{
	Build(poly);
}

void VertexBuffer::Build(const pm3::Polytope& poly)
{
    size_t v_num = 0, i_num = 0;
    for (auto& raw : poly.GetRaws())
    {
        // pos(3) + color(3) + mat(1) + offset(1)
        v_num += raw->points.size() * 8;
        i_num += raw->faces_num.size() * 6;
    }
    m_vbuf.reserve(v_num);
    m_ibuf.reserve(i_num);

	int start_idx = 0;
    sm::cube aabb;
    for (auto& raw : poly.GetRaws())
    {
        for (auto& p : raw->points)
        {
            // pos
            m_vbuf.push_back(p.x);
            m_vbuf.push_back(p.y);
            m_vbuf.push_back(p.z);

            // color
            m_vbuf.push_back(1.0f);
            m_vbuf.push_back(1.0f);
            m_vbuf.push_back(1.0f);

            // mat and offset
            m_vbuf.push_back(0);
            m_vbuf.push_back(0);
        }

        int begin = 0;
        for (auto& num : raw->faces_num)
        {
            int end = begin + num;

            // fixme
            if (num == 4)
            {
                m_ibuf.push_back(start_idx + raw->faces[begin + 0]);
                m_ibuf.push_back(start_idx + raw->faces[begin + 1]);
                m_ibuf.push_back(start_idx + raw->faces[begin + 2]);

                m_ibuf.push_back(start_idx + raw->faces[begin + 0]);
                m_ibuf.push_back(start_idx + raw->faces[begin + 2]);
                m_ibuf.push_back(start_idx + raw->faces[begin + 3]);
            }

            begin += num;
        }

        start_idx += raw->points.size();
    }
}

}