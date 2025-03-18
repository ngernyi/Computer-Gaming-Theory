#ifndef PQ_H
#define PQ_H

#include "node.h"
#include "board.h"

typedef struct priorityQueue
{
    Node** arr;
    int size;
    int capacity;
    /* data */
}priorityQueue;

priorityQueue* createPQ(int capacity);
void insertNode(priorityQueue* pq, Board board, int piece, int ini_pos, int end_pos, int step, int remaining, int piecesLeft,int weight, Node *parent); 
void heapifyUp(priorityQueue* pq, int index);
void heapifyDown(priorityQueue* pq, int index);
void swap(Node** a, Node** b);
Node* extract(priorityQueue* pq);
int PQisEmpty(priorityQueue* pq);
long PQsize(priorityQueue* pq);

#endif