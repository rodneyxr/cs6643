/*
 * Common definitions for Parallel Computing course
 * Assignment: K-Means Algorithm (Pthreads)
 *
 * To students: You may need to modify this file
 * 
 * Author:
 *     Wei Wang <wei.wang@utsa.edu>
 */

#ifndef __CS4823_ASSIGNMENT_4__
#define __CS4823_ASSIGNMENT_4__

#define MAX_POINTS 32768
#define MAX_CENTERS 16

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
 * thread function for K_means 
 */
void *k_means(void *parameter);

/*
 * return a (faked) random point
 */
struct point random_center(struct point p[]);

/*
 * TO STUDENTS: Add necessary data structures here
 */


#endif
