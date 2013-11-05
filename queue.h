//Coded by Joshua Underwood 
//04NOV13 
//CSCI 3453 
//Licenced GPLv3 or latter
#ifndef UND_QUEUE_H
#define UND_QUEUE_H

#include "stack.h"

template <typename T>
class queue_t {
public:
    queue_t(){};

    bool push(T input) { return theQueue.push_front(input); }

    T pop() { return theQueue.pop(); }

    T peek() { return theQueue.peek(); }

    int size() { return theQueue.size(); }
    bool isEmpty() { return theQueue.isEmpty(); }
    bool isFull() { return theQueue.isFull(); }
    //virtual ~queue_t ();

private:
    stack_t<T> theQueue;
};

#endif
