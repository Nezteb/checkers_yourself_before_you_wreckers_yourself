// treePrint.cpp
// Noah Betzen & Lonny Strunk
// CS 405

// Compile and run with:
// clang++ -Ofast --std=c++1y treePrint.cpp -o treePrint.o && ./treePrint.o

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <vector>
using std::vector;

// random double
double dRand(double dMin, double dMax)
{
    double d = (double)rand() / RAND_MAX;
    return dMin + d * (dMax - dMin);
}

double boardEval(string board) // dummy function for now
{
    return dRand(-1.0, 1.0);
}

struct Node
{
    Node(string newBoard)
    {
        board = newBoard;
        value = boardEval(newBoard);
    }
    
    string board = "";
    double value;
    vector<Node> childNodes;
    
    void draw(const string &tab = "", const bool last = true)
    {
        cout << tab;
        
        if(last)
            cout << "└";
        else
            cout << "├";
            
        cout << "─";
        
        if(childNodes.size() > 0)
            cout << "┬";
        else
            cout << "─";
        
        cout << "Board: " << board << " Value: " << value << endl;
        
        for(int i = 0; i < childNodes.size(); ++i)
        {
            string newTab;
            
            if(last)
                newTab="  ";
            else
                newTab="│ ";
            
            childNodes[i].draw(tab + newTab, i + 1 == childNodes.size());
        }
    }
};

void buildTree(Node &tree, int n)
{
    if (n < 1)
    {
        return;
    }
    
    // 3 = max number of children per node
    int children = 3;

    for(int i = 0; i < children; ++i)
    {
        Node child("rrrrrrrrrrrr________bbbbbbbbbbbb");
        
        tree.childNodes.push_back(child);
        
        buildTree(tree.childNodes[i], n-1);
    }
}
 
int main(void)
{
    srand(time(NULL));
    
    string treeDrawing = "";
    int depth = 0;
 
    Node root("rrrrrrrrrrrr________bbbbbbbbbbbb");
    
    // 4 = number of layers
    buildTree(root, 4);
    
    root.draw();
    
    return 0;
}