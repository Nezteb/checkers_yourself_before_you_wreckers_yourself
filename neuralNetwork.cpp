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
using std::assert;

class Neuron {}; // forward reference for Layer

typedef vector<Neuron> Layer;

struct Connection
{
    double weight;
    double deltaWeight;
}

class Neuron
{
public:
    Neuron(unsigned numberOutputs);
    void setOutputValue(double value) { _outputValue = value; }
    double getOutputValue(void) const { return _outputValue; }
    void feedForward(const Layer &previousLayer);

private:
    static double randomWeight(void) { return rand() / double(RAND_MAX); }
    double _outputValue;
    vector<Connection> _outputWeights;
}

void Neuron::feedForward(const Layer &previousLayer)
{
    double sum = 0.0

    for(unsigned n=0; n< pre)
}

Neuron::Neuron(unsigned numberOutputs)
{
    for(unsigned connections = 0; connections < numberOutputs; ++connections)
    {
        _outputWeights.push_back(Connection());
        _outputWeights.back().weight = randomWeight();
    }
}

class NeuralNetwork
{
public:
    NeuralNetwork(const vector<unsigned> &topology);
    void feedForward(const vector<double> &inputValues);
    void backPropogate(const vector<double> &targetValues) {};
    void getResults(vector<double> resultValues) const {};

private:
    vector<Layer> _layers; // _layers[layerNumber][neuronNumber]
};

void NeuralNetwork::feedForward(const vector<double> &inputValues)
{
    assert(intputValues.size() == _layers[0].size()-1);

    for(unsigned i = 0; i < inputValues.size(); ++i)
    {
        _layers[0][i].setOutputValue(inputValues[i]);
    }

    for(unsigned layerNumber = 1; layerNumber < _layers.size(); ++layerNumber)
    {
        Layer &previousLayer = _layers[layerNumber-1];

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
        _layers.push_back(Layer());
        unsigned numberOutputs = layerNumber == topology.size()-1 ? 0 : topology[layerNumber+1];

        for(unsigned neuronNumber = 0; neuronNumber <= topology[layerNumber]; ++neuronNumber)
        {
            _layers.back().push_back(Neuron(numberOutputs));
            cout << "Made a neuron." << endl;
        }
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

    vector<double> targetValues;
    test.backPropogate(targetValues); // tell network what outputs were supposed to be

    vector<double> resultValues;
    test.getResults(resultValues);
}
