#include "OMP-solution.h"
#include "Structures.h"
#include "Verify.h"


void RotateUsingOMPEx(const std::vector<CadPt3> &points, float radians)
{
	auto rotation = CadMatrix3::MakeRotationZ(radians);

	int size = static_cast<int>(points.size());

#pragma omp parallel for
	for (int i = 0; i < size; ++i)
	{
		CadPt3 p = points[i];
		CadPt3 r = rotation.Multiply(p);

		if (settings.Verify)
			Verify(p, radians, r);
	}
}

void RotateUsingOMP(const std::vector<CadPt3> &points, float step)
{

	for (float d = step; d <= 360.f; d += step){
		RotateUsingOMPEx(points, 10.0f);
	}

}



//error C3016 : 'i' : index variable in OpenMP 'for' statement must have signed integral type