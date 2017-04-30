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
void k_means(
    struct point p[MAX_POINTS], 
    int m, 
    int k, 
    int iters, 
    struct point u[MAX_CENTERS], 
    int c[MAX_POINTS], 
    int proc_cnt, 
    int proc_id) 
{
    /* custom MPI data type */
    MPI_Datatype MPI_POINT = create_mpi_point_type();

    /* integer array (of length group size) containing the number of elements that are to be received from each process */
    int point_revcounts[proc_cnt];
    int cluster_revcounts[proc_cnt];

    /* integer array (of length group size). Entry i specifies the displacement (relative to recvbuf) at which to place the incoming data from process i */
    int point_displs[proc_cnt];
    int cluster_displs[proc_cnt];

    /* compute work to do */
    // int m_decomp = m / proc_cnt;
    // int k_decomp = k / proc_cnt;
    // int m_start = m_decomp * proc_id;
    // int k_start = k_decomp * proc_id;
    // if (proc_id == proc_cnt - 1) {
    //     m_decomp += m % proc_cnt;
    //     k_decomp += k % proc_cnt;
    // }

    // int m_end = m_start + m_decomp; /* last point index assigned */
	// int k_end = k_start + k_decomp; /* last cluster index assigned */
	int c_iter;
	int c_cluster;
	int c_point;
    int i;

    /* update revcounts and displs arrays for MPI_Allgatherv() */
    for (i = 0; i < proc_cnt; i++) {
        int point_work = m / proc_cnt;
        int cluster_work = k / proc_cnt;
        point_revcounts[i] = point_work;
        cluster_revcounts[i] = cluster_work;
        if (i == proc_cnt - 1) {
            point_revcounts[i] = m - point_work * (proc_cnt - 1);
            cluster_revcounts[i] = k - cluster_work * (proc_cnt - 1);
        }
    }

    point_displs[0] = 0;
    cluster_displs[0] = 0;
    for (i = 1; i < proc_cnt; i++) {
        point_displs[i] = point_displs[i-1] + point_revcounts[i-1];
        cluster_displs[i] = cluster_displs[i-1] + cluster_revcounts[i-1];
    }

    int m_start = point_displs[proc_id];
    int m_count = point_revcounts[proc_id];
    int m_end = m_start + m_count;

    int k_start = cluster_displs[proc_id];
    int k_count = cluster_revcounts[proc_id];
    int k_end = k_start + k_count;

    /* compute centers */
    if(proc_id == 0) {
		for(i = 0; i < k; i++)
			u[i] = random_center(p);
	}

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(u, k, MPI_POINT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

	for (c_iter = 0; c_iter < iters; c_iter++)
	{
        /* find the nearest center to each point */
        for (c_point = m_start; c_point < m_end; c_point++)
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

		/* sync all processes */
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allgatherv(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, c, point_revcounts, point_displs, MPI_POINT, MPI_COMM_WORLD);

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

        /* sync all processes */
        MPI_Barrier(MPI_COMM_WORLD);
        // MPI_Allgatherv(&u[k_start], k_decomp, MPI_POINT, &u[0], cluster_revcounts, cluster_displs, MPI_POINT, MPI_COMM_WORLD);
        MPI_Allgatherv(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, u, cluster_revcounts, cluster_displs, MPI_POINT, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
    }
    return;
}

/* create a MPI data type for struct item */
MPI_Datatype create_mpi_point_type()
{
	int ret;
	MPI_Datatype new_type;

	int count = 2;
	int block_length[2] = {1, 1}; // 2 block each has 1 and 1 variables
	MPI_Aint offsets[2]; //array with beginning offset of each block;
	MPI_Datatype types[2] = {MPI_DOUBLE, MPI_DOUBLE}; // old types of each block are double, double
	MPI_Aint double_size;

	/*
	 * get the offsets for each block
	 */
	MPI_Type_extent(MPI_DOUBLE, &double_size); // get the size MPI_DOUBLE
	offsets[0] = 0; // block 1 starts from the beginning
	offsets[1] = double_size * 1; // block 2 starts after 1 double	

	ret = MPI_Type_struct(count, block_length, offsets, types, &new_type);
	if(ret != MPI_SUCCESS)
		printf("Type construct error: %d\n", ret);
	MPI_Type_commit(&new_type);
	if(ret != MPI_SUCCESS)
		printf("Type commit error: %d\n", ret);

	return new_type;
}