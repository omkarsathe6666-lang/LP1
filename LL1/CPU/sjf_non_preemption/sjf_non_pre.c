#include <stdio.h>
#include <string.h>

typedef struct process {
    char pname[10];
    int burst;
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
        sprintf(p[i].pname, "P%d", i + 1);
        printf("\nENTER THE BURST TIME FOR %s: ", p[i].pname);
        scanf("%d", &p[i].burst);
    }

    // Sort processes by burst time (SJF)
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (p[i].burst > p[j].burst) {
                swap = p[i];
                p[i] = p[j];
                p[j] = swap;
            }
        }
    }

    // Calculate Waiting Time, Turnaround Time, Response Time
    for (i = 0; i < n; i++) {
        p[i].wt = avg_rt;
        p[i].rt = p[i].wt;
        p[i].tat = p[i].burst + p[i].wt;
        avg_tat += p[i].tat;
        avg_rt += p[i].burst;
    }

    // Calculate average waiting time
    avg_wt = 0;
    for (i = 0; i < n; i++) {
        avg_wt += p[i].wt;
    }

    avg_wt /= n;
    avg_tat /= n;

    // Display results
    printf("\nPROCESS\tBT\tWT\tTAT");
    for (i = 0; i < n; i++) {
        printf("\n%s\t%d\t%d\t%d", p[i].pname, p[i].burst, p[i].wt, p[i].tat);
    }

    printf("\n\nAVERAGE WAITING TIME: %.2f", avg_wt);
    printf("\nAVERAGE TURNAROUND TIME: %.2f\n", avg_tat);

    return 0;
}
