#include <iostream>
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

/* �ʹ� �����ڵ�
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

#define defN 11
int N;

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
    int** distance_memory;
    int*** path_memory;
    bool** allow_refer;

    friend void TSP(int current, int W[][defN], way_memory& M, int memorizebit);
    friend int main();

public:
    way_memory(int N) {
        nxnArrayAssign(distance_memory, 1 << N, N);
        nxnArrayAssign(allow_refer, 1 << N, N);

        for (int i = 0; i < (1 << N); ++i)
            for (int j = 0; j < N; ++j)
                distance_memory[i][j] = 0;

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
            for (int j = 1; j < N; ++j) //���� �𸣰ڴµ� ���� j�� 0�� �ƴϰ� 1�̶�� �ؾ� ��Ÿ�� ������ �ȳ�
                delete[] path_memory[i][j];

        for (int i = 0; i < (1 << N); ++i)
            delete[] path_memory[i];

        delete[] path_memory;
    }
};

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

#define start 0 //������ ��ȸ�� ��� �����ϵ� �����
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
            wm.distance_memory[memorizebit][current] = 1000000; //Ȥ�� �������� �� ������ Ż����Ű�� ���� ū ��
        wm.path_memory[memorizebit][current][N - 2] = current;
        //�����غ��� �������̳�. ���������� �������� �� [1111][�� ����]�� ������
        //������������ ���� ����
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
����Լ��� if + return�� for�� ����.
for�� �� �־ �Ǵ� �� �ƴϸ�
if�� for ������ �Ʒ����� ���� �� ����.
�˰��� ���� if�� �� �� �̻��� �� �� ����.

�׸��� ����Լ��� �� ����� ���鼭 ���� ���̰� �׿�
������ ����Լ����� ���� �����ϴ� ��찡 �ִ°��ϸ�
(�� ��� ��������� ���õ� ������ ���ڷ� ��� �ѱ��
����Լ��� �� ������ �� ���信 ����� �� ��� �� ������ �����)
�ְ�� ����� ����Լ� ���ٰ� �ٽ� �ǵ��ƿ��鼭
��ȯ�ϴ� ���� ���̰� �׿� ������ ��ȯ���� ���� �Ǵ� �ɷε� ���� �� �ִ�.
(�� ��� ��������� ���õ� ������ ��ȯ�ϴ� ���ε�
�Ʒ�����Լ��� ȣ���� ������Լ� ���忡�� ���� ��
�Ʒ����� �̹� ������ ������ �ּ��� ����� ��ȯ�ǵ��� �ϰ�
�ڱ�� �װ� ������ �ּ��� ���� �� ��ȯ�ϴ� ��. �̰� ��ӵǸ� ������ ��ȯ�� ����.)
���±��� �� �� �ϳ��θ� ������ �Ǵ� ���� ���� ���� �� ��.
���ڴ� ���� ������ ������ �ν��Ͻ��� ������ �ʾƵ� �ȴ�. ���� ������ �ƴ�����,
�׳� �Ｎ���� cout << �Լ�() << endl; �̷��� ������� ����ص� �ȴٴ� ��.

�� �Լ��� ��쿣 ������.
*/

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

    /*
    int W[4][4] = { {0, 10, 15, 20},
                    {5, 0, 9, 10},
                    {6, 13, 0, 12},
                    {8, 8, 9, 0}
                  };
                  */

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
    int W[15][15] = { //���� 1194
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

    int W[11][11] = { //��δ� 0-7-4-3-9-5-2-6-1-10-8-0, �Ÿ��� 253
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

    way_memory wm(N);

    int memorizebit = 0;


    //������ �ƹ����Գ� 0������ ��� ����

    //���� ������ �����ϸ� ���ݱ��� �Դ� ��� �Ųٷ� ���鼭 ����ġ ���迭 ����.
    //�̹� �迭�� �� ����ִ� ��� �� ���� ���� �̱��.

    //�׷��� ��� ��� Ž�� �� 0-1[00010][1], 0-2[00100][2], 0-3[01000][3], 0-4[16][4] �߿��� ���� ª�� ���� ���� �¸�.
    TSP(start, W, wm, memorizebit);

    cout << start << " - ";
    for (int i = 0 ; i < N - 1; ++i)
        cout << wm.path_memory[1][start][i] << " - ";
    cout << start << endl;

    cout << wm.distance_memory[0b0001][start] << endl; //�ִܰŸ��� ���⿡ ���������

    //nxnArrayDelete(W, N);
    return 0;
}