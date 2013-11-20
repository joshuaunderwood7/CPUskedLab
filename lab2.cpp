//Coded by Joshua Underwood 
//04NOV13 
//CSCI 3453 
//Licenced GPLv3 or latter
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <sstream>
#include "queue.h"
#include "Processor.h"

using namespace std;

//all time is in ms
const int UND_CONTEXT_SWITCH_COST = 5;
const int UND_NUMBER_OF_PROCESSORS = 1;
const int MILLISECOND_MOD = 10;
enum sceduler {FCFS, SRTF, RR};

void pprintProcess(Process);
void pprintHeader(string);
void contextSwitch(int &time) { time += UND_CONTEXT_SWITCH_COST; }

int main(int argc, const char *argv[])
{
    if(argc < 3)
    {
        cout << " FCFS:  unixprompt> myscheduler test_input_file  0 " << endl <<
                " SRTF:  unixprompt> myscheduler test_input_file  1 " << endl << 
                " RR:    unixprompt> myscheduler test_input_file  2 quantumsize" << endl; 
        return 1;
    }

    string SceduleType = ""; 
    queue_t<Process> inputQueue, processQueue, resultsQueue;
    queue_t<Process> tempResults;
    ProcessorPool pPool(UND_NUMBER_OF_PROCESSORS);
    int schedulerIn, quantumSize;
    int timeOffset = 0;
    quantumSize = 5;

    schedulerIn = atoi(argv[2]);
    if(argc >=4 )
    {
        quantumSize = MILLISECOND_MOD * atoi(argv[3]);
    }

    int time = 0;

    ifstream inputFile;
    inputFile.open(argv[1]);
    if(inputFile.is_open())
    {
        int ID, arrivalTime, burstTime;
        while( inputFile >> ID 
            && inputFile >> arrivalTime
            && inputFile >> burstTime
            && inputFile.good())
        {
            inputQueue.push(Process(ID, 
                                   (MILLISECOND_MOD * arrivalTime), 
                                   (MILLISECOND_MOD * burstTime)));
        }
        //inputQueue.push(Process(ID, arrivalTime, burstTime));
        inputFile.close();
    }
    
    //this is the begining of the loop that simulates the process and scedule
    while(!(inputQueue.isEmpty() && processQueue.isEmpty() ) || pPool.hasQueue())
    {
        //see if any new jobs show up at this time.
        if(!inputQueue.isEmpty())
        {
            if(inputQueue.peek().arrivalTime <= time)
            { processQueue.push(inputQueue.pop()); }
        }

        //Here is the scedule algo position
        Process current; //only used for SRTF
        switch (schedulerIn)
        {
          case FCFS:
              SceduleType = " FCFS ";
              //The default behavior of this control loop
              //is FCFS, so no special behavior is needed.
              break;
          case SRTF:
              SceduleType = " SRTF ";
              //replace current process into the processQueue
              //(This does not mean that there is a 
              //contextSwitch, we will test for that latter
              if(pPool.hasQueue()) 
              {
                  current = pPool.contextSwitchPop(); //track what the current process was.
                  processQueue.push(current);
              }
              else current.ID = -1; //for first loop
              //Find the shortet job and bring it to the
              //of the processQueue
              if(!processQueue.isEmpty())
              {
                Process tempa = processQueue.pop();
                while(!processQueue.isEmpty()) 
                { 
                    Process tempb = processQueue.pop();
                    if(tempa.burstLeft <= tempb.burstLeft)
                    { tempResults.push(tempb); }
                    else
                    { 
                        tempResults.push(tempa);
                        tempa = tempb;
                    }
                }
                //if the shortet job is not the current job
                //then there is a contextSwitch cost
                if(tempa.ID != current.ID) 
                { contextSwitch(time); }
                else
                { tempa.noContextSwitch--; } 
                
                processQueue.push(tempa);
                //replace the non-shortet jobs back into processQueue
                while(!tempResults.isEmpty()) {processQueue.push(tempResults.pop());}
              }

              break;
          case RR:
              std::stringstream ss; 
              ss << quantumSize/MILLISECOND_MOD;
              SceduleType = "RR Quant:" + ss.str();
              //the timeOffset is set when a job enters the processQueue
              //(This does not model multiple processors well)
              //when the time-delta is at the quantumSize, then rotate
              //jobs
              if((time-timeOffset >= quantumSize) && pPool.hasQueue())
              {
                    processQueue.push(pPool.contextSwitchPop());
                    contextSwitch(time);
              }
              break;
        }
        //This is the end of scheduleing 

        //try to assign jobs to processor, baised on who is in
        //front of the processQueue
        if(!processQueue.isEmpty()){
            if(!pPool.hasQueue()) {timeOffset = time;}
            if(pPool.add(processQueue.peek())){ processQueue.pop(); }
        }

        //incremens the time and does the Processing
        ++time;
        tempResults = pPool.doProcessing(time);

        //this returns any completed job to the resultsQueue for display
        while(!tempResults.isEmpty()) {resultsQueue.push(tempResults.pop()); }

        //this adds waitTime to the jobs in the processQueue,
        //and increments the responseTime counter if job has
        //not been picked upp by processor at all so far.
        while(!processQueue.isEmpty()) 
        { 
            Process temp = processQueue.pop();
            if(!temp.ack) { temp.responseTime++; }
            temp.waitTime++;
            tempResults.push(temp); 
        }
        //push the jobs back into the processQueue
        while(!tempResults.isEmpty()) {processQueue.push(tempResults.pop());}

    }

    //calculate and display results
    pprintHeader(SceduleType);
    float avgBurst = 0.0;
    float avgWait = 0.0;
    float avgTurnAround = 0.0;
    float avgResponseTime = 0.0;
    float totalContextSwitchs = 0.0;
    float counter = 0.0;
    while(!resultsQueue.isEmpty())
    {
        pprintProcess(resultsQueue.peek());
        Process temp = resultsQueue.pop();

        counter += 1.0;
        avgBurst += temp.burstTime;
        avgWait += temp.waitTime;
        avgTurnAround += temp.turnAround;
        avgResponseTime += temp.responseTime;
        totalContextSwitchs += temp.noContextSwitch;
    }

    cout << "Average CPU burst time = " << avgBurst/counter/MILLISECOND_MOD << " ms,   Average waiting time = " 
            << avgWait/counter/MILLISECOND_MOD << " ms" << endl 
         << "Average turn around time = " << avgTurnAround/counter/MILLISECOND_MOD << " ms,  Average response time = " 
            << avgResponseTime/counter << " ms" << endl 
         << "Total No. of Context Switching Performed = " << totalContextSwitchs << endl; 

  return 0;
}


