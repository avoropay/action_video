/**************************************************************************/
/*!
  @file     fifoqueue.h
  @author   A.Voropay
  @license  GNU GPLv3



  @section  HISTORY

  v1.0 - First release
*/
/**************************************************************************/
#ifndef fifoqueue_H
#define fifoqueue_H
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class queue //Класс очереди
{
  private:
    struct queue_ob// Структура для хранения элементов очереди
    {
      float value;//Поле для хранения значения элемента очереди
      queue_ob *addr; // Поле для хранения слеющего элемента
      queue_ob *addr_prev; // Поле для хранения предыдущего элемента
    };

    queue_ob *head; // указатель, хранящий адресс головы очереди
    queue_ob *tail; // Указатель хранящий адрес хвоста
    unsigned long size, MaxSize; //Текущий размер очереди

  public:
    queue(unsigned long x);
    ~queue();
    int stack_size();
    void push(float value);
    void pop(float *ret);
    void peek(float *ret);
    float avg();
    float to_array(float IntoArray[], int n);
};

#endif
