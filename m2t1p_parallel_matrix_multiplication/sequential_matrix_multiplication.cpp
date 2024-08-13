#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

typedef vector<vector<int>> mInt;

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
    mInt matrix;
    int m1_rows = m1.size();    //i when i reaches m1_rows we are done m1
    int m2_rows = m2.size();    //k used to iterate over values in a column m2
    int m2_cols = m2[0].size(); //l used to iterate over values in a row m2
    int val = 0;

    for (int i = 0; i < m1_rows; i ++) {
        vector<int> row;
        for (int l = 0; l < m2_cols; l++) {
            for (int k = 0; k < m2_rows; k++)
            {
                val += m1[i][k] * m2[k][l];
            }
            row.push_back(val);
            val = 0;
        }
        matrix.push_back(row);
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

int main() {
    int n = 2;
    mInt m1 = random_square_matrix(n);
    mInt m2 = random_square_matrix(n);

    ofstream file;
    file.open("output.txt", std::ios::app);
    if (!file.is_open()) {
        cout << "failed to open output file" << endl;
        return 1;
    }

    cout << "-----proof the program works-----" << endl;
    file << "-----proof the program works-----\n";
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

    cout << "-----timing the sequential program-----" << endl;
    file << "\n-----timing the sequential program-----\n";
    n = 1000;
    m1.clear();
    m2.clear();
    m3.clear();

    m1 = random_square_matrix(n);
    m2 = random_square_matrix(n);
    auto start = high_resolution_clock::now();
    m3 = multiply_matrices(m1, m2);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "sequential program: " << duration.count() << "ms" << endl;
    file << "sequential program: " << duration.count() << "ms\n";

    file.close();
    save_matrix_to_file(m1, "m1.txt");
    save_matrix_to_file(m2, "m2.txt");
    save_matrix_to_file(m3, "m3.txt");

    return 0;
}
