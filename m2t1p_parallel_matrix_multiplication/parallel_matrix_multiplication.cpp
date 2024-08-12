#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

typedef vector<vector<int>> mInt;

void print_matrix(mInt& matrix) {
    int rows = matrix.size();
    int columns = matrix[0].size();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
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

mInt multiply_matrices(mInt m1, mInt m2) {
    mInt matrix;
    int m1_rows = m1.size();    //i when i reaches m1_rows we are done m1
    int m2_rows = m2.size();    //k used to iterate over values in a column m2
    int m2_cols = m2[0].size(); //l used to iterate over values in a row m2
    int j = 0, val = 0;

    for (int i = 0; i < m1_rows; i ++) {
        vector<int> row;
        for (int l = 0; l < m2_cols; l++) {
            for (int k = 0; k < m2_rows; k++)
            {
                val += m1[i][j] * m2[k][l];
                j++;
            }
            row.push_back(val);
            val = 0;
            j = 0;
        }
        matrix.push_back(row);
    }

    return matrix;
}

int main() {
    int n = 2;
    mInt m1 = random_square_matrix(n);
    mInt m2 = random_square_matrix(n);

    cout << "-----proof the program works-----" << endl;
    cout << "m1:" << endl;
    print_matrix(m1);
    
    cout << endl
         << "m2:" << endl;
    print_matrix(m2);

    cout << endl
         << "m3:" << endl;
    mInt m3 = multiply_matrices(m1, m2);
    print_matrix(m3);



    cout << "-----timing the sequential program-----" << endl;
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

    return 0;
}
