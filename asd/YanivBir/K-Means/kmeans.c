#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <Kmeans.h>
#define BIG_double (INFINITY)

cudaError_t cuda_findDiameter(Point *clusterPoints, int clusterCount, double *diamter);
cudaError_t cuda_calcPointsDistance(Point *points, int n, Point *centers, int k, double *distance_output);
cudaError_t cuda_setCurrentPosition(Point *points, int n, double time);

//Calc distance between all of the point to the center of a cluster
void calcPointsDistances(Point *points, int n, Point *centers, int k, double time, double *distance_output) {
	cudaError_t cudaStatus;
	cudaStatus = cuda_calcPointsDistance(points, n, centers, k, distance_output);
	if (cudaStatus != cudaSuccess) {
		printf("calcPointsDistances time=%lf return error.\n", time);
		fflush(stdout);
	}
}

//choose for evry point the sutiable cluster
void choosePointCluster(double *distance_array, int n, int *PointAssignCluster, int k){
	int i, j;
	double closest_distance;
#pragma omp parallel for private(j, closest_distance)
	for (i = 0; i < n; i++){ // for each point
		int best_index = -1;
		closest_distance = BIG_double;

		for (j = 0; j < k; j++){	// for each cluster
			double cur_distance = distance_array[i*k + j]; //distance between point and cluster center

			if (cur_distance < closest_distance){
				best_index = j;
				closest_distance = cur_distance;
			}
		}
		PointAssignCluster[i] = best_index; // record in array
	}
}

//Calc the new centers of a clusters
void calcClusterCenter(Point *points, int *PointAssignCluster, int n, Point *clustersCenters, int k, double time) {
	omp_lock_t lock;
	omp_init_lock(&lock);
	Point *oldCenters = (Point *)malloc(sizeof(Point) * k);
	int *cluster_member_count = (int *)malloc(sizeof(int)* k);
	
	if ((oldCenters == NULL) || (cluster_member_count == NULL))
	{
		printf("can't malloc at calcClusterCenter()\n");
		MPI_Abort(MPI_COMM_WORLD, 2);
	}
	// initialize cluster center coordinate sums to zero
	int i;
#pragma omp parallel for
	for (i = 0; i < k; i++) { //for each cluster
		cluster_member_count[i] = 0;
		oldCenters[i].x = clustersCenters[i].x;
		oldCenters[i].y = clustersCenters[i].y;
		oldCenters[i].z = clustersCenters[i].z;
		clustersCenters[i].x = 0;
		clustersCenters[i].y = 0;
		clustersCenters[i].z = 0;
	}
	// sum all points 
	int j;
#pragma omp parallel for
	for (j = 0; j < n; j++) {	 //for every point
		int active_cluster;
		active_cluster = PointAssignCluster[j]; // which cluster is it in?
		omp_set_lock(&lock);
		cluster_member_count[active_cluster]++; //update count of members in that cluster
		clustersCenters[active_cluster].x += points[j].x;
		clustersCenters[active_cluster].y += points[j].y;
		clustersCenters[active_cluster].z += points[j].z;
		omp_unset_lock(&lock);
	}

	// now divide each coordinate sum by number of members to find average center	
#pragma omp parallel for
	for (i = 0; i < k; i++){ // for each cluster
		if (cluster_member_count[i] == 0) {
			printf("WARNING: Empty cluster %d! \n", i);
			clustersCenters[i].x = oldCenters[i].x;
			clustersCenters[i].y = oldCenters[i].y;
			clustersCenters[i].z = oldCenters[i].z;
		}
		else {
			clustersCenters[i].x /= cluster_member_count[i];
			clustersCenters[i].y /= cluster_member_count[i];
			clustersCenters[i].z /= cluster_member_count[i];
		}
	}
	free(oldCenters);
	free(cluster_member_count);
	omp_destroy_lock(&lock);
}

//count how many points a cluster contain
void getClusterMemberCount(int n, int k, int *cluster_assignment_index, int *cluster_member_count) {
	omp_lock_t lock;
	omp_init_lock(&lock);

	int i;
#pragma omp parallel for
	for (i = 0; i < k; i++) // initialize cluster member counts
	{
		cluster_member_count[i] = 0;
	}

	// count members of each cluster  
	int j;
#pragma omp parallel for
	for (j = 0; j < n; j++) { //for each point
		omp_set_lock(&lock);
		cluster_member_count[cluster_assignment_index[j]]++;
		omp_unset_lock(&lock);
	}
	omp_destroy_lock(&lock);
}

