#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include<math.h>
int numProcesses, currentTime;
struct node {
    char processNumber[10];
    int arivalTime;
    int burstTime;
    int exitTime;
    int turnAroundTime;
    int waitingTime;
} Process[1000];

int compareProcessNumber(const void *s1, const void *s2)
{
  struct node *e1 = (struct node *)s1;
  struct node *e2 = (struct node *)s2;
  int processNumber = strcmp(e1->processNumber, e2->processNumber);
  return processNumber;
}

int compareBurstTime(const void  *s1, const void *s2)
{
  struct node *e1 = (struct node *)s1;
  struct node *e2 = (struct node *)s2;
  if(e1->arivalTime <= currentTime && e2->arivalTime <= currentTime) {
    if(e1->burstTime == e2->burstTime) return e1->arivalTime > e2->arivalTime;
    else return e1->burstTime > e2->burstTime;
  }
  else return e1->arivalTime > e2->arivalTime;
}


void SortProcesses() {
    for(int i=0;i<numProcesses;i++)
    {
        for(int j=i+1;j<numProcesses;j++){
            if(compareBurstTime(&Process[i], &Process[j])){
                 struct node temp = Process[i];
                 Process[i] = Process[j];
                 Process[j] = temp;
            }
        }
    }
}

void GanttChart()
{
    int timeStart=0, count1=0;
    currentTime=0;
    printf("Gant Chart:\n");
    printf("TimeDuration\t\tProcess\n");
    while(count1<numProcesses)
    {
        SortProcesses();
        if(timeStart<Process[0].arivalTime){
            printf("%d-%d\t\t\tIdle Time\n",timeStart, Process[0].arivalTime);
            timeStart=Process[0].arivalTime;
            currentTime=timeStart;
        }
        else{
           int exitTime=timeStart+Process[0].burstTime;
           int trnArndTime=exitTime-Process[0].arivalTime;
           printf("%d-%d\t\t\t%s\n",timeStart, exitTime, Process[0].processNumber, exitTime);
           currentTime=exitTime;
           Process[0].exitTime=exitTime;
           Process[0].turnAroundTime=trnArndTime;
           Process[0].waitingTime=trnArndTime-Process[0].burstTime;
           Process[0].arivalTime=1e8;
           timeStart+=Process[0].burstTime;
           count1++;
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
    FILE* file = fopen("file.txt", "r");
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
    GanttChart();
    VariousTimeProcess();
    return 0;
}

