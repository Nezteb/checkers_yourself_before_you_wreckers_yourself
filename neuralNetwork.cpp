// neuralNetwork.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// clang++ -Ofast --std=c++1y neuralNetwork.cpp -o neuralNetwork.o && ./neuralNetwork.o

#include <vector>
using std::vector;

class Neuron;

typedef vector<Neuron> Layer;

class NeuralNetwork
{
public:
    NeuralNetwork(const vector<unsigned> &topology);
    void feedForward(const vector<double> &inputValues);
    void backPropogate(const vector<double> &targetValues);
    void getResults(vector<double> resultValues) const;

private:
    vector<Layer> _layers; // _layers[layerNumber][neuronNumber]
}

NeuralNetwork::NeuralNetwork(const vector<unsigned> &topology)
{
    unsigned numberLayers = topology.size();
    for(unsigned layerNumber=0; layerNumber<numberLayers; ++layerNumber)
    {
        _layers.push_back(Layer());
    }
}

int main()
{
    vector<unsigned> topology;
    NeuralNetwork test(topology);

    vector<double> inputValues;
    test.feedForward(inputValues); // feed values forward in network

    vector<double> targetValues;
    test.backPropogate(targetValues); // tell network what outputs were supposed to be

    vector<double> resultValues;
    test.getResults(resultValues);
}
