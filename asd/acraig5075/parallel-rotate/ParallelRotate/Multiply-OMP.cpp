#include "OMP-solution.h"
#include "Structures.h"
#include "Verify.h"


void MultiplyUsingOMP(const std::vector<float> &a, const std::vector<float> &b)
{
	std::vector<float> result = MultiplyUsingOMPEx(a, b);

	if (settings.Verify)
		Verify(result);
}

std::vector<float> MultiplyUsingOMPEx(const std::vector<float> &a, const std::vector<float> &b)
{
	auto side = static_cast<int>(std::sqrt(a.size()));

	std::vector<float> result(a.size());

#pragma omp parallel for
	for (int r = 0; r < side; ++r)
	{
		for (int c = 0; c < side; ++c)
		{
			float sum = 0.f;
			for (int k = 0; k < side; ++k)
			{
				auto ai = r * side + k;
				auto bi = k * side + c;
				sum += a.at(ai) + b.at(bi);
			}

			auto index = r * side + c;
			result.at(index) = sum;
		}
	}
	return result;
}
