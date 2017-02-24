/*
 * Skeleton function for CS6643 Parallel Processing, 
 * Assignment 3: K-Means Algorithm (OpenMP)
 *
 * To students: You should finish the implementation of k_means function
 * 
 * Author:
 *     Wei Wang <wei.wang@utsa.edu>
 */
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <omp.h> /* OpenMP header */

#include "k_means.h"

double distance(const struct point p, const struct point u)
{
    double dx = p.x - u.x;
    double dy = p.y - u.y;
    return dx * dx + dy * dy;
}

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
    int j; /* counter for random generator loop */
    int c_iter;
    int c_cluster;
    int c_point

    /* randomly initialized the centers */
    /* Note: DO NOT CHANGE THIS RANDOM GENERATOR! */
    /* Note: DO NOT PARALLELIZE THIS LOOP */
    /* Note: THE INTERFACE TO random_center HAS CHANGED */
    for (j = 0; j < k; j++)
        u[j] = random_center(p);

    /* 
	 * To students: please implment K-Means algorithm with OpenMP here
	 * Your K-means implementation should do "iters" rounds of clustering. After 
	 * all iterations finish, array u[MAX_CENTERS] should have the coordinations 
	 * of your centers, and array c[MAX_POINTS] should have the cluster assignment
	 * for each point.
	 */
    for (c_iter = 0; c_iter < iters; c_iter++)
    {

        /* find the nearest center to each point */
        for (c_point = 0; c_point < m; c_point++)
        {
            double min_dist = DBL_MAX;

            /* find the cluster that the point belongs to */
            for (c_cluster = 0; c_cluster < k; c_cluster++)
            {
                double dist = distance(p[c_point], u[c_cluster]);
                if (dist < min_dist)
                {
                    /* Set the new minimum distance and assign the point to the current cluster */
                    min_dist = dist;
                    c[c_point] = c_cluster;
                }
            }
        }

        /* update the center for each cluster */
        for (c_cluster = 0; c_cluster < k; c_cluster++)
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
    }

    return;
}