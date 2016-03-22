// gameplay.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// clang++ -Ofast --std=c++1y gameplay.cpp -o gameplay.o && time ./gameplay.o

#include "evolution.cpp"
#include "treeSearch.cpp"

void writeGameHistoryToFile(const vector<string> & gameHistory)
{
    string fileName = "gameHistory1.txt"
    ofstream file(fileName, ofstream::out | ofstream::trunc);

    if (file.is_open())
    {
        for(auto i : gameHistory) 
        {
            file << i << endl;
        }
        file.close();
    }
}



void gameplay(NeuralNetwork & nn1, NeuralNetwork & nn2)
{
    bool win = false;
    int turns = 0;
    bool redPlayerTurn = true;
    vector<string> gameHistory;
    string currentBoard = "rrrrrrrrrrrr________bbbbbbbbbbbb";
    
    
    while (!win || turns <= 100)
    {
        gameHistory.append(currentBoard);
        currentBoard = treeSearch(redPlayerTurn, nn1);
        redPlayerTurn = !redPlayerTurn;
        ++turns;
    }
    if(redWon) {
        nn1._performance += 1
        nn2._performance -= 2
    } else if(blackWon) {
        nn1._performance -= 2
        nn2._performance += 1
    } else if(tie) {
        nn1._performance -= 1
        nn2._performance -= 1
    }
    writeGameHistoryToFile(gameHistory);
}




void tournament(vector<NeuralNetwork> NNs)
{
    srand (time(NULL));
    for (int i = 0; i < NNs.count(); ++i)
    {
        for(int j = 0; j < 5; ++j)
        {
            int randNN = rand() % (NNs.count() - 1)
            if (i == randNN) {
                gameplay(NNs[i], NNs[(NNs.count() - 1)]);
            }
            else {
                gameplay(NNs[i], NNs[randNN]);
            }
        }
    }
}