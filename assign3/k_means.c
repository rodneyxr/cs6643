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
// #include <float.h>
#include <math.h>
#include <omp.h> /* OpenMP header */

#include "k_means.h"

double distance(const struct point p, const struct point u)
{
    return sqrt(pow(p.x - u.x, 2) + pow(p.y - u.y, 2));
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
    /* To Students: add your local variables */
    int c_cluster;               /* counter for current cluster */
    int c_iter;                  /* counter for current iteration */
    int c_point;                 /* counter for the current point */
    int j;                       /* counter for random generator loop */
    int cluster_size[k];         /* the size of each cluster */
    // double min_dist;              /* current minimum distance */
    struct point cluster_sum[k]; /* the sum of each point distance in a cluster */

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
        /* initialize the sum and size of each cluster */
        #pragma omp parallel for
        for (c_cluster = 0; c_cluster < k; c_cluster++)
        {
            cluster_size[c_cluster] = 0;
            cluster_sum[c_cluster].x = 0;
            cluster_sum[c_cluster].y = 0;
        }

        /* find the nearest center to each point */
        #pragma omp parallel for
        for (c_point = 0; c_point < m; c_point++)
        {
            /* fence post min_dist */
            double min_dist = distance(p[c_point], u[0]);
            c[c_point] = 0;

            /* find the cluster that the point belongs to */
            for (c_cluster = 1; c_cluster < k; c_cluster++)
            {
                double dist = distance(p[c_point], u[c_cluster]);
                if (dist < min_dist)
                {
                    /* Set the new minimum distance and assign the point to the
                       current cluster */
                    min_dist = dist;
                    c[c_point] = c_cluster;
                }
            }

            /* update some information about the cluster that the point was
               assigned to */
            #pragma omp critical
            {
                int cluster = c[c_point];
                struct point *point = &p[c_point];
                cluster_size[cluster]++;
                cluster_sum[cluster].x += point->x;
                cluster_sum[cluster].y += point->y;
            }
        }

        /* update the center for each cluster */
        for (c_cluster = 0; c_cluster < k; c_cluster++)
        {
            struct point *center = &u[c_cluster];
            struct point *sum = &cluster_sum[c_cluster];
            int size = cluster_size[c_cluster];

            /* set the cluster center or generate a new one if no points */
            if (cluster_size[c_cluster] > 0)
            {
                center->x = sum->x / size;
                center->y = sum->y / size;
            }
            else
            {
                u[c_cluster] = random_center(p);
            }
        }
    }

    return;
}
