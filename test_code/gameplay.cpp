// gameplay.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// clang++ -Ofast --std=c++1y gameplay.cpp -o gameplay.o && time ./gameplay.o

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

//#include "evolution.cpp"
//#include "treeSearch.cpp"

#include <algorithm>
using std::reverse;
using std::replace;

string invertBoard(string board)
{
    string tempBoard = board;
    reverse(tempBoard.begin(), tempBoard.end());
    
    // need a temp character
    replace(tempBoard.begin(), tempBoard.end(), 'r', 't' );
    replace(tempBoard.begin(), tempBoard.end(), 'R', 'T' );
    
    replace(tempBoard.begin(), tempBoard.end(), 'b', 'r' );
    replace(tempBoard.begin(), tempBoard.end(), 'B', 'R' );
    
    replace(tempBoard.begin(), tempBoard.end(), 't', 'b' );
    replace(tempBoard.begin(), tempBoard.end(), 'T', 'B' );
    
    return tempBoard;
}

void writeGameHistoryToFile(const vector<string> & gameHistory)
{
    string fileName = "testGameHistory/gameHistory1.txt"
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




void tournament(vector<NeuralNetwork> & NNs)
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

int main()
{
    string board = "bbb___rrRbB___brbrbrbRBRRBrbrb";
    string reverse = invertBoard(board);
    cout << board << "\nbecomes:\n" << reverse << endl;
    
    /*NeuralNetwork nn1;
    NeuralNetwork nn2;
    gameplay(nn1, nn2);
    cout << "NN1 Performance: " << nn1._performance << endl;
    cout << "NN2 Performance: " << nn2._performance << endl;*/
    
    
    
    return 0;
}