// neuralNetwork.h
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// clang++ -Ofast --std=c++1y neuralNetwork.cpp -o neuralNetwork.o && ./neuralNetwork.o

#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>
using std::vector;

// Eigen matrix includes
#include "../third_party/Eigen/Core"
using Eigen::Matrix;
using Eigen::RowMajor;
using Eigen::Dynamic;

typedef Matrix<double, Dynamic, Dynamic, RowMajor> MatrixXXd;

class NeuralNetwork
{
public:
    NeuralNetwork(vector<int> topology);
    void feedForward();
    NeuralNetwork spawnChild();
    void print();
    
private:
    static double _sigmoid(double x);

    double _kingValue;
    vector<int> _topology;
    vector<MatrixXXd> _layers;
    vector<MatrixXXd> _weights;
    int _performance;
};

#endif

