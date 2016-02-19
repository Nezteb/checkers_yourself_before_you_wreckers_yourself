// main.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
//
// Linux:
//     g++ -Ofast --std=c++1y -msse2 -fopenmp -DEIGEN_NO_MALLOC -DEIGEN_NO_DEBUG main.cpp -o main.o && time OMP_NUM_THREADS=8 ./main.o 32 40 10 1
//
// Mac: (need to figure out how to get support for OpenMP
//     clang++ -Ofast --std=c++1y -msse2 -DEIGEN_NO_MALLOC -DEIGEN_NO_DEBUG main.cpp -o main.o && time ./main.o 32 40 10 1
//
// SSE2 and OpenMP may actually make things slower, but include them for now...
// Still need to figure out how to get OpenMP working on Mac

// This triggers the user of either dynamic or static matrices
// Note that static matrices need to be manually tweaked
#define STATIC_MATRICES

// Eigen matrix includes
#include "Eigen/Core"
using Eigen::Matrix;
using Eigen::RowMajor;
using Eigen::Dynamic;
using Eigen::initParallel;
using Eigen::setNbThreads;
using Eigen::nbThreads;

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
    //return x;
    //return x+100.0;
    //return tanh(x);
    return 1/(1+exp(-x)); // faster than tanh
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "INCORRECT USAGE\nProper Usage:\n\t"<< argv[0] << " inputLayer [otherLayers] outputLayer"
        << "\n\t(Requires at least two layer topologies as inputs)" << endl;
    }
    
    // Initialize parallelization and check thread counts (only valid when fopenmp used)
    initParallel();
    setNbThreads(1024);
    cout << "Using " << nbThreads() << " threads" << endl;
    cout << endl;
    
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

    #ifdef DYNAMIC_MATRICES

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
    
    #endif // end DYNAMIC_MATRICES
    
    #ifdef STATIC_MATRICES
    
    Matrix<double, 1, 32, RowMajor> layer0; // 1x32
    layer0.setRandom();
    Matrix<double, 32, 40, RowMajor> weight0; // 32x40
    weight0.setRandom();
    Matrix<double, 1, 40, RowMajor> layer1; // 1x40
    layer1.setRandom();
    Matrix<double, 40, 10, RowMajor> weight1; // 40x10
    weight1.setRandom();
    Matrix<double, 1, 10, RowMajor> layer2; // 1x10
    layer2.setRandom();
    
    // For matrices with 1 column, you can't specify RowMajor
    Matrix<double, 10, 1> weight2; // 10x1
    weight2.setRandom();
    Matrix<double, 1, 1> layer3; // 1x1
    layer3.setRandom();
    
    #endif // end STATIC_MATRICES

    #ifdef DYNAMIC_OUTPUT

    #ifdef STATIC_MATRICES
    int j=0;
    for(int i=0; i<layers.size(); ++i)
    {
        cout << "Layer " << i << ": " << layers[i].rows() << "x" << layers[i].cols() << "\n\n" << layers[i] << "\n" << endl;
        
        if(j<weights.size())
        {
            cout << "Weight " << j << ": " << weights[j].rows() << "x" << weights[j].cols() << "\n\n" << weights[j] << "\n" << endl;
            ++j;
        }
    }
    #endif // end STATIC_MATRICES
    
    #endif // end DYNAMIC_OUTPUT

    int testCount = 1000000; // one million evaluations
    //int testCount = 1;
    cout << "Testing " << testCount << " random sets of input (input size: " << inputs[0] << ") ..." << endl;

    for(int i=0; i<testCount; ++i)
    {
        #ifdef DYNAMIC_MATRICES
        
        layers[0].setRandom(); // randomize input layer

        for(int j=1; j<layers.size(); ++j) // go through all the layers
        {
            // TODO: this is the slowest portion
            layers[j].noalias() = layers[j-1] * weights[j-1]; // next layer = previous layer * weights in between

            // TODO: faster way to do unary expressions?
            layers[j].noalias() = layers[j].unaryExpr(ptr_fun(sigmoid)); // apply sigmoid to layer
        }

        #ifdef DYNAMIC_OUTPUT
        cout << "OUTPUT: " << layers.back() << endl;
        #endif // end DYNAMIC_OUTPUT
        
        #endif // end DYNAMIC_MATRICES
        
        #ifdef STATIC_MATRICES
        
        layer0.setRandom();
        
        layer1.noalias() = layer0 * weight0;
        layer1.noalias() = layer1.unaryExpr(ptr_fun(sigmoid));
        
        layer2.noalias() = layer1 * weight1;
        layer2.noalias() = layer2.unaryExpr(ptr_fun(sigmoid));
        
        layer3.noalias() = layer2 * weight2;
        layer3.noalias() = layer3.unaryExpr(ptr_fun(sigmoid));
        
        #ifdef STATIC_OUTPUT
        cout << "OUTPUT: " << layer3 << endl;
        #endif // end STATIC_OUTPUT
        
        #endif // end STATIC_MATRICES
    }

    return 0;
}
