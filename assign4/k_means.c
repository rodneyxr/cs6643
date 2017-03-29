/*
 * Skeleton function for Parallel Computing course.
 * Assignment: K-Means Algorithm (Pthread)
 *
 * To students: You should finish the implementation of k_means function
 * 
 * Author:
 *     Wei Wang <wei.wang@utsa.edu>
 */
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <pthread.h>

#include "k_means.h"

/* Global variables that hold the centers, data points and assignments */
extern struct point p[MAX_POINTS]; /* array that holds data points */
extern struct point u[MAX_CENTERS]; /* array that holds the centers */
extern int c[MAX_POINTS]; /* cluster id for each point */

/*
 * k_means: thread function that implements k_means clustering algorithm
 *
 * Input parameters:
 *     TO STUDENTS: Although the arrays of centers, data points and "points'
 *                  cluster ids" are made global variables, you still
 *                  need to pass in additional variables, such as number of
 *                  clusters, number of data points, and other variables to
 *                  help your decomposition.
 * Output parameters:
 *     TO STUDENTS: You should work on the global variables of centers and 
 *                  "points; cluster ids." They are visible in main thread as
 *                  well.
 * Return values:   
 *     NONE; main thread will not check return values.
 */
void *k_means(void *parameter)
{
	/* TO STUDENTS: centers random initialization is moved to main func 
	 * Please implement you k_mean here. Make sure you set the final
	 * results (centers and cluster ids) in the above global arrays of
	 * "u" and "c." Data points are passed through the global array "p."
	 */

	return NULL;
}
