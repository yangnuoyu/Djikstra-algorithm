// Author: Nuoyu Yang  Date: Feb. 22, 2019.

#ifndef A5_H
#define A5_H

#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEAP_SIZE 2000
#define INFINITY DBL_MAX
#define MAX_LINE_SIZE 20000
#define MAX_CITY_NAME 100
#define MATRIX_SIZE 1000


// Record in Heap

struct dis {
  int node;
  double weight;
  int path[MATRIX_SIZE];
};

typedef struct dis Dis;


// Heap

struct minHeap {
  int size;
  Dis* heap[MAX_HEAP_SIZE];
};

typedef struct minHeap MinHeap;


// Node in Graph

struct node {
  int vertex;
  int weight;
  struct node* next;
};

typedef struct node Node;


// heap functions

int left(int index);

int right(int index);

void heapify(MinHeap* H, int index);

void buildHeap(MinHeap* H);

Dis* popMin(MinHeap* H);

void push(MinHeap* H, Dis* newDis);

MinHeap* createHeap();

void initialize(MinHeap* H, int size, int start);

void destroyHeap(MinHeap* H);


// Helper Function

int getNode(char** cities, char* city);


// Djikstra Matrix Functions

char** createCities();

void DestoryCities(char** cities);

double** createGraphMatrix();

void destroyGraphMatrix(double** graph);

void readFileMatrix(double** graph, char** cities);

void relaxMatrix(Dis* u, Dis* v, double** graph);

Dis* djikstraMatrix(int start, int end, double** graph, int size);


// Djikstra List Functions

Node** createGraphList();

void destroyGraphList(Node** graph);

void readFileList(Node** graph, char** cities);

int adjacentDisList(Node* pin, MinHeap* H);

void relaxList(Dis* u, Node* pin, MinHeap* H);

Dis* djikstraList(int start, int end, Node** graph, int size);

#endif
