
/*
 * Skeleton function for Parallel Computing Class, 
 * Assignment: K-Means Algorithm (CUDA)
 *
 * To students: You should finish the implementation of k_means algorithm.
 *              You should add device functions/kernels to perform k_means on 
 *              GPU. The "k_means" function in this file is just an interface
 *              for passing in basic parameters needed.. You need to add GPU 
 *              kernels and launch them in the "k_means" function.
 *
 *              Note that the "k_means" function has two input parameters for
 *              block count and thread count per block. Please use these two
 *              parameters when launching your kernels.
 * 
 * Author:
 *     Wei Wang <wei.wang@utsa.edu>
 */
#include <stdio.h>
#include <float.h>
#include <math.h>
#include "k_means.h"

struct params {
	int work_m;
	int work_k;
	int k;
	int m;
};

__device__ static int counter;

__device__ struct point random_center(struct point *p, int m) {
	int idx = (counter++) % m;
	return p[idx];
}

__global__ void assign_points_to_clusters(struct point *p, struct point *u, int *c, struct params* params) {
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int k = params->k;
	int m = params->m;
	int p_start = index * params->work_m;
	int p_end = p_start + params->work_m;
	if (p_end >= m)
		p_end = m;

	int c_point;
	int c_cluster;

	/* find the nearest center to each point */
	for (c_point = p_start; c_point < p_end; c_point++)
	{
		double min_dist = DBL_MAX;
		struct point p1 = p[c_point];

		/* find the cluster that the point belongs to */
		for (c_cluster = 0; c_cluster < k; c_cluster++)
		{
			double dx = p1.x - u[c_cluster].x;
			double dy = p1.y - u[c_cluster].y;
			double dist = dx * dx + dy * dy;
			if (dist < min_dist)
			{
				/* Set the new minimum distance and assign the point to the current cluster */
				min_dist = dist;
				c[c_point] = c_cluster;
			}
		}
	}
	return;
}

__global__ void update_centers(struct point *p, struct point *u, int* c, struct params *params) {
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int k = params->k;
	int m = params->m;
	int k_start = index * params->work_k;
	int k_end = k_start + params->work_k;
	if (k_end >= k)
		k_end = k;

	int c_cluster;
	int c_point;

	/* update the center for each cluster */
	for (c_cluster = k_start; c_cluster < k_end; c_cluster++)
	{
		double sumx = 0;
		double sumy = 0;
		int cluster_size = 0;

		for (c_point = 0; c_point < m; c_point++)
		{
			if (c[c_point] == c_cluster)
			{
				sumx += p[c_point].x;
				sumy += p[c_point].y;
				cluster_size += 1;
			}
		}

		if (cluster_size > 0)
		{
			u[c_cluster].x = sumx / cluster_size;
			u[c_cluster].y = sumy / cluster_size;
		}
		else
		{
			u[c_cluster] = random_center(p, m);
		}
	}
	return;
}

/*
 * k_means: k_means clustering algorithm implementation.
 *
 * Input parameters:
 *     struct point p[]: array of data points
 *     int m           : number of data points in p[]
 *     int k           : number of clusters to find
 *     int iters       : number of clustering iterations to run
 *     int block_cnt   : number of blocks to use
 *     int threads_per_block: number of threads per block
 *
 * Output parameters:   
 *     struct point u[]: array of cluster centers
 *     int c[]         : cluster id for each data points
 */
void k_means(struct point p[MAX_POINTS],
	     int m,
	     int k,
	     int iters,
	     struct point u[MAX_CENTERS],
	     int c[MAX_POINTS],
	     int block_cnt,
	     int threads_per_block)
{
	int j; /* counter for random generator loop */
	int c_iter;

	struct point 	*d_p;		/* device points; size(m)*/
	struct point 	*d_u; 		/* device cluster centers; size(k) */
	int 			*d_c;		/* cluster id for each point; size(m) */
	struct params 	*d_params;	/* parameters to tell how much work to do */
	size_t size_p = sizeof(struct point) * m;
	size_t size_u = sizeof(struct point) * k;
	size_t size_c = sizeof(int) * m;
	size_t size_params = sizeof(struct params);

	/* randomly initialized the centers */
	for(j = 0; j < k; j++)
		u[j] = random_center(p);

	/* Allocate space for device copies */
	cudaMalloc((void **)&d_p, size_p);
	cudaMalloc((void **)&d_u, size_u);
	cudaMalloc((void **)&d_c, size_c);
	cudaMalloc((void **)&d_params, size_params);

	struct params params;
	params.work_m = ceil((float)m / (block_cnt * threads_per_block));
	params.work_k = ceil((float)k / (block_cnt * threads_per_block));
	params.k = k;
	params.m = m;

	/* Copy input from host to device */
	cudaMemcpy(d_p, p, size_p, cudaMemcpyHostToDevice);
	cudaMemcpy(d_u, u, size_u, cudaMemcpyHostToDevice);
	cudaMemcpy(d_c, c, size_c, cudaMemcpyHostToDevice);
	cudaMemcpy(d_params, &params, size_params, cudaMemcpyHostToDevice);

	for (c_iter = 0; c_iter < iters; c_iter++) {
		/* block_cnt and threads_per_block */
		assign_points_to_clusters<<<block_cnt, threads_per_block>>>(d_p, d_u, d_c, d_params);
		update_centers<<<block_cnt, threads_per_block>>>(d_p, d_u, d_c, d_params);
	}

	/* Copy results from device to host */
	cudaMemcpy(u, d_u, size_u, cudaMemcpyDeviceToHost);
	cudaMemcpy(c, d_c, size_c, cudaMemcpyDeviceToHost);

	/* Cleanup */
	cudaFree(d_p);
	cudaFree(d_u);
	cudaFree(d_c);
	cudaFree(d_params);

  	return;
}

