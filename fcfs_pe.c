/*
WAP that emulate the fcfs process scheduling algorithm
it should accept information about the maximum of io process 
that info includes arrival time burst time order >the program should print the gantt chart , 
response time waiting time and the turn around time of each rpocess
*/
#include<stdio.h>

struct process
{
    int at,bt,order,rem,rt,wt,tt,terminated;
    
}pr[10]; 

int allprocessesTerminated(int n ,struct process pr[10])
{
    int i;
    for(i=0;i<n;i++)
    {
        if(!pr[i].terminated)
        {
            return 0;
        }
    }
    return 1;
}

int nextprocessTorun(int n , struct process pr[10] , int ct)
{
    int min_val=100;
    int x=-1,i;
    for(i=0;i<n;i++)
    {
        if(pr[i].at<=ct && !pr[i].terminated)
        {
            if(pr[i].order < min_val)
            {
                min_val = pr[i].order;
                x=i;
            }
        }
    }
    return x;
}

int main()
{
    int i,j,n;
    printf("Enter the number of processes : ");
    scanf("%d",&n);
    for(i=0;i<n;i++)
    {
        printf("PROCESS P(%d)\n",i);
        printf("Enter the arrival time ( AT ) : ");
        scanf("%d",&pr[i].at);
        printf("Enter the burst time ( BT ) : ");
        scanf("%d",&pr[i].bt);
        printf("Enter the order : ");
        scanf("%d",&pr[i].order);
        pr[i].rem=pr[i].bt;
        pr[i].wt=0;
    }
    int ct=0;
    while(!allprocessesTerminated(n,pr))
    {
        int x=nextprocessTorun(n,pr,ct);
        if(x==-1)
        {
            printf("t=%d IDLE\n",ct);
            ++ct;
            continue;
        }
        if(pr[x].rem == pr[x].bt)
        {
            pr[x].rt = ct - pr[x].at;
        }
        --pr[x].rem;
        if(pr[x].rem == 0)
        {
            pr[x].tt = (ct+1) - pr[x].at;
            pr[x].terminated = 1;
        }
        for(i=0;i<n;i++)
        {
            if(x == i)
            {
                continue;
            }
            if(pr[i].at<=ct && !pr[i].terminated)
            {
                ++pr[i].wt;
            }
        }
        printf("t=%d p%d\n",ct,x);
        ++ct;
    }
    for(i=0;i<n;i++)
    {
        printf("PROCESS p%d\n",i+1);
        printf("RT : %d\n",pr[i].rt);
        printf("WT : %d\n",pr[i].wt);
        printf("TT : %d\n\n",pr[i].tt);
    }
    return 0;
}