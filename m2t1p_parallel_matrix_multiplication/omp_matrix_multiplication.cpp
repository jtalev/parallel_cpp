#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace chrono;

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

mInt random_square_matrix(int size) {
    mInt matrix;
    for (int i = 0; i < size; i ++) {
        vector<int> row;
        for (int j = 0; j < size; j++) {
            row.push_back(rand() % 10);
        }
        matrix.push_back(row);
    }
    return matrix;
}

mInt multiply_matrices(mInt& m1, mInt& m2) {
    mInt matrix(m1.size(), vector<int>(m1[0].size()));
    int m1_rows = m1.size();    //i when i reaches m1_rows we are done m1
    int m2_rows = m2.size();    //k used to iterate over values in a column m2
    int m2_cols = m2[0].size(); //l used to iterate over values in a row m2

    #pragma omp parallel for
    for (int i = 0; i < m1_rows; i ++) {
        for (int l = 0; l < m2_cols; l++) {
            int val = 0;
            for (int k = 0; k < m2_rows; k++)
            {
                val += m1[i][k] * m2[k][l];
            }
            matrix[i][l] = val;
        }
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

int main() {
    mInt m1 = random_square_matrix(2);
    mInt m2 = random_square_matrix(2);

    ofstream file;
    file.open("output.txt", std::ios::app);
    if (!file.is_open()) {
        cout << "failed to open output file" << endl;
        return 1;
    }

    cout << "-----proof the omp program works-----" << endl;
    file << "-----proof the omp program works-----\n";
    cout << "m1:" << endl;
    file << "m1:\n";
    print_matrix(m1, file);
    
    cout << endl
         << "m2:" << endl;
    file << "\nm2:\n";
    print_matrix(m2, file);

    cout << endl
         << "m3:" << endl;
    file << "\nm3:\n";
    mInt m3 = multiply_matrices(m1, m2);
    print_matrix(m3, file);

    cout << "-----timing the omp program-----" << endl;
    file << "\n-----timing the omp program-----\n";
    m1.clear();
    m2.clear();
    m3.clear();

    m1 = load_matrix_from_file("m1.txt");
    m2 = load_matrix_from_file("m2.txt");
    auto start = high_resolution_clock::now();
    m3 = multiply_matrices(m1, m2);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "omp program: " << duration.count() << "ms" << endl;
    file << "omp program: " << duration.count() << "ms\n";

    mInt seq_result = load_matrix_from_file("m3.txt");
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
