#include "board/board.hpp"
#include "node.hpp"
#include "globals.hpp"
#include "math_lib/maths.hpp"

#include <cmath>
#include <vector>
#include <stdio.h>

#define evenSampling 5000 //10000
#define C  1.2 //1.41421
#define MAX_SIMULATION  10000000//64000000
#define BATCH_SIMULATION_NUM 10000
#define EPSILON 0.00001
#define TEMPERATUE 5

int TOTAL_SIMULATION_COUNT;

node::node(Board board, int ply, int p_id, int Nchild, int Ntotal, int wins, long double WR, bool terminal, bool pruned)
    : board(board), ply(ply), p_id(p_id),  Nchild(Nchild), Ntotal(Ntotal), wins(wins), WR(WR), terminal(terminal), pruned(pruned){
    
}

node::node(){

}


// add child to the node
void node::addchild (int child_id){
    // assign the current child index 
    this->c_id[this->Nchild] = child_id;



    // increase child number by 1
    this->Nchild++;
   
    
}

bool comparePiecePosition(int arr1[2][6], int arr2[2][6]){
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (arr1[i][j] != arr2[i][j])
            {
                return false;
                /* code */
            }
            
            /* code */
        }
        
        /* code */
    }
    return true;
}

// Function to find the child node that is the succesor of current board situation
int node::findCurrent(Board currentBoard){

    for (int i = 0; i < Nchild; i++)
    {
       
        /*
            Check
            1) piece position of child
            2) dice of child
            3) moving color of child
        */
        if (comparePiecePosition(nodeArray[c_id[i]].board.piece_position,currentBoard.piece_position) && nodeArray[c_id[i]].board.dice == currentBoard.dice && nodeArray[c_id[i]].board.moving_color == currentBoard.moving_color)
        {
            // if found, return the index of child in nodeArray
            return c_id[i];
        }
        
        
        /* code */
    }
    // not found in the child
    printf("not found %d\n ",Nchild);
    return c_id[0];
    
}

// Node Expansion ----------------------------------------------------------------------------------//

// Function to do even sampling
void node:: initialSampling(){
    int deltaW = 0;
    for (int i = 0; i < evenSampling; i++)
    {
        if (board.simulate() == true)
        {
            deltaW++;
            /* code */
        }
        /* code */
    }
    update(evenSampling,deltaW);
    backpropagation(evenSampling,deltaW);
    
}

// Function to add new node into nodeArray
void node::addNode(int id){

    // if the current node is already the leaf node, label and end the function
    if (this->board.check_winner() == true)
    {
        printf("end \n");
        return;
        /* code */
    }
    
    // generate moves
    this->board.generate_moves();

   for (int i = 0; i < this->board.move_count; i++)
   {
        Board child_copy = (this->board);
        child_copy.move(i);

        bool terminalNode = child_copy.check_winner();
        
        // create a new node
        nodeArray[nodeCounter] = node(child_copy,ply+1,id,0,0,0,0,terminalNode,false);

        // assign the node to its parent
        addchild(nodeCounter);

        // do even sampling once a node is expanded
        nodeArray[nodeCounter].initialSampling();

        // increase nodeCounter by 1
        nodeCounter++;

        
   }
}



