
#include<iostream>
#include<cstdlib>
#include<sstream>
#include<fstream>
#include<cassert>
#include<iomanip>
#include<string>
#include<vector>
#include<chrono>

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


// function to perform elimination on a 2d matrix
void eliminate (vector<vector<float> >& A, int n) { /*triangulize the matrix A*/
    int i, j, k;

    for (k=0; k<n; k++) { /*loop over all diagonal (pivot) elements*/

        for (j=k+1; j<n; j++) /*for all elements in the row of, and to the right of pivot*/
            A[k][j] = A[k][j]/A[k][k]; /*divide by pivot*/

        A[k][k] = 1;

        for (i=k+1; i<n; i++) { /*for all rows below the  pivot row*/
            for (j=k+1; j<n; j++) /*for all elements in the row*/
                A[i][j] = A[i][j] - A[i][k] * A[k][j];
            A[i][k] = 0;
        }
    }
}


// main function - parse a file to form matrix (vec<vec<int> >), run gaussian-elimination
int main (int argc, char **argv) {

    assert (argc >= 2);
    //cout << "input path: " << argv[2] << endl;

    // create matrix
    int n = atoi(argv[1]);
    vector<vector<float> > A (n, vector<float> (n));

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

    //print (A, n); // print original

    auto start = std::chrono::system_clock::now();

    eliminate(A, n);

    auto finish = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    //cout << "elapsed time " << (double) elapsed.count()/1000 << endl;

    //cout << "\n" << "result:"  << "\n";
    print(A, n); // pring after elimination - verify if triangular

    return 0;
}




