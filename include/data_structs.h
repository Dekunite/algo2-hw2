/* @Author
Student Name: <Muhammet Derviş Kopuz>
Student ID : <504201531>
Date: <13/01/2021> */

#pragma once
#include <iostream>
#include <stdlib.h>
using namespace std;

struct Subtask {
  string name;
  int duration;
  Subtask* next;
};

struct Stack {
  Subtask* head;
  void init();
  void close();
  void push(Subtask* in);
  Subtask* pop();
  bool isEmpty();
};

struct Process {
  string name;
  int arrival_time;
  int deadline;
  int task_count;
  int priority;
  Stack task_stack;
  Process* next;
};

struct Queue {
  Process* head;
  Process* tail;
  void init();
  void close();
  void queue(Process* in);
  Process* dequeue();
  bool isEmpty();
  Process* front();
};

struct MultiQueue {
  Queue queues[3];
  void init();
  void close();
  void queue(Process* in);
  Process* dequeue(int priority);
  bool isEmpty();
  Process* front(int priority);
};