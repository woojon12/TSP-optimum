#include <iostream>
#include "Directed Graph.h"
using namespace std;

/*
bool is_numberth_bit_on(int num, unsigned int bit)
{
  num >>= bit - 1;
  if(num % 2 == 1) return true;
  return false;
}
*/

int pow(int num, int p)
{
    int temp = 1;
    for (int i = 0; i < p; ++i) temp *= num;

    return temp;
}

/* 초반 수도코드
int TSP() {
  int i;
  int returnvalue;
  for(i = 0; i < TNOvertex; ++i) {
    if( ivertex had not been visited
      && W[current][i] != 0) {
      ivertex has been visited; 0b0001;
      returnvalue = TSP();
      if(cm[0b1111][current] > W[current][i] + returnvalue)
      cm[0b1111][current] = W[current][i] + returnvalue;
      return W[current][i] + returnvalue;
    }
  }
}
*/

int N;


bool ivertex_had_not_been_visited(int& i, int& memorizebit)
{
    if (memorizebit & (1 << i)) return false;
    return true;
}

int the_number_of_bit_is_on(const int& memorizebit) {
    int result = 0;
    for (int k = 0; memorizebit >= (1 << k); ++k)
        if (memorizebit & (1 << k)) ++result;
    return result;
}

bool is_bit_all_on(int memorizebit)
{
    return memorizebit == (1 << N) - 1;
}

#define start 0 //지금은 순회라서 어디서 시작하든 무상관
void TSP(int current, int W[][defN], way_memory& wm, int memorizebit) {
    int i;

    memorizebit += 1 << current;

    if (wm.allow_refer[memorizebit][current]) {
        /*int i = the_number_of_bit_is_on(memorizebit);
        for(; i < N - 1; ++i)
          wm.path_memory[memorizebit][current][i]*/
        return;
    }

    if (is_bit_all_on(memorizebit))
    {
        wm.allow_refer[memorizebit][current] = true;
        if (W[current][start] != 0) {
            wm.distance_memory[memorizebit][current] = W[current][start];
        }
        else
            wm.distance_memory[memorizebit][current] = 1000000; //혹시 마지막에 길 없으면 탈락시키기 위해 큰 값
        wm.path_memory[memorizebit][current][N - 2] = current;
        //생각해보니 무조건이네. 시작지점만 정해지면 이 [1111][각 지점]은 무조건
        //시작지점으로 가는 값임
        return;
    }

    for (i = 0; i < N; ++i) {
        if (ivertex_had_not_been_visited(i, memorizebit)
            && W[current][i] != 0) {
            //ivertex has been visited;
            //returnvalue = TSP(i, W, wm, memorizebit);
            TSP(i, W, wm, memorizebit);

            if (wm.distance_memory[memorizebit][current] > W[current][i] + wm.distance_memory[memorizebit + (1 << i)][i]
                || wm.distance_memory[memorizebit][current] == 0) {
                wm.distance_memory[memorizebit][current]
                    = W[current][i] + wm.distance_memory[memorizebit + (1 << i)][i];

                int j;
                int index = the_number_of_bit_is_on(memorizebit) - 1;
                for (j = N - 2; j >= index; --j) {
                    wm.path_memory[memorizebit][current][j] = wm.path_memory[memorizebit + (1 << i)][i][j];
                }
                wm.path_memory[memorizebit][current][j] = current;
            }
        }
    }

    wm.allow_refer[memorizebit][current] = true;

    return;
}

/*
재귀함수는 if + return와 for로 구성.
for은 꼭 있어도 되는 건 아니며
if는 for 위에도 아래에도 있을 수 있음.
알고리즘에 따라 if는 두 개 이상이 될 수 있음.
그리고 재귀함수가 더 깊숙히 들어가면서 답이 쌓이고 쌓여
마지막 재귀함수에서 답을 저장하는 경우가 있는가하면
(이 경우 최종결과와 관련된 변수를 인자로 계속 넘기고
재귀함수의 각 끝에서 더 정답에 가까운 걸 계속 그 변수에 덮어씌움)
최고로 깊숙히 재귀함수 들어갔다가 다시 되돌아오면서
반환하는 값이 쌓이고 쌓여 마지막 반환값이 답이 되는 걸로도 만들 수 있다.
(이 경우 최종결과와 관련된 변수를 반환하는 식인데
아랫재귀함수를 호출한 윗재귀함수 입장에서 봤을 때
아랫놈이 이미 각자의 끝에서 최선의 결과가 반환되도록 하고
자기는 그걸 적용한 최선의 값을 또 반환하는 식. 이게 계속되면 마지막 반환이 정답.)
여태까지 둘 중 하나로만 만들어야 되는 경우는 나는 딱히 못 봄.
후자는 딱히 정답을 저장할 인스턴스를 만들지 않아도 된다. 별로 장점은 아니지만,
그냥 즉석에서 cout << 함수() << endl; 이렇게 최종결과 출력해도 된다는 거.
위 함수의 경우엔 전자임.
*/

