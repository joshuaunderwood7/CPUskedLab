#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;

int randInt() { return rand() % 10 + 1; };

int main(int argc, const char *argv[])
{
    if(argc < 2)
    {
        cout << "!#>skedmaker [Number of Jobs] > [filename.txt]"
            << endl;
        return 1;
    }

    srand(time(NULL));
    int numberOfJobs = atoi(argv[1]);
    int arrivalTime = 0;
    int burstTime = randInt();

    for (int i = 0; i < numberOfJobs; i++) {
        cout << i << '\t'
            << arrivalTime << '\t'
            << burstTime << '\n';
        arrivalTime += randInt();
        burstTime = randInt();
    }

    return 0;
}
