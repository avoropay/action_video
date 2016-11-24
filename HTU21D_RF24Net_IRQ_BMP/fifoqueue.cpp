/**************************************************************************/
/*!
  @file     fifoqueue.cpp
  @author   A.Voropay
  @license  GNU GPLv3



  @section  HISTORY

  v1.0 - First release
*/
/**************************************************************************/

#include "fifoqueue.h"


queue::queue(unsigned long x) {
  //В параметр передается голова очереди
  {
    size = 0;
    head = NULL;
    tail = head;
    MaxSize = x;
  }
};

//Реализация деструктора
queue::~queue() {
  queue_ob *current = NULL; //указатель на элемент, подлежащий удалению
  queue_ob *next = head; //указатель на следующий
  while (next) //пока есть еще элементы в списке
  {
    current = next;
    next = next->addr; //переход к следующему
    delete current;
  }
}

int queue::stack_size() //Функция возвращает размер очереди(кол-во элеменетов)
{
  return size;
}

void queue::push(float value) // Добавляет(вталкивает) элемент в очередь
{
  size++;// Кол-во элементов очереди увеличивается

  queue_ob *temp = new(queue_ob); // Выделяем память под новый хвост очереди
  if (head == NULL) // если очередь еще пуста, на первый элемент указывает и голова и хвост
  {
    head = temp;
  }

  if (size == MaxSize) {
    float ret;
    pop(&ret);
  }

  temp->addr = 0; // Записываем в поле адреса нового хвоста ноль, так как за ничего нету
  temp->value = value; //Заполняем поле значения нового хвоста
  if (tail != NULL) {
    tail->addr = temp; // Записываем в поле адреса старого хвоста адрес нового хвоста
  }
  tail = temp; //Записываем в указель хвоста адрес действительного хвоста

}

void queue::pop(float *ret) //Удаляет элемент(выталкивает) и возвращает его
// По адресу, хранящемся в ret, передается значение удаляемой головы
{
  if (size == 0)
  {
    //cout << "Очередь пуста - удалять нечего!" << endl;
    return;
  }

  queue_ob *temp = head; // записываем адрес головы
  *ret = head->value; // записываем значени головы
  head = head->addr; // изменяем адрес головы
  delete temp;  // удаляем старую голову
  size--; // уменьшаем размер
}

void queue::peek(float *ret)//Выгружает значение по адресу, передаваемому в парметре
{
  if (size == 0) // Если очередь пуста(size == 0), то ничего не выгружает
  {
    // cout << "Очередь пуста!" << endl;
    return;
  }
  *ret = head->value; // Выгружаем значение головы
}

float queue::avg() {
  float summValues = 0;
  unsigned long countValues = 0;
  queue_ob *current = NULL; //указатель на текущий элемент
  queue_ob *next = head; //указатель на следующий
  while (next) //пока есть еще элементы в списке
  {
    current = next;
    next = next->addr; //переход к следующему
    summValues += current->value;
    countValues++;
  }
  return summValues / countValues;
}

float queue::to_array(float IntoArray[], int sizeArray) {
  float summValues = 0;
  unsigned long countValues = 0;
  queue_ob *current = NULL; //указатель на текущий элемент
  queue_ob *next = head; //указатель на следующий
  while (next && countValues < sizeArray) //пока есть еще элементы в списке
  {
    current = next;
    next = next->addr; //переход к следующему
    IntoArray[countValues] = current->value;
    summValues += current->value;
    countValues++;
  }
  return summValues / countValues;
}

