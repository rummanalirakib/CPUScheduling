#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include<math.h>
int numProcesses, timeStart;
struct node {
    char processNumber[10];
    int arivalTime;
    int arivalTimeCheck;
    int burstTime;
    int burstTimeCheck;
    int exitTime;
    int turnAroundTime;
    int waitingTime;
    int priority;
} Process[1000];

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

int roundRobinFlag[1000];

int compareArrivalTime(const void *s1, const void *s2)
{
  struct node *e1 = (struct node *)s1;
  struct node *e2 = (struct node *)s2;
  if(e1->arivalTime == e2->arivalTime) return strcmp(e1->processNumber, e2->processNumber);
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

int checkAnotherProcess(int priority, int index)
{
    for(int i=0;i<numProcesses;i++)
    {
        if(Process[i].arivalTime<=timeStart && Process[i].burstTime!=0 && Process[i].priority<priority && i!=index){
            return 1;
        }
    }
    return 0;
}

void setThisProcessArrival(int processnumber, int priority)
{
    for(int i=processnumber+1;i<numProcesses;i++)
    {
        if(Process[i].arivalTime==timeStart && Process[i].priority<=priority){
            Process[processnumber].arivalTime=timeStart+1;
            return;
        }
    }
    Process[processnumber].arivalTime=timeStart;
}

void setAllTimeRelatedValues(int p)
{
    int trnArndTime=timeStart-Process[p].arivalTimeCheck;
    Process[p].exitTime=timeStart;
    Process[p].turnAroundTime=trnArndTime;
    Process[p].waitingTime=trnArndTime-Process[p].burstTimeCheck;
}

void RoundRobinExecution(int priority, int processnumber)
{
    if(roundRobinFlag[priority]==0){
      struct Queue *head = (struct Queue *) malloc (sizeof (struct Queue));
      struct Queue *tail = (struct Queue *) malloc (sizeof (struct Queue));
      tail->next=NULL;
      head=tail;
      roundRobinData[priority].top=head;
      struct Queue *newNode = (struct Queue *) malloc (sizeof (struct Queue));
      newNode->next = NULL;
      tail->val = processnumber;
      tail->next=newNode;
      roundRobinData[priority].bottom=tail;
    }
    else{
        struct Queue *tail=roundRobinData[priority].bottom;
        struct Queue *newNode = (struct Queue *) malloc (sizeof (struct Queue));
        newNode->next = NULL;
        tail->val = processnumber;
        tail->next=newNode;
        roundRobinData[priority].bottom=tail;
    }
    if(priority==1){
         struct Queue *head=roundRobinData[priority].top;
         int p=head->val;
         if(Process[p].burstTime>roundRobinData[priority].timeQuantum){
            Process[p].burstTime -= roundRobinData[priority].timeQuantum;
            printf("%d-%d\t\t\t%s\n",timeStart, timeStart+roundRobinData[priority].timeQuantum, Process[p].processNumber);
            timeStart += roundRobinData[priority].timeQuantum;
            head=head->next;
            roundRobinData[priority].top=head;
            setThisProcessArrival(processnumber,priority);
         }
         else{
            printf("%d-%d\t\t\t%s\n",timeStart, timeStart+Process[p].burstTime, Process[p].processNumber);
            timeStart+=Process[p].burstTime;
            Process[p].burstTime=0;
            head=head->next;
            roundRobinData[priority].top=head;
            setAllTimeRelatedValues(p);
         }
    }
    else{
        struct Queue *head=roundRobinData[priority].top;
        int p=head->val;
        int tempTimeStart = timeStart;
        int count1=0;
        while(1){
            if(Process[p].burstTime==0){
                printf("%d-%d\t\t\t%s\n",tempTimeStart, timeStart, Process[p].processNumber);
                head=head->next;
                roundRobinData[priority].top=head;
                setAllTimeRelatedValues(p);
                return;
            }
            else if(count1==roundRobinData[priority].timeQuantum){
                printf("%d-%d\t\t\t%s\n",tempTimeStart, timeStart, Process[p].processNumber);
                head=head->next;
                roundRobinData[priority].top=head;
                setThisProcessArrival(processnumber,priority);
                return;
            }
            else if(checkAnotherProcess(priority, p)){
                 printf("%d-%d\t\t\t%s\n",tempTimeStart, timeStart, Process[p].processNumber);
                 head=head->next;
                 roundRobinData[priority].top=head;
                 setThisProcessArrival(processnumber,priority);
                 return;
            }
            Process[p].burstTime -= 1;
            timeStart+=1;
            count1++;
        }
    }
}

void FCFS(int priority, int p)
{
    if(priority==1){
        printf("%d-%d\t\t\t%s\n",timeStart, timeStart+Process[p].burstTime, Process[p].processNumber);
        Process[p].burstTime=0;
        timeStart+=Process[p].burstTime;
        setAllTimeRelatedValues(p);
    }
    else{
        int tempTimeStart = timeStart;
        while(1){
            if(Process[p].burstTime==0){
                printf("%d-%d\t\t\t%s\n",tempTimeStart, timeStart, Process[p].processNumber);
                setAllTimeRelatedValues(p);
                return;
            }
            else if(checkAnotherProcess(priority, p)){
                 printf("%d-%d\t\t\t%s\n",tempTimeStart, timeStart, Process[p].processNumber);
                 return;
            }
            Process[p].burstTime -= 1;
            timeStart+=1;
        }
    }
}

int FindShortestJob(int qLevel)
{
    int index=-1, job=1e8;
    for(int i=0;i<numProcesses;i++)
    {
        if(job>Process[i].burstTime && Process[i].priority==qLevel && Process[i].burstTime!=0){
            index=i;
            job=Process[i].burstTime;
        }
    }

    return index;
}

void ShortestJobFirst(int priority, int qLevel)
{
    int p=FindShortestJob(qLevel);
    if(p>=0)
    {
        if(priority==1){
            printf("%d-%d\t\t\t%s\n",timeStart, timeStart+Process[p].burstTime, Process[p].processNumber);
            Process[p].burstTime=0;
            timeStart+=Process[p].burstTime;
            setAllTimeRelatedValues(p);
        }
        else{
            int tempTimeStart = timeStart;
            while(1){
                if(Process[p].burstTime==0){
                    printf("%d-%d\t\t\t%s\n",tempTimeStart, timeStart, Process[p].processNumber);
                    setAllTimeRelatedValues(p);
                    return;
                }
                else if(checkAnotherProcess(priority, p)){
                     printf("%d-%d\t\t\t%s\n",tempTimeStart, timeStart, Process[p].processNumber);
                     return;
                }
                Process[p].burstTime -= 1;
                timeStart+=1;
            }
        }
    }
}

void GanttChart()
{
    printf("Gantt Chart:\n");
    for(timeStart=0;timeStart<=totalBurstTime+10;timeStart++)
    {
        for(int j=1;j<=queueNumber;)
        {
            qsort(Process, numProcesses, sizeof(struct node), compareArrivalTime);
            int flag=0;
            for(int i=0;i<numProcesses;i++)
            {
                if(Process[i].priority==j)
                {
                    if(Process[i].burstTime>0 && Process[i].arivalTime<=timeStart){
                        if(QueueValue[j-1]==1){
                           RoundRobinExecution(j,i);
                        }
                        else if(QueueValue[j-1]==2){
                           ShortestJobFirst(j, 2);
                        }
                        else if(QueueValue[j-1]==3){
                           FCFS(j,i);
                        }
                        flag=1;
                        break;
                    }
                }
            }
            if(flag==1){
                j=1;
            }
            else{
                j++;
            }
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
        int priority;
        fscanf(file, "%s %d %d %d", &(processNumber), &(arivalTime), &(burstTime), &(priority));
        strcpy(Process[numProcesses].processNumber,processNumber);
        Process[numProcesses].arivalTime=arivalTime;
        Process[numProcesses].arivalTimeCheck=arivalTime;
        Process[numProcesses].burstTime=burstTime;
        Process[numProcesses].burstTimeCheck=burstTime;
        Process[numProcesses].priority=priority;
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
       Process[i].priority=rand()%queueNumber;
       if(Process[i].priority==0) Process[i].priority=1;
    }
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
        QueueValue[i]=val;
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
    numProcesses=0;
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
