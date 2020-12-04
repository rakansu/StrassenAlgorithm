#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <ostream>

using namespace std;

// File name to be processed
const char* FILE_NAME = "input.txt";
// Output file name
const char* OUTPUT_FILE_NAME = "output.txt";

int matrixSize;
vector<vector<int> > matrix_a;
vector<vector<int> > matrix_b;


void InitializeMatrices(std::ifstream& file);
vector<vector<int> > GetEmptyMatrix(int size);
vector<vector<int> > Partition(int r, int c,vector<vector<int> > M, int size);
vector<vector<int> > Add(vector<vector<int> > A, vector<vector<int> > B);
vector<vector<int> > Sub(vector<vector<int> > A, vector<vector<int> > B);
vector<vector<int> > Strassen(vector<vector<int> > A, vector<vector<int> > B, int n);



int main ()
{
    std::cout << "------- Start of Program -------" << '\n';
    // Attempt to open the file:
    std::ifstream file(FILE_NAME);

    // If file was not found:
    if(!file)
    {
        std::cout << "File was not found in current directory! Please, provide the file: " << FILE_NAME << '\n';
        std::cout << "Exiting ..." << '\n';
        return -1;
    }

    InitializeMatrices(file);
    vector<vector<int> > matrix_c = Strassen(matrix_a,matrix_b,matrixSize);

    std::ofstream output(OUTPUT_FILE_NAME);
    output << matrixSize << '\n';

    for(int i = 0; i < matrixSize; i++)
    {
        for(int j = 0; j < matrixSize; j++)
        {
            output << matrix_c[i][j] << ' ';
        }
        output << '\n';
    }

    std::cout << "------- End of Program -------" << '\n';
}



// Strassen Algorithm
vector<vector<int> > Strassen(vector<vector<int> > A, vector<vector<int> > B, int n)
{
    if(n == 1)
    {
        vector<vector<int> > result = GetEmptyMatrix(n);
        result[0][0] = A[0][0] * B[0][0];
        return result;
    } else
    {
        // Partition to 8 matrices:
        int sub_size = n / 2;
        vector<vector<int> > A11 = Partition(0,0,A,sub_size);
        vector<vector<int> > A12 = Partition(0,sub_size,A,sub_size);
        vector<vector<int> > A21 = Partition(sub_size,0,A,sub_size);
        vector<vector<int> > A22 = Partition(sub_size,sub_size,A,sub_size);
        
        vector<vector<int> > B11 = Partition(0,0,B,sub_size);
        vector<vector<int> > B12 = Partition(0,sub_size,B,sub_size);
        vector<vector<int> > B21 = Partition(sub_size,0,B,sub_size);
        vector<vector<int> > B22 = Partition(sub_size,sub_size,B,sub_size);
        

        // Create S1-S10 Matrices:
        vector<vector<int> > S1 = Sub(B12,B22);
        vector<vector<int> > S2 = Add(A11,A12);
        vector<vector<int> > S3 = Add(A21,A22);
        vector<vector<int> > S4 = Sub(B21,B11);
        vector<vector<int> > S5 = Add(A11,A22);
        vector<vector<int> > S6 = Add(B11,B22);
        vector<vector<int> > S7 = Sub(A12,A22);
        vector<vector<int> > S8 = Add(B21,B22);
        vector<vector<int> > S9 = Sub(A11,A21);
        vector<vector<int> > S10 = Add(B11,B12);



        // Call Strassen Algorithm on Sub-Matrices:
        vector<vector<int> > P1 = Strassen(A11,S1, sub_size);
        vector<vector<int> > P2 = Strassen(S2,B22, sub_size);
        vector<vector<int> > P3 = Strassen(S3,B11, sub_size);
        vector<vector<int> > P4 = Strassen(A22,S4, sub_size);
        vector<vector<int> > P5 = Strassen(S5,S6, sub_size);
        vector<vector<int> > P6 = Strassen(S7,S8, sub_size);
        vector<vector<int> > P7 = Strassen(S9,S10, sub_size);
        


        // Construct Sub-Matricies(C11 - C22):
        vector<vector<int> > C11 = Add(Sub(Add(P5,P4),P2),P6);
        vector<vector<int> > C12 = Add(P1,P2);
        vector<vector<int> > C21 = Add(P3,P4);
        vector<vector<int> > C22 = Sub(Sub(Add(P5,P1),P3),P7);
  


        // Merge Sub-Matrices (C11 - C22) into one matrix:
        vector<vector<int> > matrix_c = GetEmptyMatrix(n);
        
        for(int i = 0; i < sub_size; i++)
            for(int j = 0; j < sub_size; j++)
                matrix_c[i][j] = C11[i][j];

        for(int i = 0; i < sub_size; i++)
            for(int j = 0; j < sub_size; j++)
                matrix_c[i][j+sub_size] = C12[i][j];

        for(int i = 0; i < sub_size; i++)
            for(int j = 0; j < sub_size; j++)
                matrix_c[i+sub_size][j] = C21[i][j];

        for(int i = 0; i < sub_size; i++)
            for(int j = 0; j < sub_size; j++)
                matrix_c[i+sub_size][j+sub_size] = C22[i][j];
        
        return matrix_c;
    }
}


// divide matrix M into a square sub-matrix of [size] x [size] by specifying the starting row and column
vector<vector<int> > Partition(int row, int col,vector<vector<int> > M, int size)
{
    vector<vector<int> > sub_matrix = GetEmptyMatrix(size);
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            sub_matrix[i][j] = M[row+i][col+j];
    return sub_matrix;
}


// Creates an empty square matrix of [size] x [size]
vector<vector<int> > GetEmptyMatrix(int size)
{
    vector<vector<int> > matrix;
    for(int i = 0; i < size; i++) matrix.push_back(vector<int>(size,0));
    return matrix;
}


// Adds two matricies
vector<vector<int> > Add(vector<vector<int> > A, vector<vector<int> > B)
{
    vector<vector<int> > C = GetEmptyMatrix(A.size());
    for(int i = 0; i < A.size(); i++)
    {
        for(int j = 0; j < A.size(); j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}


// Subtracts two matricies
vector<vector<int> > Sub(vector<vector<int> > A, vector<vector<int> > B)
{
    vector<vector<int> > C = GetEmptyMatrix(A.size());
    for(int i = 0; i < A.size(); i++)
    {
        for(int j = 0; j < A.size(); j++)
        {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}


// Read the input of a matrix from a file
void InitializeMatrices(std::ifstream& file)
{
    std::string line;
    // Get Size:
    getline(file,line);
    matrixSize = atoi(line.c_str());
    
    // Initialize matrix A, B, and C of size [n x n]:
    for(int i = 0; i < matrixSize; i++)
    {
        matrix_a.push_back(vector<int>(matrixSize,0));
        matrix_b.push_back(vector<int>(matrixSize,0));
    }

    // Matrix A Creation:
    for(int r = 0; r < matrixSize; r++)
    {
        getline(file,line);
        int c = 0;
        std::stringstream tokenizer(line);
        std::string value;
        while(getline(tokenizer, value,' '))
        {
            matrix_a[r][c] = std::stoi(value);
            c++;
        }
    }

    // Matrix B Creation:
    for(int r = 0; r < matrixSize; r++)
    {
        getline(file,line);
        int c = 0;
        std::stringstream tokenizer(line);
        std::string value;
        while(getline(tokenizer, value,' '))
        {
            matrix_b[r][c] = std::stoi(value);
            c++;
        }
    }
}