void pprintProcess(Process input)
{
    cout << "|" << std::setw(9) << input.ID << "|"
         << std::setw(10) << input.arrivalTime/MILLISECOND_MOD << "|"
         << std::setw(10) << input.burstTime/MILLISECOND_MOD << "|"
         << std::setw(10) << input.finishTime/MILLISECOND_MOD << "|" 
         << std::setw(10) << input.waitTime/MILLISECOND_MOD << "|" 
         << std::setw(10) << input.turnAround/MILLISECOND_MOD << "|" 
         << std::setw(10) << input.responseTime/MILLISECOND_MOD << "|" 
         << std::setw(10) << input.noContextSwitch << "|" 
         << endl;
    cout << "+---------+----------+----------+----------+----------+----------+----------+----------+" << endl;
}
void pprintHeader(string headerTitle)
{
    cout << "----------------------------------------------------------------------------------------" << endl;
    cout << "--------------------------------"<< std::setw(11) << headerTitle 
         <<"---------------------------------------------" << endl;
    cout << "----------------------------------------------------------------------------------------" << endl;
    cout << "|" << std::setw(10) << "ID  |" 
         << std::setw(11) << "arrival |" 
         << std::setw(11) << "CPU burst|" 
         << std::setw(11) << "finish |" 
         << std::setw(11) << "waiting |" 
         << std::setw(11) << "turn |" 
         << std::setw(11) << "response |" 
         << std::setw(11) << " # context|" 
         << endl;
    cout << "|" << std::setw(10) << "|" 
         << std::setw(11) << "|" 
         << std::setw(11) << "|" 
         << std::setw(11) << "|" 
         << std::setw(11) << "time |" 
         << std::setw(11) << "around |" 
         << std::setw(11) << "time |" 
         << std::setw(11) << "switch |" 
         << endl;
    cout << "+---------+----------+----------+----------+----------+----------+----------+----------+" << endl;
}

