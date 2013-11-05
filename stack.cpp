//Coded by Joshua Underwood 
//04NOV13 
//CSCI 3453 
//Licenced GPLv3 or latter
//
//This is included in the stack.h 
//because of the use of templates

template <typename T>
bool stack_t<T>::push(T input)
{
    if(isFull()) return false;
    top++;
    contents[top] = input;
    return true;
};

template <typename T>
T stack_t<T>::pop()
{
    top--;
    return contents[top+1];
}; //Makes no check for empty stack

template <typename T>
bool stack_t<T>::push_front(T input)
{
    if(isFull()) return false;
    top++;
    for(int i = top; i>0; --i)
    {
        contents[i] = contents[i-1];
    } 
    contents[0] = input;
    return true;
}; //not efficient

template <typename T>
T stack_t<T>::pop_front()
{
    T temp = contents[0];
    for (int i = 0; i < top; i++) {
        contents[i] = contents[i+1];
    }
    top--;
    return temp;
}; //not efficient

template <typename T>
T stack_t<T>::peek()
{ return contents[top]; };

template <typename T>
int stack_t<T>::size()
{ return top + 1; };

template <typename T>
bool stack_t<T>::isFull()
{ return (1+top == UND_STACK_H_MAX_SIZE); };

template <typename T>
bool stack_t<T>::isEmpty()
{ return (top < 0); };
