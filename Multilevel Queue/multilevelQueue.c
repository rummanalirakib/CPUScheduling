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
} Process[1000][1000];

int QueueValue[1000], queueNumber, ProcessNumbers[1000], totalBurstTime;

struct Queue {
  int val;
  struct Queue *next;
};

struct RoundRobinInfo {
    struct Queue *top;
    int timeQuantum;
    struct Queue *bottom;
}roundRobinData[1000];

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

void insertStart(struct Queue *head, int data)
{
  printf("Data: %d\n",data);
  struct Queue *newNode = (struct Queue *) malloc (sizeof (struct Queue));
  newNode->next = NULL;
  printf("Data1: %d\n",data);
  head->val = data;
  printf("Data2: %d\n",data);
  head->next=newNode;
  printf("Data3: %d\n",data);
  head=head->next;
}

void GanttChart()
{
   // for(int i=0;i<)
   /* qsort(Process, numProcesses, sizeof(struct node), compareArrivalTime);
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
*/
    for(int time=0;time<=totalBurstTime;time++)
    {

    }
}

void VariousTimeProcess()
{
  /*  qsort(Process, numProcesses, sizeof(struct node), compareProcessNumber);
    printf("\n\nTable with Results:\n");
    printf("Process\t\tExitTime\t\tTurnaroundTime\t\tWaitingTime\n");
    for(int i=0;i<numProcesses;i++)
    {
        printf("%s\t\t%d\t\t\t%d\t\t\t%d\n",Process[i].processNumber, Process[i].exitTime, Process[i].turnAroundTime, Process[i].waitingTime);
    }
    */
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
    FILE* file = fopen("file.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
    }

    char myString[100];
    while(fgets(myString, 100, file)) {
        char processNumber[10];
        int arivalTime;
        int burstTime;
        int priority;
        fscanf(file, "%s %d %d %d", &(processNumber), &(arivalTime), &(burstTime), &(priority));
        int index=ProcessNumbers[priority];
        totalBurstTime+=burstTime;
        strcpy(Process[priority][index].processNumber,processNumber);
        Process[priority][index].arivalTime=arivalTime;
        Process[priority][index].burstTime=burstTime;
        Process[priority][index].burstTimeCheck=burstTime;
        ProcessNumbers[priority]+=1;
    }
    for(int i=1;i<=queueNumber;i++)
    {
       qsort(Process[i], ProcessNumbers[i], sizeof(struct node), compareArrivalTime);
    }

    for(int i=1;i<=queueNumber;i++)
    {
        printf("%d\n",i+1);
        for(int j=0;j<ProcessNumbers[i];j++)
        {
            printf("%s %d %d\n",Process[i][j].processNumber,Process[i][j].arivalTime,Process[i][j].burstTime);
        }
    }
    fclose(file);
}

void RandomProcessGenerate()
{
    /*printf("Enter number of processes: ");
    scanf("%d",&numProcesses);
    timeQuantum = rand()%100 + 1;
    for(int i=0;i<numProcesses;i++){
       strcpy(Process[i].processNumber,toArray(i+1));
       Process[i].arivalTime=rand()%numProcesses;
       Process[i].burstTime=rand()%numProcesses;
    }*/
}

void QueueInformation()
{
    printf("Enter the number of Queue you want: ");
    scanf("%d",&queueNumber);
    printf("The scheduling algorithm:\n");
    printf("1 for Round Robin\n");
    printf("2 for Shortest Job first\n");
    printf("3 for First Come First Serve\n");
    for(int i=0;i<queueNumber;i++)
    {
        printf("Enter the number %d queue to assign which scheduling: ",i+1);
        int val;
        scanf("%d",&val);
        if(val==1){
            printf("Enter the Time Quantum: ");
            scanf("%d",&roundRobinData[i+1].timeQuantum);
        }
        else if(val>3 || val<0){
            printf("Please select the right number.\n");
            i--;
        }
        else{
            QueueValue[i]=val;
        }
    }
}

int main() {
    printf("CPU Scheduling for Round Robin:\n");
    printf("1. Read the processes from file\n");
    printf("2. Random Value Generate for the process\n");
    printf("Enter 1 or 2: ");
    int n;
    scanf("%d", &n);
    totalBurstTime=0;
    if(n==1){
        QueueInformation();
        numProcesses=0;
        ReadFromFile();
    }
    else{
        QueueInformation();
        RandomProcessGenerate();
    }
    GanttChart();
    VariousTimeProcess();
    return 0;
}
