// main.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// Linux:
//     g++ -Ofast --std=c++1y -msse2 -fopenmp -DEIGEN_NO_DEBUG main.cpp -o main.o && time OMP_NUM_THREADS=8 ./main.o 32 40 10 1
// Mac: (need clang-omp installed)
//     clang-omp -Ofast --std=c++1y -msse2 -fopenmp -DEIGEN_NO_DEBUG main.cpp -o main.o && time OMP_NUM_THREADS=8 ./main.o 32 40 10 1

// Eigen matrix includes
#include "Eigen/Core"
using Eigen::Matrix;
using Eigen::RowMajor;
using Eigen::Dynamic;

typedef Matrix<double, Dynamic, Dynamic, RowMajor> MatrixXXd;

// Standard includes
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;
#include <cstdlib>
using std::rand;
#include <cassert>
#include <cmath>
using std::tanh;
#include <functional>
using std::ptr_fun;

double sigmoid(double x)
{
    return tanh(x);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "INCORRECT USAGE\nProper Usage:\n\t"<< argv[0] << " inputLayer [otherLayers] outputLayer"
        << "\n\t(Requires at least two layer topologies as inputs)" << endl;
    }
    
    vector<int> inputs;
    
    for(int i=1; i<argc; ++i) // first input is program name
    {
        inputs.push_back(atoi(argv[i]));
    }

    cout << "TOPOLOGY:\n";
    for(int i=0; i<inputs.size(); ++i)
    {
        cout << "Layer " << i << ": " << inputs[i] << " nodes\n";
    }
    cout << endl;

    vector<MatrixXXd> layers;
    vector<MatrixXXd> weights;

    for(int i=0; i<inputs.size(); ++i) // layers
    {
        layers.push_back( MatrixXXd(1, inputs[i]) );
        layers[i].setRandom();
    }
    
    for(int i=0; i<inputs.size()-1; ++i) // weights
    {
        weights.push_back( MatrixXXd(inputs[i], inputs[i+1]) );
        weights[i].setRandom();
    }

    /*int j=0;
    for(int i=0; i<layers.size(); ++i)
    {
        cout << "Layer " << i << ": " << layers[i].rows() << "x" << layers[i].cols() << "\n\n" << layers[i] << "\n" << endl;
        
        if(j<weights.size())
        {
            cout << "Weight " << j << ": " << weights[j].rows() << "x" << weights[j].cols() << "\n\n" << weights[j] << "\n" << endl;
            ++j;
        }
    }*/

    int testCount = 1000000; // one million evaluations
    cout << "Testing " << testCount << " random sets of input (input size: " << inputs[0] << ") ..." << endl;

    for(int i=0; i<testCount; ++i)
    {
        layers[0].setRandom(); // randomize input layer

        for(int j=1; j<layers.size(); ++j) // go through all the layers
        {
            // TODO: this is the slowest portion
            layers[j].noalias() = layers[j-1] * weights[j-1]; // next layer = previous layer * weights in between

            // TODO: faster way to do unary expressions?
            layers[j].noalias() = layers[j].unaryExpr(ptr_fun(sigmoid)); // apply sigmoid to layer
        }

        //cout << "OUTPUT: " << layers.back() << endl;
    }

    return 0;
}
