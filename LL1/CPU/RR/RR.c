#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct process {
    char pname[10];
    int bt;     // Burst Time
    int at;     // Arrival Time
    int wt;     // Waiting Time
    int rt;     // Response Time
    int tat;    // Turnaround Time
} p1;

int main() {
    p1 p[10], swap, result[10], e[10];
    float avg_tat = 0, avg_wt = 0, avg_rt = 0;
    int n, i, j, k = 0, l = 0;
    int nextprocess = 0, total_burst = 0, realtime = 0, tq;

    printf("\nENTER THE NUMBER OF PROCESSES: ");
    scanf("%d", &n);

    printf("\nENTER THE TIME QUANTUM: ");
    scanf("%d", &tq);

    // Input process details
    for (i = 0; i < n; i++) {
        sprintf(p[i].pname, "P%d", i + 1);
        p[i].rt = -1;
        p[i].wt = 0;
        p[i].tat = 0;

        printf("\nENTER THE BURST TIME FOR %s: ", p[i].pname);
        scanf("%d", &p[i].bt);
        total_burst += p[i].bt;

        printf("ENTER THE ARRIVAL TIME FOR %s: ", p[i].pname);
        scanf("%d", &p[i].at);
    }

    // Sort processes by arrival time
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (p[i].at > p[j].at) {
                swap = p[i];
                p[i] = p[j];
                p[j] = swap;
            }
        }
    }

    // Round Robin scheduling simulation
    for (i = 0; i < n; i++) {
        e[l] = p[i];
        if (i != (n - 1))
            nextprocess = p[i + 1].at;
        else
            nextprocess = 10000;

        while (total_burst >= realtime && nextprocess > (realtime + tq)) {
            if (e[0].bt > tq) {
                e[0].wt += realtime - e[0].tat;

                if (e[0].rt == -1)
                    e[0].rt = realtime;

                realtime += tq;
                e[0].tat = realtime;
                e[0].bt -= tq;

                swap = e[0];
                for (j = 0; j <= (l - 1); j++)
                    e[j] = e[j + 1];
                e[j] = swap;
            } else {
                if (e[0].bt != 0) {
                    if (e[0].rt == -1)
                        e[0].rt = realtime;

                    e[0].wt += realtime - e[0].tat - e[0].at;
                    realtime += e[0].bt;
                    e[0].tat = realtime - e[0].at;
                }

                result[k] = e[0];
                k++;

                for (j = 0; j <= (l - 1); j++)
                    e[j] = e[j + 1];
                l = l - 1;
            }
        }
        l++;
    }

    // Calculate averages
    for (i = 0; i < n; i++) {
        avg_tat += result[i].tat;
        avg_rt += result[i].rt;
        avg_wt += result[i].wt;
    }

    avg_wt /= n;
    avg_tat /= n;
    avg_rt /= n;

    // Display results
    printf("\nAVERAGE WAITING TIME: %f", avg_wt);
    printf("\nAVERAGE TURNAROUND TIME: %f", avg_tat);
    printf("\nAVERAGE RESPONSE TIME: %f\n", avg_rt);

    return 0;
}
