/*
 * Main function for Parallel Computing course, 
 * Assignment 4: K-Means Algorithm (Pthreads)
 *
 * To students: You SHOULD modify this file
 * 
 * Author:
 *     Wei Wang <wei.wang@utsa.edu>
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <err.h>
#include <pthread.h>

#include "k_means.h"

int verbose = 0;

/* command line arguments */
struct option long_options[] = {
	{"data-file",     required_argument, 0, 'f'}, /* input data file */
	{"clusters",      required_argument, 0, 'k'}, /* cluster count */
	{"iterations",    required_argument, 0, 'i'}, /* iteration count */
	{"threads",       required_argument, 0, 't'}, /* number of threads */
	{"help",          no_argument,       0, 'h'}, /* print help */
	{0, 0, 0, 0}
};

void print_usage()
{
	printf("Usage: ./this_command [-h] [-f DATA_FILE] [-k K] [-i ITERS] "
	       "[-t THREAD_COUNT]\n");
	printf("\n");
	printf("-h, --help \t\t\t show this help message and exit\n");
	printf("-f, --data-file \t\t specify the path and name of input "
	       "file\n");
	printf("-k, --clusters \t\t\t specify the number of clusters to find;"
	       "default 2 clusters\n");
	printf("-t, --threads \t\t\t specify the number of threads to use;"
	       "default 1 thread\n");
	printf("-i, --iterations \t\t specify the number of iterations of "
	       "clustering to run; default 10 iterations\n");

	return;
}

/* Global variables */
struct point p[MAX_POINTS]; /* array that holds data points */
struct point u[MAX_CENTERS]; /* array that holds the centers */
int c[MAX_POINTS]; /* cluster id for each point */


int main(int argc, char *argv[])
{
	int o;
	
	char *data_file = "";
	int k = 2; /* number of clusters */
	int m = 0; /* number of data points */
	int iters = 10; /* number of iterations of clustering */
	int thread_cnt = 1;
	
	int i, j;

	/* parse command arguments */
	while(1){
		o = getopt_long(argc, argv, "hf:k:i:t:", long_options, NULL);
		if(o == -1) /* no more options */
			break;
		switch(o){
		case 'h':
			print_usage();
			exit(0);
		case 'f':
			data_file = strdup(optarg);
			break;
		case 'k':
			k = atoi(optarg);
			if(k > MAX_CENTERS){
				printf("Too many centers (must < %d)\n",
				       MAX_CENTERS);
				exit(-1);
			}
				
			break;
		case 'i':
			iters = atoi(optarg);
			break;
		case 't':
		        thread_cnt = atoi(optarg);
			break;
		default:
			printf("Unknown argument: %d\n", o);
			print_usage();
			exit(0);
		}
	}

	printf("Reading from data file: %s.\n", data_file);
	printf("Finding %d clusters\n", k);

	/* read data points from file */
	read_points_from_file(data_file, p, &m);

	/* randomly initialized the centers */
	for(j = 0; j < k; j++)
		u[j] = random_center(p);

	/* do K-Means */
    pthread_t threads[thread_cnt];
    pthread_barrier_t barrier;
	struct k_params params[thread_cnt];
    pthread_barrier_init(&barrier, NULL, thread_cnt);
	for(i = 0; i < thread_cnt; i++){
		/*
		 * TO STUDENTS: create threads and pass necessary parameters here
		 */
		params[i].iters = iters;
		params[i].m = m;
		params[i].k = k;
		params[i].m_decomp = m / thread_cnt;
		params[i].k_decomp = k / thread_cnt;
		params[i].m_start = params[i].m_decomp * i;
		params[i].k_start = params[i].k_decomp * i;
        params[i].barrier = &barrier;
		if (i == thread_cnt - 1) {
			params[i].m_decomp += m % thread_cnt;
			params[i].k_decomp += k % thread_cnt;
		}
		
        pthread_create(&threads[i], NULL, k_means, &params[i]);
	}

	/*
	 * TO STUDENTS: add code to stop main thread from quitting 
	 */
	for(i = 0; i < thread_cnt; i++) {
		pthread_join(threads[i], NULL);
	}
    pthread_barrier_destroy(&barrier);

	/* output centers and cluster assignment */
	printf("centers found:\n");
	for(j = 0; j < k; j++)
		printf("%.2lf, %.2lf\n", u[j].x, u[j].y);
		    

	return 0;
}

