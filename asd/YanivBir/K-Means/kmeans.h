#pragma once
#include "Point.h"

int kmeans(
	Point *points,		// The Points list
	int n,				// Number of points
	int k,				// Number of clusters
	int limit,			// Maximum iteration
	double time,			// The time
	double qm,			//The QM
	char *output		//output buffer
);

void printCurrentTime(double startTime, int status);