//copy src array to tgt array
void copyAssignArr(int n, int *src, int *tgt) {
	int i;
#pragma omp parallel for
	for (i = 0; i < n; i++)
		tgt[i] = src[i];
}

//count change of clusters centers
int countChanges(int n, int a[], int b[]) {
	int change_count = 0;
	int i;
#pragma omp parallel for reduction(+:change_count)
	for (i = 0; i < n; i++)
		if (a[i] != b[i]) {
			change_count++;
		}
	return change_count;
}

//Printf How many points is in the cluster
void printToBufclusterCenterInfo(int *PointAssignCluster, int n, Point *cluster_center, int k, char *output) {
	int* cluster_member_count = (int *)malloc(sizeof(int)* k);
	if (cluster_member_count == NULL) {
		printf("can't malloc cluster_member_count at printToBufclusterCenterInfo()\n");
		MPI_Abort(MPI_COMM_WORLD, 2);
	}
	getClusterMemberCount(n, k, PointAssignCluster, cluster_member_count);
	sprintf(output + strlen(output), "Centers of clusters:\n");

	int i;
	for (i = 0; i < k; i++){	
		//sprintf(output + strlen(output), "%.6f	%.6f	%.6f count: %d \n", cluster_center[i].x, cluster_center[i].y, cluster_center[i].z, cluster_member_count[i]);
		sprintf(output + strlen(output), "%lf	%lf	%lf \n", cluster_center[i].x, cluster_center[i].y, cluster_center[i].z);	
	}
	free(cluster_member_count);
}

//Calc distance between 2 centers
double calcCentersDist(Point *centers, int center1, int center2)
{
	if (center1 != center2) {
		double x, y, z;
		x = centers[center1].x - centers[center2].x;
		y = centers[center1].y - centers[center2].y;
		z = centers[center1].z - centers[center2].z;
		x *= x;
		y *= y;
		z *= z;
		return sqrt(x + y + z);
	}
	return 0;
}

//Calc cluster diameter by cuda
double calcClusterDiameter(Point *clusterPoints, int clusterCount, int clusterId, double time) {
	double diameter;
	cudaError_t cudaStatus;
	cudaStatus = cuda_findDiameter(clusterPoints, clusterCount, &diameter);
	if (cudaStatus != cudaSuccess) {
		printf("calcClusterDiameter time=%lf return error.\n", time);
		fflush(stdout);
		return -1;
	}
	return diameter;
}

//get all the points of clusterIndex
Point * getClusterPoints(Point *points, int *PointAssignCluster, int n, int clusterIndex, int clusterPointsSize) {
	Point *clusterPoints = (Point *)malloc(sizeof(Point) * clusterPointsSize + 1);
	if (clusterPoints == NULL) {
		printf("clusterPoint malloc error\n");
		MPI_Abort(MPI_COMM_WORLD, 2);
	}

	omp_lock_t lock;
	omp_init_lock(&lock);

	int j = 0; //index for clusterPoints array
	int index;
#pragma omp parallel for shared(j)
	for (index = 0; index < n; index++) {
		if (PointAssignCluster[index] == clusterIndex) {
			if (j < clusterPointsSize) {
				omp_set_lock(&lock);
				clusterPoints[j].x = points[index].x;
				clusterPoints[j].y = points[index].y;
				clusterPoints[j].z = points[index].z;
				clusterPoints[j].vX = points[index].vX;
				clusterPoints[j].vY = points[index].vY;
				clusterPoints[j].vZ = points[index].vZ;
				j++;
				omp_unset_lock(&lock);
			}
		}
	}
	omp_destroy_lock(&lock);
	return clusterPoints;
}

