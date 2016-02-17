// main.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// clang++ -Ofast --std=c++1y main.cpp -o main.o && time ./main.o 32 40 10 1

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

    cout << "TOPOLOGY:\n";
    for(int i=1; i<argc; ++i)
    {
        cout << "Layer " << i-1 << ": " << argv[i] << " nodes\n";
    }
    cout << endl;

    vector<MatrixXXd> layers;
    vector<MatrixXXd> weights;

    MatrixXXd firstLayer(1, atoi(argv[1])); // input layer
    firstLayer.setRandom();
    layers.push_back(firstLayer);

    MatrixXXd firstWeight(atoi(argv[1]),atoi(argv[2]));
    firstWeight.setRandom();
    weights.push_back(firstWeight);

    // Create one matrix for each layer topology given (minus 1)
    for(int i=2; i<argc-1; ++i)
    {
        layers.push_back( MatrixXXd(1,atoi(argv[i])) );

        MatrixXXd temp(atoi(argv[i]),atoi(argv[i+1]));
        temp.setRandom();
        weights.push_back(temp);
    }
    layers.push_back( MatrixXXd(1,atoi(argv[argc-1])) );

    for(int i=0; i<layers.size(); ++i)
    {
        cout << "Layer " << i << ": " << layers[i].rows() << "x" << layers[i].cols() << "\n" << layers[i] << endl;
        cout << endl;
    }
    
    cout << endl;
    
    for(int i=0; i<weights.size(); ++i)
    {
        cout << "Weight " << i << ": " << weights[i].rows() << "x" << weights[i].cols() << "\n" << weights[i] << endl;
        cout << endl;
    }

    int testCount = 1;
    cout << "Testing " << testCount << " random sets of input (input size: " << atoi(argv[1]) << ") ..." << endl;

    for(int i=0; i<testCount; ++i) // one million evaluations
    {
        layers[0].setRandom(); // randomize input layer

        for(int j=1; j<layers.size(); ++j) // go through all the layers
        {
            // TODO: this is the slowest portion
            layers[j] = layers[j-1] * weights[j-1]; // next layer = previous layer * weights in between

            // TODO: faster way to do unary expressions?
            layers[j] = layers[j].unaryExpr(ptr_fun(sigmoid)); // apply sigmoid to layer
        }

        //cout << "OUTPUT: " << layers.back() << endl;
    }

    return 0;
}
