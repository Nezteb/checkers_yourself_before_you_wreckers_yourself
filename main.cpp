// main.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// clang++ -Ofast --std=c++1y main.cpp -o main.o && ./main.o

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
        cout << "Layer " << i << ": " << argv[i] << " nodes\n";
    }
    cout << endl;
    
    vector<MatrixXXd> layers;
    vector<MatrixXXd> weights;
    
    //cout << MatrixXXd(atoi(argv[1]),atoi(argv[2]))::Random() << endl;
    
    // Create one matrix for each layer topology given (minus 1)
    for(int i=1; i<argc-1; ++i)
    {
        layers.push_back( MatrixXXd(1,atoi(argv[i])) );
        
        MatrixXXd temp(atoi(argv[i]),atoi(argv[i+1]));
        temp.setRandom();
        weights.push_back(temp);
    }
    layers.push_back( MatrixXXd(1,atoi(argv[argc-1])) );
    
    for(auto layer: layers)
    {
        //cout << "Thing:\n" << layer << endl;
    }
    
    for(auto weight: weights)
    {
        //cout << "Thing:\n" << weight << endl;
    }
    
    return 0;
}