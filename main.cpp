//
//Hubert Borkowski projekt nr 4 SOI
//
//Treść zadania:
//
//Bufor n-elementowy FIFO. Jest jeden producent i trzech konsumentów (A, B, C).
//Producent produkuje jeden element, jeżeli jest miejsce w buforze.
//Element jest usuwany z bufora, jeżeli zostanie przeczytany przez albo obu konsumentów A i B, albo przez obu konsumentów B i C.
//Konsument A nie może przeczytać elementu, jeżeli został on już przez niego wcześniej przeczytany, albo został przeczytany przez konsumenta C i na odwrót.
//Konsument B również nie może wielokrotnie czytać tego samego elementu.
//Ponadto, żaden konsument nie może dwa razy z rzędu usunąć elementu z bufora (nie można zakładać stałej kolejności odczytu elementów przez konsumentów).

#include <iostream>
#include "MyMonitor.h"

const int productionLimit = 300000;
const size_t bufferSize = 10;
MyMonitor monit(bufferSize,productionLimit);

void* init(void* arg);

int main()
{
  pthread_t producer, consumerA, consumerB, consumerC;

  char prod = 'P';
  char consA = 'A';
  char consB = 'B';
  char consC = 'C';

  pthread_create(&producer,NULL,init,&prod);
  pthread_create(&consumerA,NULL,init,&consA);
  pthread_create(&consumerB,NULL,init,&consB);
  pthread_create(&consumerC,NULL,init,&consC);

  pthread_join(producer,NULL);
  pthread_join(consumerA,NULL);
  pthread_join(consumerB,NULL);
  pthread_join(consumerC,NULL);

  monit.~MyMonitor();

  std::cout << "Koniec" << std::endl;

  return 0;
}

void* init(void* arg)
{
  auto* id = static_cast<char*>(arg);

  switch(*id)
  {
    case 'A':
      monit.consumerA();
      break;

    case 'B':
      monit.consumerB();
      break;

    case 'C':
      monit.consumerC();
      break;

    case 'P':
      monit.producer();
      break;

    default:
      break;
  }
}