void Selection(int id) {
    // If current simulation count exceeds maximum, stop
    if (TOTAL_SIMULATION_COUNT >= MAX_SIMULATION || nodeArray[currentNode].terminal == true) {
        // // printf("Stopping recursion. Total simulations: %d\n", TOTAL_SIMULATION_COUNT);
        // if (nodeArray[currentNode].terminal )
        // {
        //     printf("found %Lf\n",nodeArray[currentNode].WR);
        //     /* code */
        // }
        
        return;
    }

    // If the current node is terminal, backtrack to the root
    if (nodeArray[id].terminal) {
        Selection(currentNode);
    }

    // If the current node has no children, generate children
    if (nodeArray[id].Nchild == 0) {
        nodeArray[id].addNode(id);
        TOTAL_SIMULATION_COUNT += evenSampling * nodeArray[id].Nchild;
    }

    // Determine if the current node is terminal and handle child selection
    bool isTerminal = true;
    int child_selected = -1;
    // assume that the children are blue, want to find the minimum number
    float best_ucb = std::numeric_limits<float>::infinity();
    if (nodeArray[nodeArray[id].c_id[0]].board.moving_color == BLUE)
    {
        // find max, so best ucb set as negative 
        best_ucb = -std::numeric_limits<float>::infinity();
    }
    bool redwin = true;
    bool bluewin = true;
    for (int i = 0; i < nodeArray[id].Nchild; i++) {
        node& childNode = nodeArray[nodeArray[id].c_id[i]];

        // if the current id is red, then blue can choose
        if (childNode.board.moving_color == RED && childNode.WR == 1 )
        {
            redwin = false;
            // // no need to do simulation any more
            // nodeArray[id].WR = 1;
            // nodeArray[id].terminal = true;
            // printf("cut lose \n");
            // Selection(currentNode);
            /* code */
        }

         // if the current id is blue, then check whether the child has 0 or not
        if (childNode.board.moving_color == BLUE && childNode.WR == 0)
        {
            bluewin = false;
            
            /* code */
        }

        // Skip terminal children
        if (childNode.terminal) {
            continue;
        }

        // Non-terminal child found
        isTerminal = false;

        // Calculate UCB for child
        float child_ucb = fast_UCB(childNode.wins, childNode.Ntotal, TOTAL_SIMULATION_COUNT);

        // do temperature
        // Randomness between 0 and random_factor
        // float random_value = ((rand() % 1000) / 1000.0f) * 0.1;
        // child_ucb += random_value;

        // Selection logic depends on moving color
        if (nodeArray[nodeArray[id].c_id[0]].board.moving_color == RED) {
            // For minimizing player (RED), find lowest UCB
            if (child_ucb < best_ucb) {
                child_selected = i;
                best_ucb = child_ucb;
            }
        } else {
            // For maximizing player, find highest UCB
            if (child_ucb > best_ucb) {
                child_selected = i;
                best_ucb = child_ucb;
            }
        }
    }
    // printf("ucb %lf \n",best_ucb);
    if (!bluewin)
    {
        nodeArray[id].WR = 0;
        nodeArray[id].terminal = true;
        // printf("cut lose \n");
        /* code */
    }

    if (!redwin)
    {
        nodeArray[id].WR = 1;
        nodeArray[id].terminal = true;
        // printf("cut win \n");
        /* code */
    }
    

    // If all children are terminal, mark the node as terminal and backtrack
    if (isTerminal) {
        nodeArray[id].terminal = true;
        Selection(currentNode);
        return;
    }

    // if (id == currentNode )
    // {
    //     printf("child %d ucb %lf\n",nodeArray[id].c_id[child_selected],best_ucb);
    //     /* code */
    // }
    

    // Continue selection with the chosen child
    if (child_selected != -1) {
        Selection(nodeArray[id].c_id[child_selected]);
    } 
}


void node::MCTS(){
    TOTAL_SIMULATION_COUNT = 0;
    
        Selection(currentNode);

       
        
    
    
}

// update simulation ---------

// A function to update all details after simulation
void node::update(int deltaN, int deltaW){
    Ntotal += deltaN;
    // CsqrtlogN = C*sqrt(log((long double)Ntotal));
    // sqrtN = sqrt((long double) Ntotal);
    wins += deltaW;
    WR = (long double)wins / (long double)Ntotal;
}

// Back Propagation ----------------------------------------------------

// A function to do backpropagation after each simulation batch
void node::backpropagation(int deltaN, int deltaW){
    // find the parent id of the current node
    int parent_id = p_id;

    // loop until the node is the root node
    while (parent_id != currentNode) {
        // printf("Backpropagating from node: %d\n", parent_id);
        // printf("Parent node ID: %d\n", nodeArray[parent_id].p_id);

        // Update the parent node
        nodeArray[parent_id].update(deltaN, deltaW);

        // Update parent_id
        parent_id = nodeArray[parent_id].p_id;
    }
        
}


// A function to get all the child
void node::getChild(std::vector<int> &nodesInConsideration) {
    for (int i = 0; i < this->Nchild; i++) {
        // printf("Processing child %d of node %ld\n", c_id[i], this - nodeArray);

        // Validate `c_id[i]`
        if (c_id[i] < 0 || c_id[i] >= 60000) { // Assuming `nodeArray` has 600000 elements
            // printf("parent id %d children %d\n",p_id,this->Nchild);
            printf("Invalid child index: %d\n", c_id[i]);
            return;
        }

        // Push it into the list
        nodesInConsideration.push_back(this->c_id[i]);

        // Recursive call
        if (nodeArray[c_id[i]].Nchild != 0) {
            nodeArray[c_id[i]].getChild(nodesInConsideration);
        }
    }
}
