#ifndef makeGRFs_H
#define makeGRFs_H
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <UTFT.h>
#include "fifoqueue.h"

class graphic {
  UTFT* refTFT;
  public:
    int DisplayXSize;
    int DisplayYSize;

    graphic(UTFT* myTFT);
    boolean begin();
    void displaySystemAxes();
    void displayGRF(queue* myQueue, int lowerRangeLimit, int upperRangeLimit);
};

#endif