//Calc the cluster quality
double calcClustersQuality(Point *points, int *PointAssignCluster, int n, Point *centers, int k, double currentTime) {
	double q = 0;
	int *cluster_member_count = (int *) malloc(sizeof(int)* k);
	if (cluster_member_count == NULL) 
	{
		printf("cluster_member_count malloc error\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	getClusterMemberCount(n, k, PointAssignCluster, cluster_member_count);

	int i, j;
#pragma omp parallel for private(j) reduction(+:q)
	for (i = 0; i < k; i++) { //for each cluster
		if (cluster_member_count[i] != 0) 
		{
			int currentClusterCount = cluster_member_count[i];
			
			Point *clusterPoints;
			clusterPoints = (Point *) getClusterPoints(points, PointAssignCluster, n, i, currentClusterCount); 
			
			double ClusterDiameter = calcClusterDiameter(clusterPoints, cluster_member_count[i], i, currentTime);//CUDA
			free(clusterPoints);

			if (ClusterDiameter != -1) 
			{
				for (j = 0; j < k; j++) 
				{
					if (i != j) 
					{
						double distance = calcCentersDist(centers, i, j);
						q += ClusterDiameter / distance;
					}
				}
			}
		}
	}
	q /= k*(k - 1); 
	
	free(cluster_member_count);
	return q;
}

//Print currnet time
void printCurrentTime(double startTime ,int status) {
	double t1 = MPI_Wtime();
	printf("statusTime:%d time is:  : %.6f\n", status, t1 - startTime);
	fflush(stdout);
}

//copy points array
void copyPointsArray(Point* src, Point* dst, int n) {
	int i;
#pragma omp parallel for 
	for (i = 0; i < n; i++) {
		dst[i].x = src[i].x;
		dst[i].y = src[i].y;
		dst[i].z = src[i].z;
		dst[i].vX = src[i].vX;
		dst[i].vY = src[i].vY;
		dst[i].vZ = src[i].vZ;
	}
}

//init the cluster points to be as the first points in the arry points
void initClusterCenters(Point *points, Point *clustersCenters, int k) {
	int i;
#pragma omp parallel for 
	for (i = 0; i < k; i++) {
		clustersCenters[i].x = points[i].x;
		clustersCenters[i].y = points[i].y;
		clustersCenters[i].z = points[i].z;
		clustersCenters[i].vX = 0;
		clustersCenters[i].vY = 0;
		clustersCenters[i].vZ = 0;
	}
}

//This function return 1 q<QM else return 0
int kmeans(Point *points, int n, int k, int limit, double currentTime, double qm, char *output) {
	Point *currPoints = (Point *) malloc(sizeof(Point)*n);
	double *pointsDist = (double *) malloc(sizeof(double)*n *k);
	int *PointAssignCluster_cur = (int *) malloc(sizeof(int)*n);
	int *PointAssignCluster_prev = (int *) malloc(sizeof(int)*n);
	if (!pointsDist || !PointAssignCluster_cur || !PointAssignCluster_prev || !currPoints) {
		printf("Error allocating pointsDist arrays\n");
		MPI_Abort(MPI_COMM_WORLD, 0);
	}

	Point *clustersCenters = (Point *) malloc(sizeof(Point) * k);
	if (clustersCenters == NULL) {
		printf("Can't malloc clustersCenters kmeanMainResult()\n");
		MPI_Abort(MPI_COMM_WORLD, 0);
	}

	copyPointsArray(points, currPoints, n);
	cuda_setCurrentPosition(currPoints, n, currentTime);
	initClusterCenters(currPoints, clustersCenters, k);

	calcPointsDistances(currPoints, n, clustersCenters, k, currentTime, pointsDist); 
	choosePointCluster(pointsDist, n, PointAssignCluster_cur, k); 

	int iteration = 0;
	int change_count = -1;
	while ((iteration < limit) && (change_count != 0)) {			
		calcClusterCenter(currPoints, PointAssignCluster_cur, n, clustersCenters, k, currentTime);
		copyAssignArr(n, PointAssignCluster_cur, PointAssignCluster_prev); 
		calcPointsDistances(currPoints, n, clustersCenters, k, currentTime, pointsDist);
		choosePointCluster(pointsDist, n, PointAssignCluster_cur, k); 

		iteration++;
		change_count = countChanges(n, PointAssignCluster_cur, PointAssignCluster_prev); 
	}
	double q;
	q = calcClustersQuality(currPoints, PointAssignCluster_cur, n, clustersCenters, k, currentTime); 
	if (q < qm) {
		sprintf(output + strlen(output), "First occurrence t =%.3f with q=%lf \n", currentTime, q);
		printToBufclusterCenterInfo(PointAssignCluster_cur, n, clustersCenters, k, output);
	}
	else
	{
		sprintf(output + strlen(output), "End of t=%.3f q=%lf\n", currentTime, q);
		//printToBufclusterCenterInfo(PointAssignCluster_cur, n, clustersCenters, k, output);
	}
	free(clustersCenters);
	free(pointsDist);
	free(PointAssignCluster_cur);
	free(PointAssignCluster_prev);
	free(currPoints);

	if (q < qm)
		return 1;
	return 0;
}

