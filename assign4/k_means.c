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
	struct k_params *params = (struct k_params*) parameter;
	int m_end = params->m_start + params->m_decomp; /* last point index assigned */
	int k_end = params->k_start + params->k_decomp; /* last cluster index assigned */
	int c_iter;
	int c_cluster;
	int c_point;

	for (c_iter = 0; c_iter < params->iters; c_iter++)
	{
        /* find the nearest center to each point */
        for (c_point = params->m_start; c_point < m_end; c_point++)
        {
            double min_dist = DBL_MAX;
            struct point p1 = p[c_point];

            /* find the cluster that the point belongs to */
            for (c_cluster = 0; c_cluster < params->k; c_cluster++)
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

		/* sync all threads */
        pthread_barrier_wait(params->barrier);

        /* update the center for each cluster */
        for (c_cluster = params->k_start; c_cluster < k_end; c_cluster++)
        {
            double sumx = 0;
            double sumy = 0;
            int cluster_size = 0;

            for (c_point = 0; c_point < params->m; c_point++)
            {
                if (c[c_point] == c_cluster)
                {
                    sumx += p[c_point].x;
                    sumy += p[c_point].y;
                    cluster_size++;
                }
            }

            if (cluster_size > 0)
            {
                u[c_cluster].x = sumx / cluster_size;
                u[c_cluster].y = sumy / cluster_size;
            }
            else
            {
                u[c_cluster] = random_center(p);
            }
        }

        /* sync all threads */
        pthread_barrier_wait(params->barrier);
    }

	return NULL;
}
