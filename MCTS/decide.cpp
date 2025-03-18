// These are not basic function for board.
// write them in separate file makes code cleaner.
// I recommend write all your complicated algorithms in this file.
// Or sort them into even more files to make code even cleaner!

#include "board/board.hpp"
#include "math_lib/maths.hpp"
#include "node.hpp"
#include <stdio.h>
#include <random>
#include "globals.hpp"

#define SIMULATION_BATCH_NUM 1000
#define MAX_SIMULATION_COUNT 6000000

// =========================================================
// TA provided Template
// ============================================================

// MCS UCB version
// I use max sample number as the simulation limit.
// Recommend: Using wall clock time to precisely limit the time.
// Advanced: Using both max sample number and wall clock time to limit the simiulation time.

// int MCS_UCB_argmax(Board child_nodes[],int move_num)
// {
//     // you can change the parameter C of the UCB here
//     // if you comment out this line, the default value will be 1.41421
//     // ucb_param_C = 1.41421

//     unsigned int node_sample_num[64] = {};
//     unsigned int win_num[64] = {};
//     unsigned int total_sample_num = 0;
//     // start simulating
//     // Prevent 0 appears in Denominator, do a even sampling first
//     for (int i = 0; i < move_num; i++)
//     {
//         for (int j = 0; j < SIMULATION_BATCH_NUM; j++)
//         {
//             if (child_nodes[i].simulate() == true)
//             {
//                 win_num[i] += 1;
//             }
//             node_sample_num[i] += 1;
//             total_sample_num += 1;
//         }
//     }
//     // Then do MCS UCB
//     while (total_sample_num < MAX_SIMULATION_COUNT)
//     {
//         // find the child which has the highest UCB
//         int argmax = 0;
//         float max_UCB = -1;
//         for (int i = 0; i < move_num; i++)
//         {
//             float child_UCB = fast_UCB(win_num[i], node_sample_num[i], total_sample_num);
//             if (child_UCB > max_UCB)
//             {
//                 max_UCB = child_UCB;
//                 argmax = i;
//             }
//         }
//         // do simulation on child[argmax]
//         for (int j = 0; j < SIMULATION_BATCH_NUM; j++)
//         {
//             if (child_nodes[argmax].simulate() == true)
//             {
//                 win_num[argmax] += 1;
//             }
//             node_sample_num[argmax] += 1;
//             total_sample_num += 1;
//         }
//     }
//     // Then return best step according to the win rate
//     // NOT UCB! NOT UCB! NOT UCB!
//     int return_argmax = 0;
//     float max_WR = -1;
//     for (int i = 0; i < move_num; i++)
    
//     {
//         float child_WR = (float)win_num[i]/(float)node_sample_num[i];
//         if (child_WR > max_WR)
//         {
//             max_WR = child_WR;
//             return_argmax = i;
//         }
//     }
//     // printf("index %d",return_argmax);
//     return return_argmax;
// }


// ========================================================
// My Code
// ========================================================

int Board::decide(){

    // if the dice number piece does not exist on the board
    // printf("piece position %d \n",this->piece_position[(int)this->moving_color][(int)this->dice]);
    
    
    // if there is no node exist before this, create a new node in nodeArray
    if (nodeCounter == 0)
    {
        nodeArray[nodeCounter] = node(*(this), 0, -1, 0, 0,0,0,false,false);
        
        currentNode = 0;
        nodeCounter++;

        
        // expand the first node
        nodeArray[currentNode].addNode(0);
        
    }
    // if there are nodes expanded already
    else{
        // find the index of node for current board position
        currentNode = nodeArray[currentNode].findCurrent(*(this));
    }
    // printf("h \n");

    // do MCTS 
    nodeArray[currentNode].MCTS();

    
    // return the nodes with the highest win rate
    float highestWR = 0;
    int index = 0;
    int temp_currentNode = 0;
    for (int i = 0; i < nodeArray[currentNode].Nchild; i++)
    {
        if (nodeArray[nodeArray[currentNode].c_id[i]].WR > highestWR)
        {
            highestWR = nodeArray[nodeArray[currentNode].c_id[i]].WR;
            index = i;
            temp_currentNode = nodeArray[currentNode].c_id[i];
        }
        
    }


    // return the node with highest win rate
    currentNode = temp_currentNode;
    if (nodeArray[currentNode].Nchild == 0)
    {
        nodeArray[currentNode].addNode(currentNode);
    }
    


    return index;

}


int Board ::first_move_decide_dice(){
    

    // make the node Counter as 0 to add the first node
    nodeCounter = 0;

    // set the current node as parent
    currentNode = nodeCounter;

    // add the initial board to the nodeArray
    Board copy_board = *(this);
    
    nodeArray[nodeCounter] = node(copy_board, 0, -1, 0, 0,0,0,false,false);
    
    nodeCounter++;
    // add all the selection of dice as the children of initial board
    for (int i = 0; i < PIECE_NUM; i++)
    {
        // set move for the child node
        Board board_copy = *(this);
        board_copy.dice = i;
        board_copy.moving_color ^= 1;

        // create new node and assign them into the nodeArray
        nodeArray[nodeCounter] = node(board_copy,0,0,0,0,0,0,false,false);

        // set the newly created node as the child of parent
        nodeArray[currentNode].addchild(nodeCounter);

        // do initial sampling for each child
        nodeArray[nodeCounter].initialSampling();

        // update nodeCounter
        nodeCounter++;
        
    }
    // do MCTS
    nodeArray[currentNode].MCTS();


    // determine the highest WR node
    float WR = 0;
    int index = 0;
    for (int i = 0; i < nodeArray[0].Nchild; i++)
    {

        // if the WR is higher, modify the index
        if (nodeArray[nodeArray[0].c_id[i]].WR > WR)
        {
            WR = nodeArray[nodeArray[0].c_id[i]].WR;
            index = i;
            currentNode = nodeArray[0].c_id[i];
        }
        
    }

    // the selected node needs to be expanded if it is not
    if (nodeArray[currentNode].Nchild == 0)
    {
        nodeArray[currentNode].addNode(currentNode);
    }

    
    // return the index of step
    return index;
}

// ===============================================
// TA provided template
// ==============================================

// Only used in first move
// int Board::first_move_decide_dice()
// {
//     // A nice example for expansion
//     // quick and elegant!
//     Board child_nodes[PIECE_NUM];
//     for (int i = 0; i < PIECE_NUM; i++)
//     {
//         Board board_copy = *(this);
//         board_copy.dice=i;
//         child_nodes[i] = board_copy;
//     }
//     return MCS_UCB_argmax(child_nodes,PIECE_NUM);
// }
// You should use mersenne twister and a random_device seed for the simulation
// But no worries, I've done it for you. Hope it can save you some time!
// Call random_num()%num to randomly pick number from 0 to num-1
std::mt19937 random_num(std::random_device{}());

// Very fast and clean simulation!
bool Board::simulate()
{
    Board board_copy = *(this);
    // run until game ends.
    while (!board_copy.check_winner())
    {
        board_copy.generate_moves();
        if (board_copy.move_count == 0) {
            print_board();
            exit(EXIT_FAILURE); 
            return false; // Default to a loss or other logic
        }
        board_copy.move(random_num() % board_copy.move_count);
    }
    // game ends! find winner.
    // Win!
    if (board_copy.moving_color == BLUE)
        return true;
    // Lose!
    else
        return false;
}