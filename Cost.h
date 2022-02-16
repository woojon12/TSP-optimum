#pragma once
#include <iostream>

namespace direc{class graph;}
namespace undirec{class graph;}

/*
무한의 개념을 확립하려고 만든 클래스인데 어쩌다보니
가중치의 개념으로 바뀌어버림.

인스턴스 생성 시 아무것도 안 주면 무한으로 취급.
숫자를 주면 그 숫자로 취급.

덧셈뺄셈 오버로딩은 필요하면 만들겠음.
*/

class cost {
  int weight;
  bool absoluteInf;

public:
  cost() : absoluteInf(true) {}
  cost(int weight) : weight(weight), absoluteInf(false) {}

  friend class direc::graph;
  friend class undirec::graph;

  friend bool operator<(cost, cost);
  friend bool operator>(cost, cost);
  friend bool operator<=(cost, cost);
  friend bool operator>=(cost, cost);
  friend bool operator==(cost, cost);
  friend bool operator!=(cost, cost);
  friend cost operator+(cost, cost);
  friend std::ostream& operator<<(std::ostream&, cost&);
};

bool operator<(cost t1, cost t2)
{
  if(t1.absoluteInf && t2.absoluteInf) return false;
  else if (t1.absoluteInf) return false;
  else if (t2.absoluteInf) return true;

  return t1.weight < t2.weight;
}

bool operator>(cost t1, cost t2)
{
  if(t1.absoluteInf && t2.absoluteInf) return false;
  else if (t1.absoluteInf) return true;
  else if (t2.absoluteInf) return false;

  return t1.weight > t2.weight;
}

bool operator<=(cost t1, cost t2)
{
  if(t1.absoluteInf && t2.absoluteInf) return true;
  return t1 < t2;
  //사실 이러면 약간의 불필요한 if 연산이 일어나지만 쉬운 코드 이해를 위해 희생
}

bool operator>=(cost t1, cost t2)
{
  if(t1.absoluteInf && t2.absoluteInf) return true;
  return t1 > t2;
}

bool operator==(cost t1, cost t2)
{
  if(t1.absoluteInf && t2.absoluteInf) return true;
  else if(t1.weight == t2.weight) return true;
  return false;
}

bool operator!=(cost t1, cost t2)
{
  return !(t1 == t2);
}

cost operator+(cost c1, cost c2)
{
  if(c1.absoluteInf || c2.absoluteInf) return cost();
  return c1.weight + c2.weight;
}

std::ostream& operator<<(std::ostream& o, cost& c)
{
  if(c.absoluteInf) {o << "Infinite"; return o;}
  o << c.weight;
  return o;
}