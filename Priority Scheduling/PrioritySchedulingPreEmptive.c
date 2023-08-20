#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include<math.h>
int numProcesses, currentTime;
struct node {
    char processNumber[10];
    int priorityNumber;
    int arivalTime;
    int burstTime;
    int anthrBurstTime;
    int exitTime;
    int turnAroundTime;
    int waitingTime;
} Process[1000];

int compareProcessNumber(const void *s1, const void *s2)
{
    struct node *e1 = (struct node *)s1;
    struct node *e2 = (struct node *)s2;

    int num1 = atoi(e1->processNumber + 1);
    int num2 = atoi(e2->processNumber + 1);

    if (num1 < num2) {
        return -1;
    } else if (num1 > num2) {
        return 1;
    } else {
        return 0;
    }
}

int comparePriorityTime(const void  *s1, const void *s2)
{
  struct node *e1 = (struct node *)s1;
  struct node *e2 = (struct node *)s2;
  if(e1->arivalTime <= currentTime && e2->arivalTime <= currentTime) {
    if(e1->priorityNumber == e2->priorityNumber) return (e1->arivalTime > e2->arivalTime)?1:-1;
    else return (e1->priorityNumber > e2->priorityNumber)?1:-1;
  }
  else return (e1->arivalTime > e2->arivalTime)?1:-1;
}

void GanttChart()
{
    int timeStart=0, count1=0;
    currentTime=0;
    printf("Gant Chart:\n");
    printf("TimeDuration\t\tProcess\n");
    char processCode[10]="P0";
    while(count1<numProcesses)
    {
        qsort(Process, numProcesses, sizeof(struct node), comparePriorityTime);
        if(currentTime<Process[0].arivalTime){
            printf("%d-%d\t\t\tIdle Time\n",timeStart, Process[0].arivalTime);
            timeStart=Process[0].arivalTime;
            currentTime=timeStart;
        }
        if(Process[0].burstTime>1){
            if(strcmp(processCode, Process[0].processNumber)!=0 && strcmp(processCode, "P0")!=0 && timeStart!=currentTime){
                printf("%d-%d\t\t\t%s\n",timeStart, currentTime, processCode);
                timeStart=currentTime;
            }
            currentTime+=1;
            Process[0].burstTime-=1;
        }
        else if(Process[0].burstTime==1){
           if(strcmp(processCode, Process[0].processNumber)!=0 && strcmp(processCode, "P0")!=0 && timeStart!=currentTime){
                printf("%d-%d\t\t\t%s\n",timeStart, currentTime, processCode);
                timeStart=currentTime;
           }
           printf("%d-%d\t\t\t%s\n",timeStart, currentTime+1, Process[0].processNumber);
           currentTime+=1;
           Process[0].burstTime-=1;
           if(Process[0].burstTime<=0){
              int trnArndTime=currentTime-Process[0].arivalTime;
              Process[0].exitTime=currentTime;
              Process[0].turnAroundTime=trnArndTime;
              Process[0].waitingTime=trnArndTime-Process[0].anthrBurstTime;
              Process[0].arivalTime=1e8;
              count1++;
           }
           timeStart=currentTime;
        }
        strcpy(processCode, Process[0].processNumber);
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
    FILE* file = fopen("PrioritySchedulingPreEmptive.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
    }

    char myString[100];
    while(fgets(myString, 100, file)) {
        char processNumber[10];
        int priorityNumber;
        int arivalTime;
        int burstTime;
        fscanf(file, "%s %d %d %d", &(processNumber), &(priorityNumber), &(arivalTime), &(burstTime));
        strcpy(Process[numProcesses].processNumber,processNumber);
        Process[numProcesses].priorityNumber=priorityNumber;
        Process[numProcesses].arivalTime=arivalTime;
        Process[numProcesses].burstTime=burstTime;
        Process[numProcesses].anthrBurstTime = burstTime;
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
       Process[i].priorityNumber=rand()%numProcesses;
       Process[i].arivalTime=rand()%numProcesses;
       Process[i].burstTime=rand()%numProcesses;
       Process[i].anthrBurstTime = Process[i].burstTime;
       if(Process[i].burstTime==0) Process[i].burstTime=1;
    }
}

void PrintTheProcesses()
{
    printf("Printing All the Processes:\n");
    printf("Process\t\tArrivalTime\t\tBurstTime\t\tPriority\n");
    for(int i=0;i<numProcesses;i++)
    {
        printf("%s\t\t%d\t\t\t%d\t\t\t%d\n",Process[i].processNumber, Process[i].arivalTime, Process[i].burstTime, Process[i].priorityNumber);
    }
}

int main() {
    printf("CPU Scheduling for SJF(Shortest Job First):\n");
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
    PrintTheProcesses();
    GanttChart();
    VariousTimeProcess();
    return 0;
}
