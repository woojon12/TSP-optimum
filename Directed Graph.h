#pragma once
#include "Cost.h"
#include <iostream>
#include <vector>
#include <set>
using namespace std;

#define defN 4

template <typename T>
void nxnArrayAssign(T**& arr, int row, int col)
{
    arr = new T * [row];

    for (int i = 0; i < row; ++i)
        arr[i] = new T[col];
}

template <typename T>
void nxnArrayDelete(T**& arr, int row)
{
    for (int i = 0; i < row; ++i)
        delete[] arr[i];

    delete[] arr;
}

class way_memory {
    cost** distance_memory;
    int*** path_memory;
    bool** allow_refer;
    int N;

    friend void TSP(int current, int W[][defN], way_memory& M, int memorizebit);
    friend void TSP_directed(int current, graph& g, way_memory& M, int memorizebit);
    friend void TSP_undirected(int current, graph& g, way_memory& wm, int memorizebit);
    friend int main();

public:
    way_memory(int N) : N(N) {
        nxnArrayAssign(distance_memory, 1 << N, N);
        nxnArrayAssign(allow_refer, 1 << N, N);

        for (int i = 0; i < (1 << N); ++i)
            for (int j = 0; j < N; ++j)
                distance_memory[i][j] = cost();

        for (int i = 0; i < (1 << N); ++i)
            for (int j = 0; j < N; ++j)
                allow_refer[i][j] = false;

        path_memory = new int** [1 << N];
        for (int i = 0; i < (1 << N); ++i)
            path_memory[i] = new int* [N];
        for (int i = 0; i < (1 << N); ++i)
            for (int j = 0; j < N; ++j)
                path_memory[i][j] = new int[N - 1];

        for (int i = 0; i < (1 << N); ++i)
            for (int j = 0; j < N; ++j)
                for (int k = 0; k < N - 1; ++k)
                    path_memory[i][j][k] = -1;
    }

    way_memory(const way_memory& w) {
        nxnArrayAssign(distance_memory, 1 << N, N);
        nxnArrayAssign(allow_refer, 1 << N, N);

        for (int i = 0; i < (1 << N); ++i)
            for (int j = 0; j < N; ++j)
                distance_memory[i][j] = w.distance_memory[i][j];

        for (int i = 0; i < (1 << N); ++i)
            for (int j = 0; j < N; ++j)
                allow_refer[i][j] = w.allow_refer[i][j];

        path_memory = new int** [1 << N];
        for (int i = 0; i < (1 << N); ++i)
            path_memory[i] = new int* [N];
        for (int i = 0; i < (1 << N); ++i)
            for (int j = 0; j < N; ++j)
                path_memory[i][j] = new int[N - 1];

        for (int i = 0; i < (1 << N); ++i)
            for (int j = 0; j < N; ++j)
                for (int k = 0; k < N - 1; ++k)
                    path_memory[i][j][k] = w.path_memory[i][j][k];
    }

    ~way_memory() {
        nxnArrayDelete(distance_memory, 1 << N);
        nxnArrayDelete(allow_refer, 1 << N);

        for (int i = 0; i < (1 << N); ++i)
            for (int j = 1; j < N; ++j) //왠지 모르겠는데 여기 j를 0이 아니고 1이라고 해야 런타임 오류가 안남
                delete[] path_memory[i][j];

        for (int i = 0; i < (1 << N); ++i)
            delete[] path_memory[i];

        delete[] path_memory;
    }
};

class vertex {
  int number;
  bool visited;

  friend class graph;

  public:
  vertex() : visited(false) {}
  vertex(int number)
  : number(number), visited(false) {}

  friend bool operator==(const vertex&, const vertex&);
  friend bool operator<(const vertex&, const vertex&);
};

bool operator==(const vertex& v1, const vertex& v2)
{return v1.number == v2.number;}

bool operator<(const vertex& v1, const vertex& v2)
{return v1.number < v2.number;}

class edge {
  cost weight;
  vertex departure, destination;

  friend class graph;

  bool isVertexSame(edge e) {
    return departure == e.departure
    && destination == e.destination;
  }

public:
  edge() : weight() {}
  edge(vertex departure, vertex destination, cost weight)
  : departure(departure), destination(destination), weight(weight)
  {}

  friend int operator==(edge, edge);

  edge reverse() { //임시로 만들긴 했는데 한 번도 안 쓰인 메서드
    return edge(destination, departure, weight);
  }
};

int operator==(edge e1, edge e2)
{
  if(e1.departure == e2.departure
  && e1.destination == e2.destination) {
    if(e1.weight == e2.weight) return 1;
    return -1;
  }

  return 0;
}


class graph {
  vector<edge> Edges;

