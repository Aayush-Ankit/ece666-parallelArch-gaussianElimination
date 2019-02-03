
#include<iostream>
#include<cstdlib>
#include<sstream>
#include<fstream>
#include<cassert>
#include<iomanip>
#include<string>
#include<vector>
#include<pthread.h>
#include<chrono>

using namespace std;

#define NUM_THREADS 8

// global variables - matrix size, matrix, barriers
int n = 256;
vector<vector<float> > A (n, vector<float> (n));
pthread_barrier_t barrier1, barrier2;

// function to print the contents of a 2d matrix
void print () {
    int i, j;
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++)
            cout << setprecision(2) << A[i][j] << " ";
        cout << endl;
    }
}


// perform elimination on a 2d matrix - each thread operates on a partition of A
// (spcific number of rows)
void* eliminate (void *ptr) { /*triangulize the matrix A*/

    int k, i, j; // each thread has a local copy of iteration id (k-loop)
    long tid = (long) ptr;

    for (k=0; k<n; k++) { /*outermost loop of gauss-elimination*/
        //printf("iter: %d tid %d\n", k, tid);

        // compute pivot row if applicable (only 1 pivot row per k-th iteration)
        if (k%NUM_THREADS == tid) {
            //cout << "pivot row comp: threadId, " << tid << endl;
            for (j=k+1; j<n; j++)
                A[k][j] = A[k][j]/A[k][k]; /* all elements in the pivot row, to the right of pivot, divide by pivot */
            A[k][k] = 1;
        }
        pthread_barrier_wait(&barrier1); // barriers get reset once the reqd. num of threads have reached

        for (i=tid; i<n; i+=NUM_THREADS) {
            if (i>=k+1 && i<n) { /*only for rows below pivot, and within matrix*/
                for (j=k+1; j<n; j++) /*for all elements in the row*/
                    A[i][j] = A[i][j] - A[i][k] * A[k][j];
                A[i][k] = 0;
            }
        }
        //pthread_barrier_wait(&barrier2); // not reqd. - next iter it will either compute on its own part (if next pivot row is in its mapping), or wait
    }

    pthread_exit(NULL);
}


// main function - parse a file to form matrix (vec<vec<int> >), run gaussian-elimination
int main (int argc, char **argv) {

    assert (argc >= 2);
    //cout << "input path: " << argv[2] << endl;

    // create matrix
    //int n = atoi(argv[1]);

    // initialize matrix A from file
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

    // print matrix
    //print ();

    // initialize barriers
    pthread_barrier_init(&barrier1, NULL, NUM_THREADS);
    pthread_barrier_init(&barrier2, NULL, NUM_THREADS);

    // create and join threads
    pthread_t threads[NUM_THREADS];
    int rc, i;

    auto start = std::chrono::system_clock::now();

    for (i=0; i<NUM_THREADS; i++) {
        rc = pthread_create(&threads[i], NULL, eliminate, (void*) i);
        if (rc) {
            cout << "Error: unable to create thread, " << rc << endl;
            exit(-1);
        }
    }

    for (i=0; i<NUM_THREADS; i++) {
        rc = pthread_join(threads[i], NULL);
        if (rc) {
            cout << "Error: unable to join thread, " << rc << endl;
            exit(-1);
        }
    }

    auto finish = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    cout << "elapsed time " << (double) elapsed.count()/1000 << endl;

    // print result
    //cout << "\n" << "result:"  << "\n";
    //print(); // verify if triangular

    pthread_barrier_destroy(&barrier1);
    pthread_barrier_destroy(&barrier2);

    pthread_exit(NULL);

    return 0;
}




