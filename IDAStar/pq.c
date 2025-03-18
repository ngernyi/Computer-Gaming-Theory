#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "pq.h"
#include "board.h"

priorityQueue* createPQ(int capacity){
    priorityQueue* pq = (priorityQueue*) malloc(sizeof(priorityQueue));
    pq->arr = (Node**) malloc(capacity * sizeof(Node));
    pq->size = 0;  
    pq->capacity = capacity;  

    return pq;  
}

void insertNode(priorityQueue* pq, Board board, int piece, int ini_pos, int end_pos, int step, int remaining, int piecesLeft, int weight, Node* parent){
    Node* newNode = createNode(board, piece, ini_pos, end_pos, step, remaining, piecesLeft, weight, parent);

    pq->arr[pq->size] = newNode;
    pq->size++;

    heapifyUp(pq, pq->size - 1);
}

void swap (Node** a, Node** b){
    Node* temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(priorityQueue* pq, int index){

    int parentIndex = (index - 1) / 2;

    while (index > 0 && pq->arr[index]->weight < pq->arr[parentIndex]->weight)
    {
        swap(&pq->arr[index], &pq->arr[parentIndex]);
        index = parentIndex;
        parentIndex = (index - 1) / 2;
        /* code */
    }
    
}

void heapifyDown(priorityQueue* pq, int index){
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // check if left is smaller
    if (left<pq->size && pq->arr[left]->weight < pq->arr[smallest]->weight)
    {
        smallest = left;
       
    }

    // check if right is smaller
    if (right<pq->size && pq->arr[right]->weight < pq->arr[smallest]->weight)
    {
        smallest = right;
        
    }

    if (smallest != index)
    {
        swap(&pq->arr[index],&pq->arr[smallest]);
        heapifyDown(pq,smallest);
        
    }
}

Node* extract(priorityQueue* pq){
    Node* temp = pq->arr[0];
    pq->arr[0] = pq->arr[pq->size - 1];
    pq->size -- ;
    heapifyDown(pq,0);
    return temp;
}

int PQisEmpty(priorityQueue* pq){
    if (pq->size != 0)
    {
        return 0;
        /* code */
    }
    return 1;
    
}

long PQsize(priorityQueue* pq){
    return pq->size;
}