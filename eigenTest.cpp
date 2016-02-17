// eigenTest.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// clang++ -Ofast --std=c++1y eigenTest.cpp -o eigenTest.o && ./eigenTest.o

// Eigen reference material: http://eigen.tuxfamily.org/dox/group__QuickRefPage.html

#include "Eigen/Core"
using Eigen::Matrix;
using Eigen::RowMajor;
using Eigen::ColMajor;
using Eigen::Dynamic;
#include <iostream>
using std::cout;
using std::endl;

typedef Matrix<double, Dynamic, Dynamic, RowMajor> MatrixXXd;

int main()
{
    MatrixXXd operand1(3,4);
    operand1 << 1, 2, 3, 4,
                5, 6, 7, 8,
                9, 10, 11, 12;

    cout << "OPERAND 1:\n" << operand1 << endl;

    cout << endl;

    MatrixXXd operand2(4,5);
    operand2 << 1, 2, 3, 4, 5,
                6, 7, 8, 9, 10,
                11, 12, 13, 14, 15,
                16, 17, 18, 19, 20;

    cout << "OPERAND 2:\n" << operand2 << endl;

    cout << endl;

    MatrixXXd result(3,5);

    result = operand1 * operand2;

    cout << "RESULT:\n" << result << endl;

    cout << endl;

    cout << "ARRAY-STYLE ACCESS:\n";

    for(int row=0; row<3; ++row)
    {
        for(int col=0; col<5; ++col)
        {
            cout << "result(row: " << row << ", col: " << col << "): " << result(row,col) << endl;
        }
    }
    
    cout << "MULTIPLICATION TEST:" << endl;
    
    cout << "TEST 1:" << endl;
    MatrixXXd test1(3,3);
    test1 << 1,3,5,7,9,11,13,15,17;
    cout << test1 << endl;
    
    cout << "TEST 2:" << endl;
    MatrixXXd test2(3,3);
    test2 << 2,4,6,8,10,12,14,16,18;
    cout << test2 << endl;
    
    cout << "PRODUCT:" << endl;
    cout << test1*test2 << endl;
    
    return 0;
}