void TSP_directed(int current, graph& g, way_memory& wm, int memorizebit) {
    int i;

    memorizebit += 1 << current;

    if (wm.allow_refer[memorizebit][current]) {
        /*int i = the_number_of_bit_is_on(memorizebit);
        for(; i < N - 1; ++i)
          wm.path_memory[memorizebit][current][i]*/
        return;
    }

    if (is_bit_all_on(memorizebit))
    {
        wm.allow_refer[memorizebit][current] = true;
        if (g.findWeight(current, start) != cost()) {
            wm.distance_memory[memorizebit][current] = g.findWeight(current, start);
        }
        else
            wm.distance_memory[memorizebit][current] = cost(); //혹시 마지막에 길 없으면 탈락시키기 위해 큰 값
        wm.path_memory[memorizebit][current][N - 2] = current;
        //생각해보니 무조건이네. 시작지점만 정해지면 이 [1111][각 지점]은 무조건
        //시작지점으로 가는 값임
        return;
    }

    for (i = 0; i < N; ++i) {
        if (ivertex_had_not_been_visited(i, memorizebit)
            && g.findWeight(current, i) != cost()) {
            //ivertex has been visited;
            //returnvalue = TSP(i, W, wm, memorizebit);
            TSP_directed(i, g, wm, memorizebit);

            if (wm.distance_memory[memorizebit][current] > g.findWeight(current, i) + wm.distance_memory[memorizebit + (1 << i)][i]) {
                wm.distance_memory[memorizebit][current]
                = g.findWeight(current, i) + wm.distance_memory[memorizebit + (1 << i)][i];

                int j;
                int index = the_number_of_bit_is_on(memorizebit) - 1;
                for (j = N - 2; j >= index; --j) {
                    wm.path_memory[memorizebit][current][j] = wm.path_memory[memorizebit + (1 << i)][i][j];
                }
                wm.path_memory[memorizebit][current][j] = current;
            }
        }
    }

    wm.allow_refer[memorizebit][current] = true;

    return;
}

//아래 함수는 현재 include 한 edge 클래스가 방향 그래프로서 동작하고 있어 구분의 의미가 없음.
//edge 클래스를 무방향 그래프로서 동작하게 만든다면 아래 함수만 잘 동작할 것으로 예상
void TSP_undirected(int current, graph& g, way_memory& wm, int memorizebit) {
    int i;

    memorizebit += 1 << current;

    if (wm.allow_refer[memorizebit][current]) {
        /*int i = the_number_of_bit_is_on(memorizebit);
        for(; i < N - 1; ++i)
          wm.path_memory[memorizebit][current][i]*/
        return;
    }

    if (is_bit_all_on(memorizebit))
    {
        wm.allow_refer[memorizebit][current] = true;
        if (g.findWeight_undirected(current, start) != cost()) {
            wm.distance_memory[memorizebit][current] = g.findWeight_undirected(current, start);
        }
        else
            wm.distance_memory[memorizebit][current] = cost(); //혹시 마지막에 길 없으면 탈락시키기 위해 큰 값
        wm.path_memory[memorizebit][current][N - 2] = current;
        //생각해보니 무조건이네. 시작지점만 정해지면 이 [1111][각 지점]은 무조건
        //시작지점으로 가는 값임
        return;
    }

    for (i = 0; i < N; ++i) {
        if (ivertex_had_not_been_visited(i, memorizebit)
            && g.findWeight_undirected(current, i) != cost()) {
            //ivertex has been visited;
            //returnvalue = TSP(i, W, wm, memorizebit);
            TSP_undirected(i, g, wm, memorizebit);

            if (wm.distance_memory[memorizebit][current] > g.findWeight_undirected(current, i) + wm.distance_memory[memorizebit + (1 << i)][i]) {
                wm.distance_memory[memorizebit][current]
                = g.findWeight_undirected(current, i) + wm.distance_memory[memorizebit + (1 << i)][i];

                int j;
                int index = the_number_of_bit_is_on(memorizebit) - 1;
                for (j = N - 2; j >= index; --j) {
                    wm.path_memory[memorizebit][current][j] = wm.path_memory[memorizebit + (1 << i)][i][j];
                }
                wm.path_memory[memorizebit][current][j] = current;
            }
        }
    }

    wm.allow_refer[memorizebit][current] = true;

    return;
}


