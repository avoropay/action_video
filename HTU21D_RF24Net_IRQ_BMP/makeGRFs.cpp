/**************************************************************************/
/*!
  @file     makeGRFs.h
  @author   A.Voropay
  @license  GNU GPLv3



  @section  HISTORY

  v1.0 - First release
*/
/**************************************************************************/

#include "makeGRFs.h"

/* classes */

graphic::graphic(UTFT* myTFT) {
  refTFT = myTFT;
};

boolean graphic::begin() {
  DisplayXSize = refTFT->getDisplayXSize();
  DisplayYSize = refTFT->getDisplayYSize();
  return true;
}

void graphic::displaySystemAxes() {
  refTFT->setColor(255, 0, 0);
  refTFT->fillRect(0, 0, 159, 13);
  refTFT->setColor(64, 64, 64);
  refTFT->fillRect(0, 114, 159, 127);
  refTFT->setColor(255, 255, 255);
  refTFT->setBackColor(255, 0, 0);
  refTFT->print("Universal TFT Lib.", CENTER, 1);
  refTFT->setBackColor(64, 64, 64);
  refTFT->setColor(255, 255, 0);
  refTFT->print("H.Karlsen", LEFT, 114);
  refTFT->print("(C)2015", RIGHT, 114);

  refTFT->setColor(0, 0, 255);
  refTFT->drawRect(0, 13, 159, 113);

  refTFT->setColor(0, 0, 0);
  refTFT->fillRect(1, 14, 158, 113);
  refTFT->setColor(0, 0, 255);
  refTFT->setBackColor(0, 0, 0);
  refTFT->drawLine(79, 14, 79, 113);
  refTFT->drawLine(1, 63, 158, 63);
}

void graphic::displayGRF(queue* myQueue, int lowerRangeLimit, int upperRangeLimit) {
  float buf_val[DisplayXSize];

  float avg;
  unsigned long factSize;

  avg = myQueue->to_array(buf_val, DisplayXSize);

  factSize = myQueue->stack_size();

  int y;
  for (int i = 1; i < DisplayXSize - 1; i++)
  {
    Serial.print(buf_val[i]);
    y = map(buf_val[i], lowerRangeLimit, upperRangeLimit, 0, DisplayYSize);
    Serial.print(i);
    Serial.println(y);
    refTFT->setColor(0, 255, 255);
    refTFT->drawPixel(i, y);
  }
}

