
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


// global variables - matrix size, matrix, barriers
#define NUM_THREADS 16
int n = 256;
vector<vector<float> > A (n, vector<float> (n));

pthread_mutex_t lock[NUM_THREADS];
pthread_cond_t cond[NUM_THREADS];
int ready[NUM_THREADS]; //predicate variable for cond

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
            //printf("iter: %d pivot row comp pid:, %d\n", k, tid);
            for (j=k+1; j<n; j++)
                A[k][j] = A[k][j]/A[k][k]; /* all elements in the pivot row, to the right of pivot, divide by pivot */
            A[k][k] = 1;
        }

        // wait for pivot row to arrive from previous thread
        else { //wait for the previous thread to send the pivot row (logically)
            pthread_mutex_lock (&lock[tid]);
            while (ready[tid]==0) {
                //printf("iter: %d pid waiting: %d ready: %d\n", k, tid, ready[tid]);
                pthread_cond_wait(&cond[tid], &lock[tid]);
            }
            ready[tid] -= 1;
            //printf("iter: %d pid reset-ready: %d ready: %d\n", k, tid, ready[tid]);
            pthread_mutex_unlock (&lock[tid]);
        }

        // if this thread comes out of the wait - signal next thread to proceed
        long tid_n = (tid+1)%NUM_THREADS;
        if (tid_n != (k%NUM_THREADS)) { //cannot send the pivot row back to the thread computing it
            pthread_mutex_lock (&lock[tid_n]);
            ready[tid_n] += 1;
            //printf("iter: %d pid signalling: %d ready %d\n", k, tid_n, ready[tid_n]);
            pthread_cond_signal (&cond[tid_n]);
            pthread_mutex_unlock (&lock[tid_n]);
        }

        for (i=tid; i<n; i+=NUM_THREADS) {
            if (i>=k+1 && i<n) { /*only for rows below pivot, and within matrix*/
                for (j=k+1; j<n; j++) /*for all elements in the row*/
                    A[i][j] = A[i][j] - A[i][k] * A[k][j];
                A[i][k] = 0;
            }
        }
    }

    pthread_exit(NULL);
}


// main function - parse a file to form matrix (vec<vec<int> >), run gaussian-elimination
int main (int argc, char **argv) {

    assert (argc >= 2);
    //cout << "input path: " << argv[2] << endl;
    //n = atoi(argv[1]);

    // create matrix

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

    int rc, i;
    // initialize locks and conds
    for (i=0; i<NUM_THREADS; i++) {
        pthread_mutex_init (&lock[i], NULL);
        pthread_cond_init (&cond[i], NULL);
        ready[i] = 0;
    }

    // create and join threads
    pthread_t threads[NUM_THREADS];

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

    // destroy mutexes and conds
    for (i=0; i<NUM_THREADS; i++) {
        pthread_mutex_destroy (&lock[i]);
        pthread_cond_destroy (&cond[i]);
    }

    pthread_exit(NULL);

    return 0;
}




