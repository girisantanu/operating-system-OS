// wap in c that emulates the fcfs processscheduling algorithm it should accept information about a maximum of 10 process. that information includes arrival time , burst time, Order(to break ties, if process have same at).The program should printthe gannt chart,'response time,waiting time & turn around time of each, process//

#include <stdio.h>
#include <stdlib.h>

struct pcb
{
    int at, bt, ord, rem, rt, wt, tat, term;
};

int allProcessTerminated(int n, struct pcb arr[n])
{
    int i;
    for (i = 0; i < n; i++)
        if (!arr[i].term)
            return 0;

    return 1;
}

int nextProcessToRun(int n, struct pcb arr[n], int ct)
{
    int min_val = 100;
    int x = -1, i;
    for (i = 0; i < n; ++i)
        if (arr[i].at <= ct && !arr[i].term)
            if (arr[i].ord < min_val)
            {
                min_val = arr[i].ord;
                x = i;
            }
    return x;
}

int main()
{
    int n, i;
    printf("Enter the number of process:");
    scanf("%d", &n);
    struct pcb arr[n];

    for (i = 0; i < n; i++)
    {
        printf("\nEnter AT:");
        scanf("%d", &arr[i].at);
        printf("\nEnter BT:");
        scanf("%d", &arr[i].bt);
        printf("\nEnter Order:");
        scanf("%d", &arr[i].ord);
        arr[i].rem = arr[i].bt;
        arr[i].wt = 0;
    }

    int ct = 0;
    while (!allProcessTerminated(n, arr))
    {
        int x = nextProcessToRun(n, arr, ct);
        if (x == -1)
        {
            printf("t=%d  Idle", ct);
            ++ct;
            continue;
        }
        if (arr[x].rem == arr[x].bt)
            arr[x].rt = ct - arr[x].at;
        --arr[x].rem;

        if (arr[x].rem == 0)
        {
            arr[x].tat = (ct + 1) - arr[x].at;
            arr[x].term = 1;
        }
        for (int i = 0; i < n; ++i)
        {
            if (x == i)
                continue;
            if (arr[i].at <= ct && !arr[i].term)
                ++arr[i].wt;
        }

        printf("t=%d p=%d\n", ct, x);
        ++ct;
    }
    for (i = 0; i < n; i++)
    {
        printf("RT of P%d: %d\n", i, arr[i].rt);
        printf("WT of P%d: %d\n", i, arr[i].wt);
        printf("TAT of p%d: %d\n", i, arr[i].tat);
    }
    return 0;
}