#pragma once
#include "Cost.h"
#include "share.h"
#include <iostream>
#include <vector>
#include <set>
using namespace std;

#define defN 6

namespace undirec {

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

class vertex {
  int number;
  bool visited;

  friend class direc::graph;
  friend class undirec::graph;

  public:
  vertex() : visited(false) {}
  vertex(int number)
  : number(number), visited(false) {}

  friend bool operator==(const vertex&, const vertex&);
  friend bool operator<(const vertex&, const vertex&);
  friend ostream& operator<<(ostream& o, const vertex&);
};

bool operator==(const vertex& v1, const vertex& v2)
{return v1.number == v2.number;}

bool operator<(const vertex& v1, const vertex& v2)
{return v1.number < v2.number;}

ostream& operator<<(ostream& o, const vertex& v){
  o << v.number;
  return o;
}

  class way_memory {
    cost** distance_memory;
    vertex*** path_memory;
    bool** allow_refer;
    int N;

    friend class graph;

public:
    way_memory(int N) : N(N) {
        nxnArrayAssign(distance_memory, 1 << N, N);
        nxnArrayAssign(allow_refer, 1 << N, N);
        /*
        for (int i = 0; i < (1 << N); ++i)
            for (int j = 0; j < N; ++j)
                distance_memory[i][j] = cost();
                */
        //어차피 new cost[]가 작동하면서 자동으로 빈 인자 생성자로 가게 되어있음

        for (int i = 0; i < (1 << N); ++i)
            for (int j = 0; j < N; ++j)
                allow_refer[i][j] = false;

        path_memory = new vertex** [1 << N];
        for (int i = 0; i < (1 << N); ++i) {
          path_memory[i] = new vertex* [N];
            for (int j = 0; j < N; ++j)
                path_memory[i][j] = new vertex[N - 1];
        }

        for (int i = 0; i < (1 << N); ++i)
            for (int j = 0; j < N; ++j)
                for (int k = 0; k < N - 1; ++k)
                    path_memory[i][j][k] = -1;
    }

    ~way_memory() {
        nxnArrayDelete(distance_memory, 1 << N);
        nxnArrayDelete(allow_refer, 1 << N);

        for (int i = 0; i < (1 << N); ++i) {
            for (int j = 0; j < N; ++j) {
                delete[] path_memory[i][j];
            }
          delete[] path_memory[i];
        }         

        delete[] path_memory;
    }
};

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
  if(e1.departure == e2.destination
  && e1.destination == e2.departure) {
    if(e1.weight == e2.weight) return 1;
    return -1;
  }

  return 0;
}


class graph {
  vector<edge> Edges;
  vector<vertex> Vertices;

  public:
  graph(){}
  graph(int capacity){Edges.reserve(capacity);}
  graph(int size, int **arrayGraph) {
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
    set<vertex> V;
    for(edge e : Edges) {
      V.insert(e.departure);
      V.insert(e.destination);
    }
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

      case noExist: {
      Edges.push_back(element);
      bool depar = true, desti = true;
      for(vector<vertex>::iterator itr = Vertices.begin();
      itr != Vertices.end(); ++itr) {
        if(*itr == element.departure) depar = false;
        if(*itr == element.destination) desti = false;
        if(!depar && !desti) break;
      }
      if(depar) Vertices.push_back(element.departure);
      if(desti) Vertices.push_back(element.destination);
      return *this;
      }
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

  cost findWeight(vertex v1, vertex v2) {
    return Edges[findIndex(edge(v1, v2, NULL))].weight;
  }

  cost findWeight_undirected(vertex v1, vertex v2) {
    int v1v2 = findIndex(edge(v1, v2, 0));
    if(v1v2 != -1)return Edges[v1v2].weight;
    
    int v2v1 = findIndex(edge(v2, v1, 0));
    if(v2v1 != -1)return Edges[v2v1].weight;

    return cost();
  }

  private:
  template <typename T>
  graph& setvectorizing(vector<T>& v, const set<T>& s) {
    for(typename set<T>::iterator itrS = s.begin();
        itrS != s.end(); ++itrS)
          v.push_back(*itrS);
      return *this;
  }

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
  }

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
    return Vertices.size();
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

  const vertex& vtxat(int index) {
    return Vertices.at(index);
  }

  int vertexindex(const vertex& v)
  {
    int N = Vertices.size();
    for(int i = 0; i < N; ++i)
      if(Vertices.at(i) == v) return i;
    
    return -1;
  }

