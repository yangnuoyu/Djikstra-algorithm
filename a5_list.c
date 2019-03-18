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

// Djikstra Algorithm with adjacency list:

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

// Returns the position of vertex which the note represents in heap.
int adjacentDisList(Node* pin, MinHeap* H) {
  int i = 0;
  while (i < H->size) {
    if (H->heap[i]->node == pin->vertex) {
      return i;
    }
    i++;
  }
  return -1;
}

// Updates the distance between start and adjacent node of u
void relaxList(Dis* u, Node* pin, MinHeap* H) {
  int adj = adjacentDisList(pin, H);
  if (adj == -1) {
    return;
  } else if (H->heap[adj]->weight > u->weight + pin->weight) {
    H->heap[adj]->weight = u->weight + pin->weight;
    for (int j = 0; j< MATRIX_SIZE; j++) {
      if (u->path[j] != -1) {
        H->heap[adj]->path[j] = u->path[j];
      } else {
        H->heap[adj]->path[j] = u->node;
       break;
      }
    }
  }
  return;
}

// Djikstra Algorithm
Dis* djikstraList(int start, int end, Node** graph, int size) {
  MinHeap* H = createHeap();
  initialize(H, size, start);
  buildHeap(H);
  while (H->size) {
    Dis* u = popMin(H);
    if (u->node == end) {
      destroyHeap(H);
      return u;
    } else {
      Node* pin = graph[u->node];
      while (pin != NULL) {
        relaxList(u, pin, H);
        pin = pin->next;
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

// Creates an adjacency list
Node** createGraphList() {
  Node** graph = (Node**)malloc(MATRIX_SIZE * sizeof(Node*));
  for (int i = 0; i< MATRIX_SIZE; i++) {
    graph[i] = NULL;
  }
  return graph;
}

// Destroy the adjancency matrix
void destroyGraphList(Node** graph) {
  for (int i = 0; i < MATRIX_SIZE; i++) {
    Node* pin = graph[i];
    while (pin) {
      graph[i] = pin->next;
      free(pin);
      pin = graph[i];
    }
  }
  free(graph);
  graph = NULL;
}

// Reads file and store the data in cities array
void readFileNode(char** cities) {
  FILE *cfPtr;
  char* pin = NULL;
  if ((cfPtr = fopen("nodes.csv", "r")) == NULL) {
    printf("File could not be opened\n");
  } else {
    int row = 0;
    char c[MAX_LINE_SIZE];
    while (!feof(cfPtr)) {
      fgets(c, MAX_LINE_SIZE, cfPtr);
      if (row != 0) {
        if (c[strlen(c)-1] == '\n') {
          c[strlen(c)-1] = '\0';
        }
        pin = strtok(c, ",");
        pin = strtok(NULL, ",");
        cities[row-1] = strdup(pin);
      }
      row++;
    }
    fclose(cfPtr);
  }
}

// Reads file and store the data in graph
void readFileEdge(Node** graph, char** cities) {
  FILE *cfPtr;
  char* pin = NULL;
  char source[MAX_CITY_NAME];
  char dest[MAX_CITY_NAME];
  int hours;
  int mins;
  int seconds;

  int currentNode = 0;

  if ((cfPtr = fopen("edges_FINAL.csv", "r")) == NULL) {
    printf("File could not be opened\n");
  } else {
    int adjNode = 0;
    while (!feof(cfPtr)) {
      fscanf(cfPtr, "%s %s %d %d %d", source, dest, &hours, &mins, &seconds);
      while (strcmp(cities[currentNode], source) != 0) {
        currentNode += 1;
        adjNode = 0;
      }
      Node* nodePin = (Node*) malloc(sizeof(Node));
      while (strcmp(cities[adjNode], dest) != 0) {
        adjNode += 1;
      }
      nodePin->vertex = adjNode;
      nodePin->weight = hours * 3600 + mins * 60 + seconds;
      nodePin->next = graph[currentNode];
      graph[currentNode] = nodePin;
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
  Node** graph = createGraphList();
  char** cities = createCities();
  int pathPin;
  int seconds;
  int minutes;
  int hours;
  Dis* result;
  int start;
  int end;
  readFileNode(cities);
  readFileEdge(graph, cities);


  start = getNode(cities, "Seattle_WA");
  end = getNode(cities, "Boston_MA");
  result = djikstraList(start, end, graph, MATRIX_SIZE);
  seconds = (int)(result->weight+ 0.5);
  hours = seconds / 3600;
  seconds %= 3600;
  minutes = seconds / 60;
  seconds %= 60;
  printf("Seattle to Boston: %dh %dm %ds\n", hours, minutes, seconds);
  pathPin = 0;
  while (result->path[pathPin] != -1) {
    printf("%s -> ", cities[result->path[pathPin]]);
    pathPin++;
  }
  printf("%s\n", cities[result->node]);
  free(result);


  printf("\n");


  start = getNode(cities, "Minneapolis_MN");
  end = getNode(cities, "Ann_Arbor_MI");
  result = djikstraList(start, end, graph, MATRIX_SIZE);
  seconds = (int)(result->weight+ 0.5);
  hours = seconds / 3600;
  seconds %= 3600;
  minutes = seconds / 60;
  seconds %= 60;
  printf("Minneapolis to Ann Arbor: %dh %dm %ds\n", hours, minutes, seconds);
  pathPin = 0;
  while (result->path[pathPin] != -1) {
    printf("%s -> ", cities[result->path[pathPin]]);
    pathPin++;
  }
  printf("%s\n", cities[result->node]);
  free(result);


  destroyGraphList(graph);
  destoryCities(cities);

  printf("\n\n");
  return 0;
}

