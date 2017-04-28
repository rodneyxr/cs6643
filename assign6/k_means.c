/*
 * Skeleton function for Parallel Computing course, 
 * Assignment: K-Means Algorithm (MPI)
 *
 * To students: You should finish the implementation of k_means function
 * 
 * Author:
 *     Wei Wang <wei.wang@utsa.edu>
 */
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <mpi.h>

#include "k_means.h"


/*
 * k_means: k_means clustering algorithm implementation.
 *
 * Input parameters:
 *     struct point p[]: array of data points
 *     int m           : number of data points in p[]
 *     int k           : number of clusters to find
 *     int iters       : number of clustering iterations to run
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
	    int c[MAX_POINTS])
{
	/* TO STUDENTS: Please implement the MPI k_means here.
	 *	
	 * Note that, this time you will do the random initialization 
	 * yourself. The random function's interface is the same as 
	 * previous assignments. You can direct copy the old code here.
	 * However, you need to make sure that all your processes have
	 * the same initial centers (i.e., only initialized in one 
	 * process and send the initial centers to other processes).
	 */
	

	return;
}
