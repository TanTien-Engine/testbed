#include "BRepTrans.h"

#include <polymesh3/Polytope.h>

namespace brepgraph
{

void BRepTrans::BRepToByteArray(const pm3::Polytope& brep, uint8_t** data, uint32_t& length)
{
	auto& points = brep.Points();
	auto& faces  = brep.Faces();

	size_t sz = 0;
	sz += sizeof(uint32_t);
	sz += points.size() * 3 * sizeof(float);
	sz += sizeof(uint32_t) + sizeof(uint32_t) * faces.size();
	for (auto& f : faces) {
		sz += f->border.size() * sizeof(uint32_t);
	}

	length = static_cast<uint32_t>(sz);
	*data = new uint8_t[length];
	uint8_t* ptr = *data;

	const uint32_t p_num = static_cast<uint32_t>(points.size());
	memcpy(ptr, &p_num, sizeof(uint32_t));
	ptr += sizeof(uint32_t);
	for (auto& p : points)
	{
		for (int i = 0; i < 3; ++i)
		{
			const float coords = p->pos.xyz[i];
			memcpy(ptr, &coords, sizeof(uint32_t));
			ptr += sizeof(float);
		}
	}

	const uint32_t f_num = static_cast<uint32_t>(faces.size());
	memcpy(ptr, &f_num, sizeof(uint32_t));
	ptr += sizeof(uint32_t);
	for (auto& f : faces)
	{
		const uint32_t v_num = static_cast<uint32_t>(f->border.size());
		memcpy(ptr, &v_num, sizeof(uint32_t));
		ptr += sizeof(uint32_t);
		for (auto& v : f->border)
		{
			const uint32_t idx = static_cast<uint32_t>(v);
			memcpy(ptr, &idx, sizeof(uint32_t));
			ptr += sizeof(uint32_t);
		}
	}
}

pm3::PolytopePtr BRepTrans::BRepFromByteArray(const uint8_t* data)
{
	auto ptr = data;

	std::vector<pm3::Polytope::PointPtr> points;

	uint32_t p_num;
	memcpy(&p_num, ptr, sizeof(uint32_t));
	ptr += sizeof(uint32_t);
	points.reserve(p_num);
	for (uint32_t i = 0; i < p_num; ++i)
	{
		sm::vec3 pos;
		for (int j = 0; j < 3; ++j)
		{
			memcpy(&pos.xyz[j], ptr, sizeof(float));
			ptr += sizeof(float);
		}
		points.push_back(std::make_shared<pm3::Polytope::Point>(pos));
	}

	std::vector<pm3::Polytope::FacePtr> faces;

	uint32_t f_num;
	memcpy(&f_num, ptr, sizeof(uint32_t));
	ptr += sizeof(uint32_t);
	faces.reserve(f_num);
	for (uint32_t i = 0; i < f_num; ++i)
	{
		auto face = std::make_shared<pm3::Polytope::Face>();

		uint32_t v_num;
		memcpy(&v_num, ptr, sizeof(uint32_t));
		ptr += sizeof(uint32_t);
		face->border.reserve(v_num);

		for (uint32_t j = 0; j < v_num; ++j)
		{
			uint32_t idx;
			memcpy(&idx, ptr, sizeof(uint32_t));
			ptr += sizeof(uint32_t);
			face->border.push_back(idx);
		}

		faces.push_back(face);
	}

	return std::make_shared<pm3::Polytope>(points, faces);
}

}
