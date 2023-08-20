#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include<math.h>
int numProcesses, timeQuantum;
struct node {
    char processNumber[10];
    int arivalTime;
    int burstTime;
    int burstTimeCheck;
    int exitTime;
    int turnAroundTime;
    int waitingTime;
} Process[1000];

struct Queue {
  int val;
  struct Queue *next;
};

int compareArrivalTime(const void *s1, const void *s2)
{
  struct node *e1 = (struct node *)s1;
  struct node *e2 = (struct node *)s2;

  if (e1->arivalTime == e2->arivalTime)
    return strcmp(e1->processNumber, e2->processNumber);
  else if (e1->arivalTime < e2->arivalTime)
    return -1;
  else
    return 1;
}

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

void GanttChart()
{
    qsort(Process, numProcesses, sizeof(struct node), compareArrivalTime);
    int timeStart=0, i=0, check=0;
    printf("Gant Chart:\n");
    printf("TimeDuration\t\tProcess\n");
    struct Queue *head = (struct Queue *) malloc (sizeof (struct Queue));
    struct Queue *tail = (struct Queue *) malloc (sizeof (struct Queue));
    tail->next=NULL;
    head=tail;

    while(1)
    {
        if(i>=numProcesses){
            break;
        }
        if(timeStart<Process[i].arivalTime){
           printf("%d-%d\t\t\tIdle Time\n",timeStart, Process[i].arivalTime);
           timeStart=Process[i].arivalTime;
        }
        struct Queue *newNode = (struct Queue *) malloc (sizeof (struct Queue));
        newNode->next = NULL;
        tail->val = i;
        tail->next=newNode;
        tail=tail->next;
        i++;

        while(1) {
            if(head==tail){
                break;
            }
            int p = head->val;
            if(timeQuantum<Process[p].burstTime) {
                Process[p].burstTime -= timeQuantum;
                printf("%d-%d\t\t\t%s\n",timeStart, timeStart+timeQuantum, Process[p].processNumber, timeStart+timeQuantum);
                timeStart += timeQuantum;
                int temp=p;
                for(;i<numProcesses;)
                {
                    if(Process[i].arivalTime<=timeStart){
                       struct Queue *newNode = (struct Queue *) malloc (sizeof (struct Queue));
                       newNode->next = NULL;
                       tail->val = i;
                       tail->next=newNode;
                       tail=tail->next;
                       i++;
                    }
                    else {
                        break;
                    }
                }
                struct Queue *newNode = (struct Queue *) malloc (sizeof (struct Queue));
                newNode->next = NULL;
                tail->val = temp;
                tail->next=newNode;
                tail=tail->next;
            }
            else{
               int exitTime=timeStart+Process[p].burstTime;
               int trnArndTime=exitTime-Process[p].arivalTime;
               printf("%d-%d\t\t\t%s\n",timeStart, exitTime, Process[p].processNumber, exitTime);
               Process[p].exitTime=exitTime;
               Process[p].turnAroundTime=trnArndTime;
               Process[p].waitingTime=trnArndTime-Process[p].burstTimeCheck;
               timeStart+=Process[p].burstTime;
               Process[p].burstTime = 0;
            }
            head = head->next;
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
    printf("Input Time Quantum: ");
    scanf("%d",&timeQuantum);
    FILE* file = fopen("roundrobin.txt", "r");
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
        Process[numProcesses].burstTimeCheck=burstTime;
        numProcesses++;
    }
    fclose(file);
}

void RandomProcessGenerate()
{
    printf("Enter number of processes: ");
    scanf("%d",&numProcesses);
    printf("Input Time Quantum: ");
    scanf("%d",&timeQuantum);
    for(int i=0;i<numProcesses;i++){
       strcpy(Process[i].processNumber,toArray(i+1));
       Process[i].arivalTime=rand()%numProcesses;
       Process[i].burstTime=rand()%numProcesses;
       if(Process[i].burstTime==0) Process[i].burstTime=1;
       Process[i].burstTimeCheck=Process[i].burstTime;
    }
}

void PrintTheProcesses()
{
    printf("Printing All the Processes:\n");
    printf("Process\t\tArrivalTime\t\tBurstTime\n");
    for(int i=0;i<numProcesses;i++)
    {
        printf("%s\t\t%d\t\t\t%d\n",Process[i].processNumber, Process[i].arivalTime, Process[i].burstTime);
    }
}

int main() {
    printf("CPU Scheduling for Round Robin:\n");
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
