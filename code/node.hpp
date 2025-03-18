#ifndef node_hpp
#define node_hpp
#include "board/board.hpp"
#include <vector>

struct node
{
    Board board; // board
    int ply; // the ply from parent to here
    int p_id; // parent id
    int c_id[36]; // children id
    // int depth; // depth
    int Nchild; // number of children
    int Ntotal; // total number of simulation
    // long double CsqrtlogN; // c*sqrt(log(Ntotal))
    // long double sqrtN; // sqrt(Ntotal)
    int wins; // number of wins
    long double WR; // win rate
    bool terminal;
    bool pruned;
    /* data */

    // constructor
    node();
    node(Board board, int ply, int p_id, int Nchild, int Ntotal, int wins,long double WR, bool terminal, bool pruned);

    // function to add children
    void addchild(int child_id);
    
    // Function to find the child node that is the succesor of current board situation
    int findCurrent(Board board);

    // Function to add new Node into nodeArray
    void addNode(int id);

    // Function to do evenSampling
    void initialSampling();

    // MCTS main function
    void MCTS();

    // selection
    // void Selection(int id);

    // Function to update after simulation
    void update(int deltaN, int deltaW);

    // Function to do backpropagation after each batch of simulation
    void backpropagation(int deltaN, int deltaW);

    // Function to get all the child into a list;
    void getChild(std::vector<int> &nodesInConsideration);

    // void progressivePruning();
};


#endif