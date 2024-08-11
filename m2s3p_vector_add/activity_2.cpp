#include <iostream>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <omp.h>
#include <thread>
#include <vector>

using namespace std::chrono;
using namespace std;

const int num_cores = thread::hardware_concurrency();
typedef void (*procedure)(unsigned long);

//------------sequential-----------//
void random_vector(int vector[], int size)
{
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

    auto start = high_resolution_clock::now();
    random_vector(v1, size);
    random_vector(v2, size);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "time taken by sequential random_vector: " << duration.count() << "ms" << endl;

    start = high_resolution_clock::now();
    for (int i = 0; i < size; i++)
    {
        v3[i] = v1[i] + v2[i];
    }
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "time taken by sequential sum v3: " << duration.count() << "ms" << endl;

    free(v1);
    free(v2);
    free(v3);
}

//------------omp-----------//
void random_vector_omp(int vector[], int size) {
    #pragma omp parallel for num_threads(num_cores) shared(vector, size) default(none)
    for (int i = 0; i < size; i++)
    {
        vector[i] = rand() % 100;
    }   
}

void parallel_omp(unsigned long size) {
    int *v1, *v2, *v3;

    v1 = (int *)malloc(size * sizeof(int *));
    v2 = (int *)malloc(size * sizeof(int *));
    v3 = (int *)malloc(size * sizeof(int *));

    auto start = high_resolution_clock::now();
    random_vector_omp(v1, size);
    random_vector_omp(v2, size);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "time taken by random_vector_omp: " << duration.count() << "ms" << endl;
    start = high_resolution_clock::now();
    int total = 0;
    #pragma omp parallel for num_threads(num_cores) shared (v1, v2, v3, size, num_cores) default(none) reduction(+:total) schedule(static)
    for (int i = 0; i < size; i++)
    {
        v3[i] = v1[i] + v2[i];
        total += v3[i];
    }
    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    cout << "time taken by omp sum v3: " << duration.count() << "ms" << endl;
    cout << "total sum of v3 elements (reduction): " << total << endl;

    free(v1);
    free(v2);
    free(v3);
}

//------------std::thread-----------//
void random_vector_chunk_stdthread(int vector[], int start, int end) {
    for (int i = start; i < end; i++) {
        vector[i] = rand() % 100;
    }
}

void sum_vector_chunk_stdthread(int v1[], int v2[], int v3[], int start, int end) {
    for (int i = start; i < end; i++) {
        v3[i] = v1[i] + v2[i];
    }
}

void random_vector_stdthread(int vector[], unsigned long size) {
    int num_threads;
    if (num_cores == 0) {
        num_threads = 2;
    } else {
        num_threads = num_cores;
    }

    std::vector<std::thread> threads;
    int chunk = size / num_threads;
    for (int i = 0; i < num_threads; i++) {
        int start = chunk * i;
        int end = start + chunk;
        threads.emplace_back(random_vector_chunk_stdthread, vector, start, end);
    }

    for (auto &th: threads) {
        th.join();
    }
}

void sum_vector_stdthread(int v1[], int v2[], int v3[], unsigned long size) {
    int num_threads;
    if (num_cores == 0) {
        num_threads = 2;
    } else {
        num_threads = num_cores;
    }

    std::vector<std::thread> threads;
    int chunk = size / num_threads;
    for (int i = 0; i < num_threads; i++) {
        int start = chunk * i;
        int end = start + chunk;
        threads.emplace_back(sum_vector_chunk_stdthread, v1, v2, v3, start, end);
    }

    for (auto& th: threads) {
        th.join();
    }
}

void parallel_stdthread(unsigned long size) {
    int *v1, *v2, *v3;

    v1 = (int *) malloc(size * sizeof(int *));
    v2 = (int *) malloc(size * sizeof(int *));
    v3 = (int *)malloc(size * sizeof(int *));

    auto start = high_resolution_clock::now();
    random_vector_stdthread(v1, size);
    random_vector_stdthread(v2, size);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "time taken by std::thread random vector: " << duration.count() << "ms" << endl;

    start = high_resolution_clock::now();
    sum_vector_stdthread(v1, v2, v3, size);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "time taken by std::thread sum v3: " << duration.count() << "ms" << endl;

    free(v1);
    free(v2);
    free(v3);
}

int main(){

    unsigned long size = 100000000;

    srand(time(0));

    procedure procedures[] = {sequential, parallel_omp, parallel_stdthread};
    const char *procedure_types[3] = {"sequential", "parallel_omp", "parallel_stdthread"};

    int num_procedures = end(procedures) - begin(procedures);

    for (int i = 0; i < num_procedures; i++) {
        auto start = high_resolution_clock::now();
        procedures[i](size);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);

        cout << "Time taken by " << procedure_types[i] << " protocol: "
            << duration.count() << " milliseconds" << endl << endl;
    }

    return 0;
}