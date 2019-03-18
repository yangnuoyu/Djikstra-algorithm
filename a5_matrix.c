// Author: Nuoyu Yang  Date: Feb. 22, 2019.

#include "a5.h"


// Heap Functions:

int parent(int index) {
  return (index+1)/2 - 1;
}

int left(int index) {
  return 2 * index + 1;
}

int right(int index) {
  return 2 * index + 2;
}

void heapify(MinHeap* H, int index) {
  int l = left(index);
  int r = right(index);
  int smallest;
  if (l < H->size && H->heap[l]->weight < H->heap[index]->weight) {
    smallest = l;
  } else {
    smallest = index;
  }
  if (r < H->size && H->heap[r]->weight < H->heap[smallest]->weight) {
    smallest = r;
  }
  if (smallest != index) {
    Dis* temp = H->heap[index];
    H->heap[index] = H->heap[smallest];
    H->heap[smallest] = temp;
    heapify(H, smallest);
  }
}

void buildHeap(MinHeap* H) {
  for (int i = H->size/2-1; i >= 0; i--) {
    heapify(H, i);
  }
}

// Pops mininum element in heap
Dis* popMin(MinHeap* H) {
  Dis* max;
  if (H->size < 1) {
    printf("Heap is empty.");
    return NULL;
  } else {
    max = H->heap[0];
    H->heap[0] = H->heap[H->size - 1];
    H->heap[H->size - 1] = NULL;
    H->size--;
    heapify(H, 0);
    return max;
  }
}

// Push element in heap
void push(MinHeap* H, Dis* newDis) {
  if (H->size == MAX_HEAP_SIZE) {
    printf("Heap is full!");
    return;
  } else {
    H->size++;
    int i = H->size;
    while (i > 1 && H->heap[parent(i)]->weight < newDis->weight) {
      H->heap[i] = H->heap[parent(i)];
      i = parent(i);
    }
    H->heap[i] = newDis;
  }
}

// Creates a empty heap
MinHeap* createHeap() {
  MinHeap* H = (MinHeap*)malloc(sizeof(MinHeap));
  H->size = 0;
  return H;
}

// Destroy a heap
void destroyHeap(MinHeap* H) {
  for (int i = 0; i < H->size; i++) {
    free(H->heap[i]);
    H->heap[i] = NULL;
  }

  free(H);
  H = NULL;
}

// Djikstra Algorithm with Matrix:

// Initializes the heap
void initialize(MinHeap* H, int size, int start) {
  H->size = size;
  for (int i = 0; i < size; i++) {
    H->heap[i] = (Dis*) malloc(sizeof(Dis));
    H->heap[i]->node = i;
    H->heap[i]->weight = INFINITY;
    for (int j = 0; j < MATRIX_SIZE; j++) {
      H->heap[i]->path[j] = -1;
    }
  }
  H->heap[start]->weight = 0;
}

// Updates the distance between start and adjacent node of u
void relaxMatrix(Dis* u, Dis* v, double** graph) {
  if (graph[u->node][v->node] == -1) {
    return;
  } else if (v->weight > u->weight + graph[u->node][v->node]) {
    v->weight = u->weight + graph[u->node][v->node];
    for (int j = 0; j< MATRIX_SIZE; j++) {
      if (u->path[j] != -1) {
        v->path[j] = u->path[j];
      } else {
        v->path[j] = u->node;
        break;
      }
    }
  }
  return;
}

// Djikstra Algorithm
Dis* djikstraMatrix(int start, int end, double** graph, int size) {
  MinHeap* H = createHeap();
  initialize(H, size, start);
  buildHeap(H);
  while (H->size) {
    Dis* u = popMin(H);
    if (u->node == end) {
      destroyHeap(H);
      return u;
    } else {
      for (int i = 0; i < H->size; i++) {
        if (graph[u->node][H->heap[i]->node] > 0) {
          relaxMatrix(u, H->heap[i], graph);
        }
      }
      free(u);
      buildHeap(H);
    }
  }
}

// Helper Functions:

// Create an array to store the name of cities.
char** createCities() {
  char** cities = (char**) malloc(MATRIX_SIZE * sizeof(char*));
}

// Destroy the cities array.
void destoryCities(char** cities) {
  for (int i = 0; i< MATRIX_SIZE; i++) {
    free(cities[i]);
    cities[i] = NULL;
  }
  free(cities);
  cities = NULL;
}

// Creates an adjacency matrix
double** createGraphMatrix() {
  double** graph = (double**) malloc(MATRIX_SIZE * sizeof(double*));
  for (int i = 0; i< MATRIX_SIZE; i++) {
    graph[i] = (double*) malloc(MATRIX_SIZE * sizeof(double));
  }
  return graph;
}

// Destroy the adjancency matrix
void destroyGraphMatrix(double** graph) {
  for (int i = 0; i< MATRIX_SIZE; i++) {
    free(graph[i]);
  }
  free(graph);
  graph = NULL;
}

// Reads file and store the data in graph and cities array
void readFileMatrix(double** graph, char** cities) {
  FILE *cfPtr;
  char* pin = NULL;
  if ((cfPtr = fopen("miles_graph_FINAL.csv", "r")) == NULL) {
    printf("File could not be opened\n");
  } else {
    char c[MAX_LINE_SIZE];
    int row = 0;
    while (!feof(cfPtr)) {
      fgets(c, MAX_LINE_SIZE, cfPtr);
      pin = strtok(c, ",");
      int col = 0;
      if (row == 0) {
        while (pin) {
          if (col != 0) {
            cities[col-1] = strdup(pin);
          }
          pin = strtok(NULL, ",");
          col++;
        }
      } else {
        while (pin) {
          if (col != 0) {
            graph[row-1][col-1] = atof(pin);
          }
          pin = strtok(NULL, ",");
          col++;
        }
      }
      row++;
    }
    fclose(cfPtr);
  }
}

// Return the index of a city with city's name.
// Return -1 if there is no such city.
int getNode(char** cities, char* city) {
  int i = 0;
    for (; i < MATRIX_SIZE; i++) {
    if (strcmp(cities[i], city) == 0) {
      return i;
    }
  }
  return -1;
}


int main() {
  double** graph = createGraphMatrix();
  char** cities = createCities();
  Dis* result;
  int start;
  int end;
  int pathPin;
  readFileMatrix(graph, cities);

  start = getNode(cities, "Seattle_WA");
  end = getNode(cities, "Boston_MA");
  result =  djikstraMatrix(start, end, graph, MATRIX_SIZE);
  printf("Seattle to Boston: %lf miles.\n", result->weight);
  pathPin = 0;
  while (result->path[pathPin] != -1) {
    printf("%s -> ", cities[result->path[pathPin]]);
    pathPin++;
  }
  printf("%s\n", cities[result->node]);
  free(result);

  printf("\n");

  start = getNode(cities, "Minneapolis_MN");
  end = getNode(cities, "Ann Arbor_MI");
  result =  djikstraMatrix(start, end, graph, MATRIX_SIZE);
  printf("Minneapolis to Ann Arbor: %lf miles.\n", result->weight);
  pathPin = 0;
  while (result->path[pathPin] != -1) {
    printf("%s -> ", cities[result->path[pathPin]]);
    pathPin++;
  }
  printf("%s\n", cities[result->node]);
  free(result);

  destroyGraphMatrix(graph);
  destoryCities(cities);

  printf("\n\n");
  return 0;
}

