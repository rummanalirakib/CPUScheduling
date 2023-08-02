#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include<math.h>
int numProcesses;
struct node {
    char processNumber[10];
    int arivalTime;
    int burstTime;
    int exitTime;
    int turnAroundTime;
    int waitingTime;
} Process[1000];

int compareArrivalTime(const void *s1, const void *s2)
{
  struct node *e1 = (struct node *)s1;
  struct node *e2 = (struct node *)s2;
  if(e1->arivalTime == e2->arivalTime) return e1->processNumber > e2->processNumber;
  else if(e1->arivalTime > e2->arivalTime) return 1;
  else return 0;
}

int compareProcessNumber(const void *s1, const void *s2)
{
  struct node *e1 = (struct node *)s1;
  struct node *e2 = (struct node *)s2;
  int processNumber = strcmp(e1->processNumber, e2->processNumber);
  return processNumber;
}

void GanttChart()
{
    qsort(Process, numProcesses, sizeof(struct node), compareArrivalTime);
    int timeStart=0, i=0;
    printf("Gant Chart:\n");
    printf("TimeDuration\t\tProcess\n");
    while(i<numProcesses)
    {
        if(timeStart<Process[i].arivalTime){
            printf("%d-%d\t\t\tIdle Time\n",timeStart, Process[i].arivalTime);
            timeStart=Process[i].arivalTime;
        }
        else{
           int exitTime=timeStart+Process[i].burstTime;
           int trnArndTime=exitTime-Process[i].arivalTime;
           printf("%d-%d\t\t\t%s\n",timeStart, exitTime, Process[i].processNumber, exitTime);
           Process[i].exitTime=exitTime;
           Process[i].turnAroundTime=trnArndTime;
           Process[i].waitingTime=trnArndTime-Process[i].burstTime;
           timeStart+=Process[i].burstTime;
           i++;
        }
    }
}

void VariousTimeProcess()
{
    qsort(Process, numProcesses, sizeof(struct node), compareProcessNumber);
    printf("\n\nTable with Results:\n");
    printf("Process\t\tExitTime\t\tTurnaroundTime\t\tWaitingTime\n");
    for(int i=0;i<numProcesses;i++)
    {
        printf("%s\t\t%d\t\t\t%d\t\t\t%d\n",Process[i].processNumber, Process[i].exitTime, Process[i].turnAroundTime, Process[i].waitingTime);
    }
}

char* toArray(int number)
{
    int n = log10(number) + 2;
    int i;
    char *numberArray = calloc(n, sizeof(char));
    for (i = n-1; i >= 1; --i, number /= 10)
    {
        numberArray[i] = (number % 10) + '0';
    }
    numberArray[0] = 'P';
    return numberArray;
}

void ReadFromFile()
{
    FILE* file = fopen("FIFO.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
    }

    char myString[100];
    while(fgets(myString, 100, file)) {
        char processNumber[10];
        int arivalTime;
        int burstTime;
        fscanf(file, "%s %d %d", &(processNumber), &(arivalTime), &(burstTime));
        strcpy(Process[numProcesses].processNumber,processNumber);
        Process[numProcesses].arivalTime=arivalTime;
        Process[numProcesses].burstTime=burstTime;
        numProcesses++;
    }
    fclose(file);
}

void RandomProcessGenerate()
{
    printf("Enter number of processes: ");
    scanf("%d",&numProcesses);
    for(int i=0;i<numProcesses;i++){
       strcpy(Process[i].processNumber,toArray(i+1));
       Process[i].arivalTime=rand()%numProcesses;
       Process[i].burstTime=rand()%numProcesses;
    }
}

int main() {
    printf("CPU Scheduling for FIFO(First in first out):\n");
    printf("1. Read the processes from file\n");
    printf("2. Random Value Generate for the process\n");
    printf("Enter 1 or 2: ");
    int n;
    scanf("%d", &n);
    if(n==1){
        numProcesses=0;
        ReadFromFile();
    }
    else{
        RandomProcessGenerate();
    }
    GanttChart();
    VariousTimeProcess();
    return 0;
}
