#include <stdio.h>
#include <string.h>

typedef struct process {
    char pname[10];
    int burst;
    int at;
    int wt;
    int rt;
    int tat;
} p1;

int main() {
    p1 p[10], swap;
    float avg_tat = 0, avg_wt = 0, avg_rt = 0;
    int n, i, j;

    printf("\nENTER THE NUMBER OF PROCESSES: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        sprintf(p[i].pname, "P%d", i + 1); // Assign process name like P1, P2, ...
        printf("\nENTER THE BURST TIME FOR %s: ", p[i].pname);
        scanf("%d", &p[i].burst);
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

    // Calculate Waiting Time, Turnaround Time, and Response Time
    for (i = 0; i < n; i++) {
        p[i].wt = avg_rt;
        p[i].rt = p[i].wt;
        p[i].tat = p[i].burst + p[i].wt;
        avg_tat += p[i].tat;
        avg_rt += p[i].burst;
    }

    // Calculate average waiting and turnaround times
    avg_wt = 0;
    for (i = 0; i < n; i++) {
        avg_wt += p[i].wt;
    }
    avg_wt /= n;
    avg_tat /= n;

    // Display results
    printf("\nPROCESS\tAT\tBT\tWT\tTAT");
    for (i = 0; i < n; i++) {
        printf("\n%s\t%d\t%d\t%d\t%d", p[i].pname, p[i].at, p[i].burst, p[i].wt, p[i].tat);
    }

    printf("\n\nAVERAGE WAITING TIME: %.2f", avg_wt);
    printf("\nAVERAGE TURNAROUND TIME: %.2f\n", avg_tat);

    return 0;
}
