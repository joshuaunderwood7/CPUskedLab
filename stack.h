//Coded by Joshua Underwood 
//04NOV13 
//CSCI 3453 
//Licenced GPLv3 or latter
#ifndef UND_STACK_H
#define UND_STACK_H

const int UND_STACK_H_MAX_SIZE = 256;

template <typename T>
class stack_t {
public:
    stack_t() { top = -1; };

    bool push(T);
    T pop();  //Makes no check for empty stack

    bool push_front(T); //not efficient
    T pop_front(); //not efficient

    T peek();
    int size();

    bool isFull();
    bool isEmpty();
    int maxSize(){return UND_STACK_H_MAX_SIZE;}
    //virtual ~stack_t ();

private:
    T contents[UND_STACK_H_MAX_SIZE];
    int top;
};

#include "stack.cpp"
#endif
