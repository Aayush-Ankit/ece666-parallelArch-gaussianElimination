
#include<iostream>
#include<cstdlib>
#include<sstream>
#include<fstream>
#include<cassert>
#include<iomanip>
#include<string>
#include<vector>
#include <mpi.h>

using namespace std;

// function to print the contents of a 2d matrix
void print (const vector<vector<float> >& A, int n) {
    int i, j;
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++)
            cout << setprecision(2) << A[i][j] << " ";
        cout << endl;
    }
}


// perform elimination on a 2d matrix - each thread operates on a partition of A
// (spcific number of rows)
void eliminate (vector<vector<float> >& A, vector<float>& pivotA, int n, int block_size, int pid, int num_procs) { /*triangulize the matrix A*/
    int i, j, k;

    int start_row = pid*block_size;
    int end_row = start_row+block_size;

    for (k=0; k<n; k++) { /*loop over all diagonal (pivot) elements*/
        int pivot_pid = k/block_size;

        // do computation for pivot row and send data
        if (k>=start_row && k<end_row) {
            int pivot_row = k % block_size;
            for (j=k+1; j<n; j++) {/*for all elements in pivot row, to the right of pivot*/
                A[pivot_row][j] = A[pivot_row][j] / A[pivot_row][k];
                pivotA[j] = A[pivot_row][j];
                for (int Tpid=0; Tpid<num_procs; Tpid++)
                    if (Tpid != pid)
                        MPI_Send(&A[pivot_row][j], 1, MPI_FLOAT, Tpid, 2+j, MPI_COMM_WORLD);
            }
            //printf("pid %d - pivot send data complete\n", pid);
            A[pivot_row][k] = 1;
        }

        // receive pivotA if applicable
        else {
            for (j=k+1; j<n; j++)
                MPI_Recv(&pivotA[j], 1, MPI_FLOAT, pivot_pid, 2+j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //printf("pid %d - pivot receive data complete\n", pid);
        }

        // process all rows of myA
        for (i=start_row; i<end_row && i<n; i++) {
            if (i>k) {
                for (j=k+1; j<n; j++)
                    A[i%block_size][j] = A[i%block_size][j] - A[i%block_size][k] * pivotA[j];
                A[i%block_size][k] = 0;
            }
        }
    }
}


// main function - parse a file to form matrix (vec<vec<int> >), run gaussian-elimination
int main (int argc, char** argv) {

    int pid, num_procs;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs); // number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &pid); // process Id

    int n = atoi(argv[1]); // matrix size
    int block_size = n/num_procs;

    // root process
    if (pid == 0) {
        vector<vector<float> > A (n, vector<float> (n)); // input matrix
        vector<float> pivotA (n); // pivot row

        // initialize matrix from file
        string line;
        stringstream ss;
        float temp;
        ifstream infile (argv[2]);

        assert (infile.is_open());
        for (int i=0; i<n; i++) { // read a row
            assert (!infile.eof());
            getline (infile, line);
            ss.clear();
            ss.str(line);

            for (int j=0; j<n; j++) {
                assert (!ss.eof());
                ss >> temp;
                A[i][j] = temp;
            }
        }

        // print original matrix
        //print (A, n);

        // send matrix's rows to other processes
        for (int i=1; i<num_procs; i++) {
            for (int j=0; j<block_size; j++)
                for (int k=0; k<n; k++)
                    MPI_Send(&A[i*block_size+j][k], 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
        }
        //printf("pid %d send matrix completed\n", pid);

        // do eliminate function
        eliminate(A, pivotA, n, block_size, pid, num_procs); // matrix and number of iterations (num_iter = original matrix size)

        // receive updated matrices from non-root procs
        for (int i=1; i<num_procs; i++) {
            for (int j=0; j<block_size; j++)
                for (int k=0; k<n; k++)
                    MPI_Recv(&A[i*block_size+j][k], 1, MPI_FLOAT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        //printf("pid %d receive result completed\n", pid);

        // print result
        print (A, n);
    }

    // all other processes
    else {
        vector<vector<float> > myA (block_size, vector<float> (n)); // input matrix for child process
        vector<float> pivotA (n); // pivot row

        // receive data to allocate matrix
        for (int j=0; j<block_size; j++) {
            for (int k=0; k<n; k++)
                MPI_Recv(&myA[j][k], 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        //printf("pid %d receive matrix completed\n", pid);

        // do eliminate function
        eliminate(myA, pivotA, n, block_size, pid, num_procs);

        // send result to pid 0
        for (int j=0; j<block_size; j++) {
            for (int k=0; k<n; k++)
                MPI_Send(&myA[j][k], 1, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
        }
        //printf("pid %d send result completed\n", pid);
    }

    // Finalize the MPI envoronment
    MPI_Finalize();
    return 0;
}




