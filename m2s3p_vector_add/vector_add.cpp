#include <iostream>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <omp.h>
#include <thread>

using namespace std::chrono;
using namespace std;

const int num_cores = thread::hardware_concurrency();
typedef void (*procedure)(unsigned long);

void random_vector(int vector[], int size)
{
    for (int i = 0; i < size; i++)
    {
        vector[i] = rand() % 100;
    }
}

void random_vector_omp(int vector[], int size) {
    #pragma omp parallel for num_threads(num_cores)
    for (int i = 0; i < size; i++)
    {
        vector[i] = rand() % 100;
    }   
}

void sequential(unsigned long size) {
    int *v1, *v2, *v3;

    v1 = (int *) malloc(size * sizeof(int *));
    v2 = (int *) malloc(size * sizeof(int *));
    v3 = (int *)malloc(size * sizeof(int *));

    random_vector(v1, size);

    random_vector(v2, size);

    for (int i = 0; i < size; i++)
    {
        v3[i] = v1[i] + v2[i];
    }

    free(v1);
    free(v2);
    free(v3);
}

void parallel_omp(unsigned long size) {
    int *v1, *v2, *v3;

    v1 = (int *)malloc(size * sizeof(int *));
    v2 = (int *)malloc(size * sizeof(int *));
    v3 = (int *)malloc(size * sizeof(int *));

    random_vector_omp(v1, size);
    random_vector_omp(v2, size);

    #pragma omp parallel for num_threads(num_cores)
    for (int i = 0; i < size; i++)
    {
        v3[i] = v1[i] + v2[i];
    }

    free(v1);
    free(v2);
    free(v3);
}

int main(){

    unsigned long size = 100000000;

    srand(time(0));

    procedure procedures[] = {sequential, parallel_omp};
    const char *procedure_types[2] = {"sequential", "parallel_omp"};

    int num_procedures = end(procedures) - begin(procedures);

    for (int i = 0; i < num_procedures; i++) {
        auto start = high_resolution_clock::now();
        procedures[i](size);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);

        cout << "Time taken by " << procedure_types[i] << " protocol: "
            << duration.count() << " milliseconds" << endl;
    }

    return 0;
}