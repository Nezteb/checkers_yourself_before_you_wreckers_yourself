// neuralNetwork.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// clang++ -Ofast --std=c++1y neuralNetwork.cpp -o neuralNetwork.o && ./neuralNetwork.o

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

class Neuron
{
public:
    Neuron(unsigned numberOutputs, unsigned myIndex);
    void setOutputValue(double value) { _outputValue = value; }
    double getOutputValue(void) const { return _outputValue; }
    void feedForward(const vector<Neuron> &previousLayer);
    void updateInputWeights(vector<Neuron> &previousLayer);

private:
    static double sigmoid(double x);
    static double sigmoidDerivative(double x);
    static double randomWeight(void) { return rand() / double(RAND_MAX); }
    double _outputValue;
    vector<double> _outputWeights;
    unsigned _myIndex;
};

void Neuron::updateInputWeights(vector<Neuron> &previousLayer)
{
    
}

double Neuron::sigmoid(double x) // sigmoid
{
    // tanh - output range [-1.0..1.0]
    return tanh(x);
}

double Neuron::sigmoidDerivative(double x) // sigmoid
{
    // tanh derivative
    return 1.0 - x*x; // approximation
}

void Neuron::feedForward(const vector<Neuron> &previousLayer)
{
    double sum = 0.0;

    for(unsigned n=0; n<previousLayer.size(); ++n)
    {
        sum += previousLayer[n].getOutputValue() *
                previousLayer[n]._outputWeights[_myIndex];
    }
    
    _outputValue = Neuron::sigmoid(sum); // transfer/activation function
    
}

Neuron::Neuron(unsigned numberOutputs, unsigned myIndex)
{
    for(unsigned connections = 0; connections < numberOutputs; ++connections)
    {
        _outputWeights.push_back(randomWeight());
    }
    
    _myIndex = myIndex;
}

class NeuralNetwork
{
public:
    NeuralNetwork(const vector<unsigned> &topology);
    void feedForward(const vector<double> &inputValues);
    void backPropogate(const vector<double> &targetValues);
    void getResults(vector<double> &resultValues) const;

private:
    vector<vector<Neuron>> _layers; // _layers[layerNumber][neuronNumber]
    double _error;
    double _recentAverageError;
    double _recentAverageSmoothingFactor;
};

void NeuralNetwork::getResults(vector<double> &resultValues) const
{
    resultValues.clear();
    
    for(unsigned n=0; n<_layers.back().size()-1; ++n)
    {
        resultValues.push_back(_layers.back()[n].getOutputValue());
    }
}

void NeuralNetwork::feedForward(const vector<double> &inputValues)
{
    //assert(inputValues.size() == _layers[0].size()-1);

    for(unsigned i = 0; i < inputValues.size(); ++i)
    {
        _layers[0][i].setOutputValue(inputValues[i]);
    }

    for(unsigned layerNumber = 1; layerNumber < _layers.size(); ++layerNumber)
    {
        vector<Neuron> &previousLayer = _layers[layerNumber-1];

        for(unsigned number = 0; number < _layers[layerNumber].size()-1; ++number)
        {
            _layers[layerNumber][number].feedForward(previousLayer);
        }
    }
}

NeuralNetwork::NeuralNetwork(const vector<unsigned> &topology)
{
    unsigned numberLayers = topology.size();
    for(unsigned layerNumber = 0; layerNumber < numberLayers; ++layerNumber)
    {
        _layers.push_back(vector<Neuron>());
        unsigned numberOutputs = layerNumber == topology.size()-1 ? 0 : topology[layerNumber+1];

        for(unsigned neuronNumber = 0; neuronNumber <= topology[layerNumber]; ++neuronNumber)
        {
            _layers.back().push_back(Neuron(numberOutputs, neuronNumber));
            cout << "Made a neuron." << endl;
        }
        
        _layers.back().back().setOutputValue(1.0);
    }
}

int main()
{
    vector<unsigned> topology;
    topology.push_back(3);
    topology.push_back(2);
    topology.push_back(1);
    NeuralNetwork test(topology);

    vector<double> inputValues;
    test.feedForward(inputValues); // feed values forward in network

    vector<double> resultValues;
    test.getResults(resultValues);
}
