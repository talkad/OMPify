#include "OMP-solution.h"
#include "Structures.h"
#include "Verify.h"
#include <cassert>


void CheckDuplicatesUsingOMP(const std::vector<CadPt2ID> &points, int gridSize)
{
	// auto emptyPair = std::make_pair(0, 0);
	std::vector<std::pair<int, int>> duplicates(points.size(), emptyPair);

#pragma omp parallel for
	for (int i = 0; i < (int)points.size(); ++i)
	{
		auto outerPt = points.at(i);

		for (int j = 0; j < (int)points.size(); ++j)
		{
			auto innerPt = points.at(j);

			if (innerPt.id != outerPt.id && innerPt.pt == outerPt.pt) // coordinates are the same, but id's are different
			{
				duplicates.at(i) = std::make_pair(innerPt.id, outerPt.id);
			}
		}
	}

	// Remove default zeroed entries
	duplicates.erase(std::remove(duplicates.begin(), duplicates.end(), emptyPair), duplicates.end());

	// Ensure first member of pair is less than second
	std::transform(duplicates.begin(), duplicates.end(), duplicates.begin(), &OrderPair);

	// Sort vector to get pairs ordered adjacent to each other
	std::sort(duplicates.begin(), duplicates.end(), &ComparePairs);

	// The vector now contains a,b entries as well as b,a. Ensure uniqueness.
	duplicates.erase(std::unique(duplicates.begin(), duplicates.end()), duplicates.end());

	if (settings.Verify)
		Verify(duplicates, gridSize);
}