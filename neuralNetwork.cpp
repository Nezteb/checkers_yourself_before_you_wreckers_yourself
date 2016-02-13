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

class Neuron; // forward reference for Layer

typedef vector<Neuron> Layer;

struct Connection
{
    double weight;
    double deltaWeight;
};

class Neuron
{
public:
    Neuron(unsigned numberOutputs, unsigned myIndex);
    void setOutputValue(double value) { _outputValue = value; }
    double getOutputValue(void) const { return _outputValue; }
    void feedForward(const Layer &previousLayer);
    void calculateOutputGradients(double targetValue);
    void calculateHiddenGradients(const Layer &nextLayer);
    void updateInputWeights(Layer &previousLayer);

private:
    static double eta; // [0.0..1.0] overall net training rate
    static double alpha; // [0.0..n] multiplier of last weight change (momentum)
    static double transferFunction(double x);
    static double transferFunctionDerivative(double x);
    static double randomWeight(void) { return rand() / double(RAND_MAX); }
    double sumDOW(const Layer &nextLayer) const;
    double _outputValue;
    vector<Connection> _outputWeights;
    unsigned _myIndex;
    double _gradient;
};

double Neuron::eta = 0.15; // learning rate
double Neuron::alpha = 0.5; // momentum, multipler of last deltaWeight

void Neuron::updateInputWeights(Layer &previousLayer)
{
    // weights to be updated are in the connection container in the neurons in the preceding layer
    
    for(unsigned n=0; n<previousLayer.size(); ++n)
    {
        Neuron &neuron = previousLayer[n];
        double oldDeltaWeight = neuron._outputWeights[_myIndex].deltaWeight;
        
        double newDeltaWeight =
            // individual input, magnified by the gradient and train rate:
            eta *
            neuron.getOutputValue() *
            _gradient *
            // momentum = a fraction of the previous delta weight
            alpha *
            oldDeltaWeight;
            
        neuron._outputWeights[_myIndex].deltaWeight = newDeltaWeight;
        neuron._outputWeights[_myIndex].weight += newDeltaWeight;
    }
}

double Neuron::sumDOW(const Layer &nextLayer) const
{
    double sum = 0.0;
    
    // sum our contribution sof the errors at the nodes we feed
    
    for(unsigned n=0; n<nextLayer.size()-1; ++n)
    {
        sum += _outputWeights[n].weight * nextLayer[n]._gradient;
    }
    
    return sum;
}

void Neuron::calculateHiddenGradients(const Layer &nextLayer)
{
    double dow = sumDOW(nextLayer);
    _gradient = dow * Neuron::transferFunctionDerivative(_outputValue);
}

void Neuron::calculateOutputGradients(double targetValue)
{
    double delta = targetValue - _outputValue;
    _gradient = delta * Neuron::transferFunctionDerivative(_outputValue);
}

double Neuron::transferFunction(double x) // sigmoid
{
    // tanh - output range [-1.0..1.0]
    return tanh(x);
}

double Neuron::transferFunctionDerivative(double x) // sigmoid
{
    // tanh derivative
    return 1.0 - x*x; // approximation
}

void Neuron::feedForward(const Layer &previousLayer)
{
    double sum = 0.0;

    for(unsigned n=0; n<previousLayer.size(); ++n)
    {
        sum += previousLayer[n].getOutputValue() *
                previousLayer[n]._outputWeights[_myIndex].weight;
    }
    
    _outputValue = Neuron::transferFunction(sum); // transfer/activation function
    
}

Neuron::Neuron(unsigned numberOutputs, unsigned myIndex)
{
    for(unsigned connections = 0; connections < numberOutputs; ++connections)
    {
        _outputWeights.push_back(Connection());
        _outputWeights.back().weight = randomWeight();
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
    vector<Layer> _layers; // _layers[layerNumber][neuronNumber]
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

void NeuralNetwork::backPropogate(const vector<double> &targetValues)
{
    // calulate overall net error (RMS of output errors)
    Layer &outputLayer = _layers.back();
    _error = 0.0;
    
    for(unsigned n=0; n<outputLayer.size()-1; ++n)
    {
        double delta = targetValues[n] - outputLayer[n].getOutputValue();
        _error += delta*delta;
    }
    
    _error /= outputLayer.size()-1;
    _error = sqrt(_error);
    
    // Implement a recent average measurement:
    
    _recentAverageError = (_recentAverageError * _recentAverageSmoothingFactor + _error) /
                            (_recentAverageSmoothingFactor+1.0);
    
    // calculate output layer gradients
    
    for(unsigned n=0; n<outputLayer.size()-1; ++n)
    {
        outputLayer[n].calculateOutputGradients(targetValues[n]);
    }
    
    // calculate gradients on hidden lyaers
    
    for(unsigned layerNumber = _layers.size()-2; layerNumber>0; --layerNumber)
    {
        Layer &hiddenLayer = _layers[layerNumber];
        Layer &nextLayer = _layers[layerNumber+1];
        
        for(unsigned n=0; n<hiddenLayer.size(); ++n)
        {
            hiddenLayer[n].calculateHiddenGradients(nextLayer);
        }
    }
    
    // for all layers from outputs to first hidden layer, update connection weights

    for(unsigned layerNumber = _layers.size()-1; layerNumber>0; --layerNumber)
    {
        Layer &layer = _layers[layerNumber];
        Layer &previousLayer = _layers[layerNumber-1];
        
        for(unsigned n=0; n<layer.size()-1; ++n)
        {
            layer[n].updateInputWeights(previousLayer);
        }
    }
}

void NeuralNetwork::feedForward(const vector<double> &inputValues)
{
    assert(inputValues.size() == _layers[0].size()-1);

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
            _layers.back().push_back(Neuron(numberOutputs, neuronNumber));
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