  //include 한 edge 클래스가 방향 그래프로서 동작하고 있다면 아래 함수를 굳이 쓰지 않아도 됨
//그러나 edge 클래스를 무방향 그래프로서 동작하게 만들거나(Undirected Graph.h)
//주어지는 그래프가 무방향 그래프로서 입력된다면 아래 함수만 잘 동작함
void TSP_undirected(int current, way_memory& wm, int memorizebit) {
    int i;
    static bool firstexecute = true;
    static int start;
    static int vertices;

    if(firstexecute) {start = current; vertices = allVerticesCount(); firstexecute = false;}
    //start를 인자로 넘기기 싫어서 꼼수 씀.
    
    memorizebit += 1 << current;

    if (wm.allow_refer[memorizebit][current]) {
        /*int i = the_number_of_bit_is_on(memorizebit);
        for(; i < N - 1; ++i)
          wm.path_memory[memorizebit][current][i]*/
        return;
    }

    if (is_bit_all_on(memorizebit, vertices))
    {
        wm.allow_refer[memorizebit][current] = true;
        if (findWeight_undirected(vtxat(current), vtxat(start)) != cost()) {
            wm.distance_memory[memorizebit][current] = findWeight_undirected(vtxat(current), vtxat(start));
        }
        else
            wm.distance_memory[memorizebit][current] = cost(); //혹시 마지막에 길 없으면 탈락시키기 위해 큰 값
        wm.path_memory[memorizebit][current][vertices - 2] = vtxat(current);
        //생각해보니 무조건이네. 시작지점만 정해지면 이 [1111][각 지점]은 무조건
        //시작지점으로 가는 값임
        return;
    }

    for (i = 0; i < vertices; ++i) {
      cost iweight = findWeight_undirected(vtxat(current), vtxat(i));

        if (ivertex_had_not_been_visited(i, memorizebit)
            && iweight != cost()) {
            //ivertex has been visited;
            //returnvalue = TSP(i, W, wm, memorizebit);
            TSP_undirected(i, wm, memorizebit);

            if (wm.distance_memory[memorizebit][current] > iweight + wm.distance_memory[memorizebit + (1 << i)][i]) {
                wm.distance_memory[memorizebit][current]
                = iweight + wm.distance_memory[memorizebit + (1 << i)][i];

                int j;
                int index = the_number_of_bit_is_on(memorizebit) - 1;
                for (j = vertices - 2; j >= index; --j) {
                    wm.path_memory[memorizebit][current][j] = wm.path_memory[memorizebit + (1 << i)][i][j];
                }
                if(j >= 0)wm.path_memory[memorizebit][current][j] = vtxat(current);
            }
        }
    }

    wm.allow_refer[memorizebit][current] = true;

    return;
}

//이젠 순회가 아니므로 시작 지점이 중요함
void TSP_undirected_path(int current, way_memory& wm, int memorizebit) {
    int i;
    static bool firstexecute = true;
    static int vertices;
    memorizebit += 1 << current;

    if(firstexecute) {vertices = allVerticesCount(); firstexecute = false;}

    if (wm.allow_refer[memorizebit][current]) {
        /*int i = the_number_of_bit_is_on(memorizebit);
        for(; i < N - 1; ++i)
          wm.path_memory[memorizebit][current][i]*/
        return;
    }

    if (is_bit_all_on(memorizebit, vertices))
    {
        wm.allow_refer[memorizebit][current] = true;
        /*
        if (g.findWeight_undirected(current, start) != cost()) {
            wm.distance_memory[memorizebit][current] = g.findWeight_undirected(current, start);
        }
        else
            wm.distance_memory[memorizebit][current] = cost(); //혹시 마지막에 길 없으면 탈락시키기 위해 큰 값
        */
        //생각해보니 무조건이네. 시작지점만 정해지면 이 [1111][각 지점]은 무조건
        //시작지점으로 가는 값임
        wm.distance_memory[memorizebit][current] = 0;
        wm.path_memory[memorizebit][current][vertices - 2] = vtxat(current);
        return;
    }

    for (i = 0; i < vertices; ++i) {
      cost iweight = findWeight_undirected(vtxat(current), vtxat(i));
        if (ivertex_had_not_been_visited(i, memorizebit)
            && iweight != cost()) {
            //ivertex has been visited;
            //returnvalue = TSP(i, W, wm, memorizebit);
            TSP_undirected_path(i, wm, memorizebit);

            if (wm.distance_memory[memorizebit][current] > iweight + wm.distance_memory[memorizebit + (1 << i)][i]) {
                wm.distance_memory[memorizebit][current]
                = iweight + wm.distance_memory[memorizebit + (1 << i)][i];

                int j;
                int index = the_number_of_bit_is_on(memorizebit) - 1;

                for (j = vertices - 2; j >= index; --j) {
                    wm.path_memory[memorizebit][current][j] = wm.path_memory[memorizebit + (1 << i)][i][j];
                }
                if(j >= 0) wm.path_memory[memorizebit][current][j] = vtxat(current);
            }
        }
    }

    wm.allow_refer[memorizebit][current] = true;

    return;
}

void result_print(const way_memory& wm, int start, bool isCircuit, int N)
{
  int outmembit = 1 << start;

  cout << "minimum way : " << Vertices.at(start);
  for (int i = 0 ; i < N - 1; ++i)
    cout << " - " << wm.path_memory[outmembit][start][i];
  if(isCircuit) cout << " - " << Vertices.at(start);

  cout << endl << "minimum cost : " << wm.distance_memory[outmembit][start] << endl;
}

};

}