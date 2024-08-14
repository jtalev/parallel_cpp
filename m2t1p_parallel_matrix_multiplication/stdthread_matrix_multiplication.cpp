#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef vector<vector<int>> mInt;
const int n = 1000;

void print_matrix(mInt& matrix, ofstream& file) {
    int rows = matrix.size();
    int columns = matrix[0].size();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            cout << matrix[i][j] << " ";
            file << matrix[i][j] << " ";
        }
        cout << endl;
        file << "\n";
    }
}

mInt load_matrix_from_file(string filename) {
    ifstream file(filename);
    mInt matrix;
    if (file.is_open()) {
        for (int i = 0; i < n; i++) {
            vector<int> row(n);
            for (int j = 0; j < n; j++) {
                file >> row[j];
            }
            matrix.push_back(row);
        }
        file.close();
    } else {
        cerr << "Error opening file: " << filename << endl;
    }

    return matrix;
}

void save_matrix_to_file(mInt& matrix, string filename) {
    ofstream file(filename);
    if (file.is_open()) {
        int rows = matrix.size();
        int cols = matrix[0].size();
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                file << matrix[i][j] << " ";
            }
            file << endl;
        }
        file.close();
    } else {
        cerr << "Error opening file: " << filename << endl;
    }
}

void section_multiplication(mInt& m1, mInt& m2, mInt& m3, int start, int end) {
    int m1_rows = m1.size();
    int m2_rows = m2.size();
    int m2_cols = m2[0].size();

    for (int i = start; i < end; i++) {
        for (int l = 0; l < m2_cols; l++) {
            int val = 0;
            for (int k = 0; k < m2_rows; k++) {
                val += m1[i][k] * m2[k][l];
            }
            m3[i][l] = val;
        }
    }
}

mInt multiply_matrices(mInt& m1, mInt& m2, mInt& m3) {
    int m1_rows = m1.size();
    int num_threads = thread::hardware_concurrency();
    int rows_per_thread = m1_rows / num_threads;
    int remaining_rows = m1_rows % num_threads;

    vector<thread> threads;
    int start = 0;
    for (int i = 0; i < num_threads; i++) {
        int end = start + rows_per_thread + (i < remaining_rows ? 1 : 0);
        threads.emplace_back(section_multiplication, ref(m1), ref(m2), ref(m3), start, end);
        start = end;
    }

    for (auto& th : threads) {
        th.join();
    }

    return m3;
}

int main() {
    ofstream file;
    file.open("output.txt", std::ios::app);

    cout << "-----proof the std:thread program works-----" << endl;
    file << "-----proof the std:thread program works-----" << endl;

    mInt test1 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    mInt test2 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    mInt test_results(3, vector<int>(3));
    multiply_matrices(test1, test2, test_results);
    cout << "m1:" << endl;
    file << "m1:" << endl;
    print_matrix(test1, file);
    cout << endl
         << "m2:" << endl;
    file << endl
         << "m2:" << endl;
    print_matrix(test2, file);
    cout << endl
         << "m3:" << endl;
    file << endl
         << "m3:" << endl;
    print_matrix(test_results, file);

    mInt m1 = load_matrix_from_file("m1.txt");
    mInt m2 = load_matrix_from_file("m2.txt");
    mInt m3(n, vector<int>(n));
    mInt seq_result = load_matrix_from_file("m3.txt");

    cout << "\n-----timing the std::thread program-----" << endl;
    file << "\n-----timing the std::thread program-----" << endl;
    auto start = high_resolution_clock::now();
    multiply_matrices(m1, m2, m3);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "std::thread program: " << duration.count() << "ms" << endl;
    file << "std::thread program: " << duration.count() << "ms" << endl;

    cout << endl << "-----checking result matrices-----" << endl;
    file << endl << "-----checking result matrices-----" << endl;
    for (int i = 0; i < m3.size(); i++) {
        for (int j = 0; j < m3[0].size(); j++) {
            if (m3[i][j] != seq_result[i][j]) {
                cout << endl << "matrices mismatch :( " << endl;
                file << endl << "matrices mismatch :( " << endl;
                return 1;
            }
        }
    }
    cout << "matrices match :)" << endl;
    file << "matrices match :)" << endl;
    
    file.close();
    return 0;
}