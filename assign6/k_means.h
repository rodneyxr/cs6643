/*
 * Common definitions for Parallel Computing course,, 
 * Assignment: K-Means Algorithm (MPI)
 *
 * To students: You may modify this file
 * 
 * Author:
 *     Wei Wang <wei.wang@utsa.edu>
 */

#ifndef __CS4823_ASSIGNMENT_4__
#define __CS4823_ASSIGNMENT_4__

#define MAX_POINTS 32768
#define MAX_CENTERS 16

#include <mpi.h>

/*
 * data structure for holding a data point
 */
struct point{
	double x;
	double y;
};

/*
 * read data points from input file
 */
int read_points_from_file(char *data_file, struct point *pts, int *m);

/*
 * find K_means 
 */
void k_means(struct point p[], int m, int k, int iters, struct point u[], int c[], int proc_cnt, int proc_id);

/*
 * return a (faked) random point
 */
struct point random_center(struct point p[]);

MPI_Datatype create_mpi_point_type();

#endif
