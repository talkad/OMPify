#include "OMP-solution.h"
#include "Structures.h"
#include "Verify.h"
#include "Serial-solution.h"

// Serial outer loop, and parallel inner point-in-poly test
void PointInPolyOMP(const std::vector<CadPt2> &points, const CadPolygon &polygon, float width, float extent)
{
		// parallelise inner loop, point-in-point test
	for (auto pt : points)
	{
		bool inside = PointInPolyOMPEx(pt, polygon);

		if (settings.Verify)
			Verify(pt, width, extent, inside);
	}
}

bool PointInPolyOMPEx(const CadPt2 &pt, const CadPolygon &polygon)
{
	int inout = 0;

#pragma omp parallel for reduction(+:inout)
	for (int i = 0; i < (int)polygon.size(); ++i)
	{
		CadEdge edge = polygon.at(i);
		auto fst = edge.first;
		auto snd = edge.second;

		if ((Float::IsLessThanOrEqual(fst.y, pt.y) && Float::IsLessThan(pt.y, snd.y)) ||
			(Float::IsLessThanOrEqual(snd.y, pt.y) && Float::IsLessThan(pt.y, fst.y)))
		{
			float tdbl1 = Float::Divide(pt.y - fst.y, snd.y - fst.y);
			float tdbl2 = snd.x - fst.x;
			if (Float::IsGreaterThanOrEqual(fst.x + (tdbl2 * tdbl1), pt.x))
				inout++;
		}
	}

	return (0 == inout ? false : (bool)(0 != inout % 2));
}

// Parallel outer loop, serial inner point-in-poly test
void PointInPolyOMP2(const std::vector<CadPt2> &points, const CadPolygon &polygon, float width, float extent)
{
	// parallelise outer loop, serial point-in-point test
	int size = points.size();

#pragma omp parallel for
	for (int i = 0; i < size; ++i)
	{
		CadPt2 pt = points[i];
		bool inside = PointInPolySeriallyEx(pt, polygon);

		if (settings.Verify)
			Verify(pt, width, extent, inside);
	}
}