int main() {
    cout << "program start" << endl;
    N = defN;//cin >> N;

    /*
    int **W;
    nxnArrayAssign(W, N, N);
    for(int i = 0; i < N; ++i)
      for(int j = 0; j < N; ++j) {
        cin >> W[i][j];
      }
    */

    
    int W[4][4] = { {0, 10, 15, 20},
                    {5, 0, 9, 10},
                    {6, 13, 0, 12},
                    {8, 8, 9, 0}
                  };
                  
                  
    /*
    int W[6][6] = {
        0, 64, 378, 519, 434, 200,
        64, 0,318,455,375,164,
        378,318,0,170,265,344,
        519,455,170,0,223,428,
        434,375,265,223,0,273,
        200,164,344,428,273,0
    };
    */

    /*
    int W[15][15] = { //답은 1194
      {0, 141, 134, 152, 173, 289, 326, 329, 285, 401, 388, 366, 343, 305, 276},
                {141, 0, 152, 150, 153, 312, 354, 313, 249, 324, 300, 272, 247, 201, 176},
                {134, 152, 0, 24,  48,  168, 210, 197, 153, 280, 272, 257, 237, 210, 181 },
                {152, 150, 24, 0, 24,  163, 206, 182, 133, 257, 248, 233, 214, 187, 158 },
                {173, 153, 48, 24, 0, 160, 203, 167, 114, 234, 225, 210, 190, 165, 137},
                {289, 312, 168, 163, 160, 0, 43,  90,  124, 250, 264, 270, 264, 267, 249},
                {326, 354, 210, 206, 203, 43, 0, 108, 157, 271, 290, 299, 295, 303, 287},
                {329, 313, 197, 182, 167, 90, 108, 0, 70, 164, 183, 195, 194, 210, 201},
                {285, 249, 153, 133, 114, 124, 157, 70, 0, 141, 147, 148, 140, 147, 134},
                {401, 324, 280, 257, 234, 250, 271, 164, 141, 0, 36, 67, 88, 134, 150},
                {388, 300, 272, 248, 225, 264, 290, 183, 147, 36, 0, 33, 57, 104, 124},
                {366, 272, 257, 233, 210, 270, 299, 195, 148, 67, 33, 0, 26, 73, 96},
                {343, 247, 237, 214, 190, 264, 295, 194, 140, 88, 57, 26, 0, 48, 71},
                {305, 201, 210, 187, 165, 267, 303, 210, 147, 134, 104, 73, 48, 0, 30},
                {276, 176, 181, 158, 137, 249, 287, 201, 134, 150, 124, 96, 71, 30, 0}
                  };
                  */

    /*
    int W[11][11] = { //경로는 0-7-4-3-9-5-2-6-1-10-8-0, 거리는 253
        {0, 29, 20, 21, 16, 31, 100, 12, 4, 31, 18},
        {29, 0, 15, 29, 28, 40, 72, 21, 29, 41, 12},
        {20, 15, 0, 15, 14, 25, 81, 9, 23, 27, 13},
        {21, 29, 15, 0, 4, 12, 92, 12, 25, 13, 25},
        {16, 28, 14, 4, 0, 16, 94, 9, 20, 16, 22},
        {31, 40, 25, 12, 16, 0, 95, 24, 36, 3, 37},
        {100, 72, 81, 92, 94, 95, 0, 90, 101, 99, 84},
        {12, 21, 9, 12, 9, 24, 90, 0, 15, 25, 13},
        {4, 29, 23, 25, 20, 36, 101, 15, 0, 35, 18},
        {31, 41, 27, 13, 16, 3, 99, 25, 35, 0, 38},
        {18, 12, 13, 25, 22, 37, 84, 13, 18, 38, 0}
    };
    */
    

    graph g(N, W);
    way_memory wm(N);

    int memorizebit = 0;


    //시작은 아무렇게나 0번으로 잡고 시작

    //만약 원점에 도착하면 지금까지 왔던 경로 거꾸로 가면서 가중치 기억배열 갱신.
    //이미 배열에 뭐 들어있는 경우 더 작은 쪽이 이긴다.

    //그렇게 모든 경로 탐색 후 0-1[00010][1], 0-2[00100][2], 0-3[01000][3], 0-4[16][4] 중에서 가장 짧은 놈이 최종 승리.
    TSP_directed(start, g, wm, memorizebit);

    cout << start << " - ";
    for (int i = 0 ; i < N - 1; ++i)
        cout << wm.path_memory[1][start][i] << " - ";
    cout << start << endl;

    cout << wm.distance_memory[0b0001][start] << endl; //최단거리는 여기에 저장되있음음

    g.showEdges();

    //nxnArrayDelete(W, N);
    return 0;
}