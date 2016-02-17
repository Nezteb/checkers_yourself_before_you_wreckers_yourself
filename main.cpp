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
#include <functional>
using std::ptr_fun;

double sigmoid(double x)
{
    //x += 100;
    return tanh(x);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "INCORRECT USAGE\nProper Usage:\n\t"<< argv[0] << " inputLayer [otherLayers] outputLayer"
        << "\n\t(Requires at least two layer topologies as inputs)" << endl;
    }

    /*cout << "TOPOLOGY:\n";
    for(int i=1; i<argc; ++i)
    {
        cout << "Layer " << i << ": " << argv[i] << " nodes\n";
    }
    cout << endl;*/

    vector<MatrixXXd> layers;
    vector<MatrixXXd> weights;

    MatrixXXd firstLayer(1, atoi(argv[1]));
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

    for(int i=0; i<1000000; ++i)
    {
        layers[0].setRandom();

        for(int i=1; i<layers.size(); ++i)
        {
            // TODO: this is the slowest portion
            layers[i] = layers[i-1] * weights[i-1];

            // TODO: faster way to do unary expressions?
            //layers[i].unaryExpr(ptr_fun(sigmoid));
            layers[i] = layers[i].unaryExpr(ptr_fun(sigmoid));
        }

        //cout << "OUTPUT LAYER:\n" << layers.back() << endl;
    }

    return 0;
}
