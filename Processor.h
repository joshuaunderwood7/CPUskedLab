//Coded by Joshua Underwood 
//04NOV13 
//CSCI 3453 
//Licenced GPLv3 or latter
#ifndef UND_PROCESSOR_H
#define UND_PROCESSOR_H

#include <iostream>

using namespace std;

class Process
{
  public:
    Process() {}
    Process(int ID_, int arrivalTime_, int burstTime_):
        ID(ID_), arrivalTime(arrivalTime_), burstTime(burstTime_)
        {
            finishTime = 0; 
            burstLeft = burstTime;
            waitTime = 0;
            turnAround = 0;
            responseTime = 0;
            noContextSwitch = 0;
            ack = false;
        }

    int ID, arrivalTime, burstTime, finishTime;
    int burstLeft, responseTime;
    int waitTime, turnAround, noContextSwitch;
    bool ack;
};

class ProcessorPool
{
  public:
    ProcessorPool(int numberOfProcessors_): numberOfProcessors(numberOfProcessors_){};

    bool add(Process input)
    {
        if(toProcess.size() < numberOfProcessors)
        { 
            input.ack = true;
            toProcess.push(input);
            return true;
        }
        return false;
    }

    queue_t<Process> doProcessing(int time)
    {
        queue_t<Process> finished;
        while(!toProcess.isEmpty())
        {
            Process temp = toProcess.pop();
            --temp.burstLeft;
            if(temp.burstLeft <= 0) 
            {
               temp.finishTime = time; 
               temp.turnAround = time - temp.arrivalTime;
               finished.push(temp); 
            }
            else { alreadyProcessed.push(temp); }
        }
        while(!alreadyProcessed.isEmpty())
        { toProcess.push(alreadyProcessed.pop()); }
        return finished;
    }

    Process peek(void) {return toProcess.peek(); } // for debugging only

    Process contextSwitchPop(void) 
    { 
        Process temp = toProcess.pop(); 
        temp.noContextSwitch++;
        return temp; 
    }

    bool hasQueue() { return !toProcess.isEmpty(); }

  private:
    stack_t<Process> toProcess, alreadyProcessed;
    int numberOfProcessors;
};

#endif
