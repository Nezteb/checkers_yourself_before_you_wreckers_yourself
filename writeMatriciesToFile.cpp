// writeMatriciesToFile.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// clang++ -Ofast --std=c++1y writeMatriciesToFile.cpp -o writeMatriciesToFile.o && time ./writeMatriciesToFile.o

#include "Eigen/Core"
using Eigen::MatrixXf;
using Eigen::IOFormat;
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <sys/stat.h> // for mkdir

IOFormat CSV(Eigen::StreamPrecision, Eigen::DontAlignCols, ",", ",", "", "", "", "");

int main()
{
    // create directory to save weights in
    const int dir_err = mkdir("saved_matrix_weights", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    
    cout << "\n========== WRITING MATRICES TO FILES ==========" << endl;
    
    for(int i = 0; i < 10; ++i)
    {
        MatrixXf matrix = MatrixXf::Random(4,4);

        ostringstream stringStream;
        stringStream << "saved_matrix_weights/matrix" << i << ".txt";
        string fileName = stringStream.str();
        
        cout << "\nAttempting to write to file: " << fileName << endl;
        cout << matrix << endl;
        
        ofstream file(fileName, ofstream::out | ofstream::trunc);

        if (file.is_open())
        {
            file << matrix.rows() << "\n";
            file << matrix.cols() << "\n";
            file << matrix.format(CSV);
            
            file.close();
        }
    }
    
    cout << "\n========== READING MATRICES FROM FILES ==========" << endl;
    
    for(int i = 0; i < 10; ++i)
    {
        ostringstream stringStream;
        stringStream << "saved_matrix_weights/matrix" << i << ".txt";
        string fileName = stringStream.str();
        
        cout << "\nAttempting to read from file: " << fileName << endl;
        
        ifstream file(fileName, ifstream::in);
        
        string rowsString;
        string colsString;
        int rows;
        int cols;
        string tempString;
        
        if (file.is_open())
        {
            getline(file, rowsString);
            getline(file, colsString);
            rows = stoi(rowsString);
            cols = stoi(colsString);
            
            MatrixXf matrix(rows, cols);
            
            for(int j = 0; j < rows; ++j)
            {
                for(int k = 0; k < cols; ++k)
                {
                    getline(file, tempString, ',');
                    matrix(j, k) = stof(tempString);
                }
            }
            
            cout << "Matrix " << i << ":" << endl;
            cout << "rows: " << rows << ", cols: " << cols << endl;
            cout << matrix << endl;
            
            file.close();
        }
    }
    
    return 0;
}
