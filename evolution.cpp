// evolution.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
//
// Linux:
//     g++ -Ofast --std=c++1y -msse2 -fopenmp -DEIGEN_NO_MALLOC -DEIGEN_NO_DEBUG evolution.cpp -o evolution.o && time OMP_NUM_THREADS=8 ./evolution.o 32 40 10 1
//
// Mac: (need to figure out how to get support for OpenMP
//     clang++ -Ofast --std=c++1y -msse2 -DEIGEN_NO_MALLOC -DEIGEN_NO_DEBUG evolution.cpp -o evolution.o && time ./evolution.o 32 40 10 1

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
using std::srand;
using std::rand;
#include <cassert>
#include <cmath>
using std::tanh;
#include <functional>
using std::ptr_fun;
#include <random>
using std::random_device;
using std::mt19937;
using std::normal_distribution;

random_device seed;
mt19937 superRandom(seed());

// random double
double dRand(double dMin, double dMax)
{
    double d = (double)rand() / RAND_MAX;
    return dMin + d * (dMax - dMin);
}

class NeuralNetwork
{
public:
    NeuralNetwork(vector<int> topology): _topology(topology)
    {
        _performance = 0;
        _kingValue = dRand(1.0,2.0);
        
        for(int i=0; i<_topology.size(); ++i) // layers
        {
            _layers.push_back( MatrixXXd(1, _topology[i]) );
            _layers[i].setRandom();
        }

        for(int i=0; i<_topology.size()-1; ++i) // weights
        {
            _weights.push_back( MatrixXXd(_topology[i], _topology[i+1]) );
            _weights[i].setRandom();
        }
    }

    void feedForward()
    {
        for(int j=1; j<_layers.size(); ++j) // go through all the layers
        {
            _layers[j].noalias() = _layers[j-1] * _weights[j-1]; // next layer = previous layer * weights in between

            _layers[j].noalias() = _layers[j].unaryExpr(ptr_fun(&_sigmoid)); // apply sigmoid to layer
        }
    }

    NeuralNetwork spawnChild()
    {
        NeuralNetwork childNetwork(_topology);

        for(int i = 0; i < _weights.size(); ++i)
        {
            for(int row = 0; row < _weights[i].rows(); ++row)
            {
                for(int col = 0; col < _weights[i].cols(); ++col)
                {
                    // create new distirubtion based on weight and sigma
                    normal_distribution<double> distribution(_weights[i](row,col), dRand(0.0, 1.0));

                    // apply distribution to weight
                    childNetwork._weights[i](row,col) = distribution(superRandom);
                }
            }
        }
        
        // randomize king value slightly
        normal_distribution<double> distribution(_kingValue, dRand(0.0, 1.0));
        childNetwork._kingValue = distribution(superRandom);

        return childNetwork;
    }
    
    void print()
    {
        int j = 0;
        for(int i = 0; i < _layers.size(); ++i)
        {
            cout << "Layer " << i << ": " << _layers[i].rows() << "x" << _layers[i].cols() << "\n\n" << _layers[i] << "\n" << endl;
            
            if(j < _weights.size())
            {
                cout << "Weight " << j << ": " << _weights[j].rows() << "x" << _weights[j].cols() << "\n\n" << _weights[j] << "\n" << endl;
                ++j;
            }
        }
    }
    
private:
    static double _sigmoid(double x)
    {
        return 1/(1+exp(-x));
    }

    double _kingValue;
    vector<int> _topology;
    vector<MatrixXXd> _layers;
    vector<MatrixXXd> _weights;
    int _performance;
};

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "INCORRECT USAGE\nProper Usage:\n\t"<< argv[0] << " inputLayer [otherLayers] outputLayer"
        << "\n\t(Requires at least two layer topologies as inputs)" << endl;
    }
    
    initParallel();
    setNbThreads(1024);
    cout << "Using " << nbThreads() << " threads" << endl;
    cout << endl;

    srand(time(NULL));

    vector<int> inputs;

    for(int i=1; i<argc; ++i) // first input is program name
    {
        inputs.push_back(atoi(argv[i]));
    }

    NeuralNetwork test(inputs);
    
    cout << "PARENT:" << endl;
    test.print();

    NeuralNetwork baby = test.spawnChild();
    
    cout << "CHILD:" << endl;
    baby.print();

    return 0;
}
