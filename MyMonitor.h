//
// Created by hubertborkowski on 15.12.17.
//

#ifndef MONITORY_MYMONITOR_H
#define MONITORY_MYMONITOR_H

#include "monitor.h"
#include <queue>
#include <iostream>


class MyMonitor : Monitor
{
private:
  size_t size;
  std::queue<int> buffer;
  int removedCount;
  char lastDelete;
  char lastRead;
  Condition prod;
  Condition conA;
  Condition conB;
  Condition conC;

  const char defaultId = 'X';
  const int productionLimit;

  int read(char id);
  int remove(char id);
  void add(int item);
  bool isFull() const {return buffer.size() == size;};
  bool isEmpty() const {return buffer.empty();};

public:
  explicit MyMonitor(size_t size, int limit);
  ~MyMonitor() = default;

  void producer();
  void consumerA();
  void consumerB();
  void consumerC();
};


#endif //MONITORY_MYMONITOR_H
