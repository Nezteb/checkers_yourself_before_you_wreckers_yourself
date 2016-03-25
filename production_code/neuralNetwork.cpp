// neuralNetwork.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// clang++ -Ofast --std=c++1y neuralNetwork.cpp -o neuralNetwork.o && ./neuralNetwork.o

#include "neuralNetwork.h"

// Eigen matrix includes
#include "../third_party/Eigen/Core"
using Eigen::Matrix;
using Eigen::RowMajor;
using Eigen::Dynamic;
using Eigen::IOFormat;
using Eigen::initParallel;
using Eigen::setNbThreads;
using Eigen::nbThreads;

typedef Matrix<double, Dynamic, Dynamic, RowMajor> MatrixXXd;

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
#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <sys/stat.h> // for mkdir
#include <sys/types.h>

random_device seed;
mt19937 superRandom(seed());

// random double
double dRand(double dMin, double dMax)
{
    double d = (double)rand() / RAND_MAX;
    return dMin + d * (dMax - dMin);
}

NeuralNetwork::NeuralNetwork(vector<int> topology): _topology(topology)
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

void NeuralNetwork::feedForward()
{
    for(int j=1; j<_layers.size(); ++j) // go through all the layers
    {
        _layers[j].noalias() = _layers[j-1] * _weights[j-1]; // next layer = previous layer * weights in between

        _layers[j].noalias() = _layers[j].unaryExpr(ptr_fun(&_sigmoid)); // apply sigmoid to layer
    }
}

NeuralNetwork NeuralNetwork::spawnChild()
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
    
void NeuralNetwork::print()
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
    
double NeuralNetwork::_sigmoid(double x)
{
    return 1/(1+exp(-x));
}

IOFormat CSV(Eigen::StreamPrecision, Eigen::DontAlignCols, ",", ",", "", "", "", "");

MatrixXXd NeuralNetwork::readWeightFromFile(const string subdirectory, string weightFilename)
{
    struct stat info;

    if(stat(subdirectory.c_str(), &info) != 0)
    {
        cout << "Cannot access: " << subdirectory << endl;
    }
    else if(!(info.st_mode & S_IFDIR))
    {
        cout << subdirectory << " is not a directory." << endl;
    }
    
    ostringstream stringStream;
    stringStream << subdirectory << "/" << weightFilename << ".txt";
    string fileName = stringStream.str();
    
    cout << "\nAttempting to read from file: " << fileName << endl;
    
    ifstream file(fileName, ifstream::in);
    
    string rowsString;
    string colsString;
    int rows;
    int cols;
    string tempString;
    
    MatrixXXd *pointer ;
    
    if (file.is_open())
    {
        getline(file, rowsString);
        getline(file, colsString);
        rows = stoi(rowsString);
        cols = stoi(colsString);
        
        MatrixXXd matrix(rows, cols);
        pointer = &matrix;
        
        for(int j = 0; j < rows; ++j)
        {
            for(int k = 0; k < cols; ++k)
            {
                getline(file, tempString, ',');
                matrix(j, k) = stof(tempString);
            }
        }
        
        file.close();
        return matrix;
    }
    else
    {
        cout << "Could not open: " << fileName << endl;
    }
}

void NeuralNetwork::writeWeightToFile(const string subdirectory, MatrixXXd weight, string weightFilename)
{
    struct stat info;

    if(stat(subdirectory.c_str(), &info) != 0)
    {
        cout << "Cannot access: " << subdirectory << endl;
    }
    else if(!(info.st_mode & S_IFDIR))
    {
        cout << subdirectory << " is not a directory." << endl;
    }
    
    // make the directory if it doesn't already exist
    const int dir_err = mkdir(subdirectory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    ostringstream stringStream;
    stringStream << subdirectory << "/" << weightFilename << ".txt";
    string fileName = stringStream.str();
    
    cout << "\nAttempting to write to file: " << fileName << endl;
    
    ofstream file(fileName, ofstream::out | ofstream::trunc);

    if (file.is_open())
    {
        file << weight.rows() << "\n";
        file << weight.cols() << "\n";
        file << weight.format(CSV);
        
        file.close();
    }
    else
    {
        cout << "Could not open: " << fileName << endl;
    }
}
