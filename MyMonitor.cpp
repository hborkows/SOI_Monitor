//
// Created by hubertborkowski on 15.12.17.
//

#include "MyMonitor.h"

MyMonitor::MyMonitor(size_t size, int limit): size(size), buffer(), lastDelete('X'),
                                   lastRead('X'), prod(), conA(), conB(), conC(), productionLimit(limit),
                                              removedCount(0)
{

}

void MyMonitor::producer()
{
  int i = 0;

  while(i < productionLimit)
  {
    enter();

    while(isFull())
    {
      wait(prod);
    }

    add(i++);

    if(!isEmpty()) signal(conA);
    if(!isEmpty()) signal(conB);
    if(!isEmpty()) signal(conC);

    leave();
  }

  pthread_exit(NULL);
}

void MyMonitor::consumerA()
{
  const char id = 'A';
  int item;

  while(removedCount < productionLimit)
  {
    enter();

    while (isEmpty() || lastRead == 'A' || lastRead == 'C' || lastDelete == 'B')
    {
      wait(conA);
      if(lastRead == 'B')
        break;
      if(removedCount == productionLimit)
        break;
    }

    if(/*!isEmpty() && */lastRead == 'X')
    {
      usleep(10);
      item = read(id);
      std::cout << "Konsument A czytam: " << item << std::endl;
    }

    while(!isEmpty() && lastRead == 'B')
    {
      if (lastDelete == id)
      {
        wait(conA);
      }
      else
      {
        usleep(10);
        item = remove(id);
        std::cout << "Konsument A usuwam: " << item << std::endl;

        signal(prod);
        signal(conC);
        signal(conB);

        break;
      }
    }

    leave();
  }
  pthread_exit(NULL);
}

void MyMonitor::consumerB()
{
  int item;
  const char id = 'B';

  while(removedCount < productionLimit)
  {
    enter();

    while (isEmpty() || lastRead == 'B')
    {
      wait(conB);
    }

    while(/*!isEmpty() &&*/ lastRead == 'X')
    {
      usleep(10);
      item = read(id);
      std::cout << "Konsument B czytam: " << item << std::endl;

      signal(conA);
      signal(conC);
    }

    if(!isEmpty() && (lastRead == 'A' || lastRead == 'C'))
    {
      usleep(10);
      item = remove(id);
      std::cout << "Konsument B usuwam: " << item << std::endl;

      signal(prod);
      signal(conA);
      signal(conC);
    }

    leave();
  }
  pthread_exit(NULL);
}

void MyMonitor::consumerC()
{
  int item;
  const char id = 'C';

  while(removedCount < productionLimit)
  {
    enter();

    while (isEmpty() || lastRead == 'A' || lastRead == 'C' || lastDelete == 'B')
    {
      wait(conC);
      if(lastRead == 'B')
        break;
      if(removedCount == productionLimit)
        break;
    }

    if (/*!isEmpty() &&*/ lastRead == 'X')
    {
      usleep(10);
      item = read(id);
      std::cout << "Konsument C czytam: " << item << std::endl;
    }

    while (!isEmpty() && lastRead == 'B')
    {
      if (lastDelete == id)
      {
        wait(conC);
      }
      else
      {
        usleep(10);
        item = remove(id);
        std::cout << "Konsument C usuwam: " << item << std::endl;

        signal(prod);
        signal(conA);
        signal(conB);

        break;
      }
    }

    leave();
  }
  pthread_exit(NULL);
}

void MyMonitor::add(int item)
{
  buffer.push(item);
}

int MyMonitor::read(const char id)
{
  int value = buffer.front();
  lastRead = id;

  return value;
}

int MyMonitor::remove(const char id)
{
  int value = buffer.front();
  buffer.pop();
  lastDelete = id;
  lastRead = defaultId;
  removedCount++;

  return value;
}