  public:
  graph(){}
  graph(int capacity){Edges.reserve(capacity);}
  graph(int size, int arrayGraph[][defN]) {
      Edges.reserve(size * (size - 1));

      //들어오는 배열이 무조건 정사각형이라 상정한다.
      //동적 배열의 메모리 크기를 아는 함수 malloc.h 의 _msize 함수가 있는데
      //어째선지 여기선 동작하지 않음.

      for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) {
          if(i != j) push(edge(i, j, arrayGraph[i][j]));
        }
      }
  }
  graph(vector<edge> Edges) {
    this->Edges = Edges;
  }

  graph& push(edge element) {
    enum {onlyCostDiffrent = -1, noExist = 0, alreadyExist = 1};

    switch(alreadyExistORonlyCostDiff(element)) {
      case alreadyExist:
      return *this;

      case onlyCostDiffrent: {
        int i = findIndex(element);
        Edges[i].weight = element.weight;
      return *this;
      }

      case noExist:
      Edges.push_back(element);
      return *this;

      default:
      cout << "What happended" << endl;
      return *this;
    }
  }

  int findIndex(edge element) {
    int index = 0;

    for (edge e : Edges) {
      if(e.isVertexSame(element)) return index;
      ++index;
    }

    return -1; //혹시 없으면 -1 반환
  }

  int findWeight(vertex v1, vertex v2) {
    return Edges[findIndex(edge(v1, v2, NULL))].weight.weight;
  }

  int findWeight_undirected(vertex v1, vertex v2) {
    int v1v2 = findIndex(edge(v1, v2, 0));
    if(v1v2 != -1)return Edges[v1v2].weight.weight;
    
    int v2v1 = findIndex(edge(v2, v1, 0));
    if(v2v1 != -1)return Edges[v2v1].weight.weight;

    return -1;
  }

  private:
  int alreadyExistORonlyCostDiff(edge element) {
    for(edge e : Edges)
      if (e == element) return e == element;
    
    return 0;
  }

  public:
  graph MST() { //Minimum Spanning Tree
    graph exploreGraph;
    graph resultGraph;
    set<vertex> visitedVertices;
    
    visitedVertices.insert(this->Edges[0].departure);
    //탐색 시작점은 그냥 대충 정하기
    
    int theNumberOfVertices = allVerticesCount();

    while(resultGraph.Edges.size() < theNumberOfVertices - 1) {
      edge minEdge;
      exploreGraph.Edges = 
      this->theEdgesIncludingTheseVertices(visitedVertices);

      subtractGraph(exploreGraph, resultGraph);

      for(edge e : exploreGraph.Edges) {
        if(!(isBothEndsVertices(e, visitedVertices))
        && minEdge.weight > e.weight)
          minEdge = e;
      }

      visitedVertices.insert(minEdge.departure);
      visitedVertices.insert(minEdge.destination);
      resultGraph.push(minEdge);
    }

    return resultGraph;
  } //내가 준 그래프가 완전 그래프가 아니어서 무한루프 걸리는 듯 하다.

  vector<edge> theEdgesIncludingTheseVertices(set<vertex> V)
  {
    vector<edge> resultEdges;

    for(edge e : Edges) {
      for(vertex v : V) {
        if(e.departure == v || e.destination == v)
          resultEdges.push_back(e);
      }
    }

    return resultEdges;
  }

  void subtractGraph(graph& exploreGraph, graph& resultGraph) {
    for(vector<edge>::iterator itr = exploreGraph.Edges.begin();
      itr != exploreGraph.Edges.begin(); ++itr) {
        vector<edge>::iterator itrR;
        for(itrR = resultGraph.Edges.begin();
        itrR != resultGraph.Edges.end(); ++itrR) {
          if(*itr == *itrR) {
            exploreGraph.Edges.erase(itr);
            break;
          }
        }
      if(itrR != resultGraph.Edges.end()) break;
    }
  }

  int allVerticesCount()
  {
    //set 사용법 정리 : https://blockdmask.tistory.com/79
    set<vertex> V;

    for (edge e : Edges) {
      V.insert(e.departure);
      V.insert(e.destination);
    }

    return V.size();
  }

  bool isBothEndsVertices(edge e, set<vertex> V) {
    if(V.find(e.departure) != V.end()
    && V.find(e.destination) != V.end()) return true;
    return false;
  }

  void showEdges() {
    for(edge e : Edges) {
      cout << "(" << e.departure.number << ", " << e.destination.number << ") - "
      << e.weight.weight << endl;
    }
  }

  void showTheEdge(int index) {
    cout << "(" << Edges[index].departure.number << ", " << Edges[index].destination.number << ") - " << Edges[index].weight.weight << endl;
  }
};