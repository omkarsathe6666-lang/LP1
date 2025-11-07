#include <stdio.h>

int main() {
    int nb, np; // nb = number of blocks, np = number of processes

    printf("Enter number of memory blocks: ");
    scanf("%d", &nb);

    int blockSize[nb];
    printf("Enter size of each block:\n");
    for (int i = 0; i < nb; i++) {
        printf("Block %d: ", i + 1);
        scanf("%d", &blockSize[i]);
    }

    printf("\nEnter number of processes: ");
    scanf("%d", &np);

    int processSize[np];
    printf("Enter size of each process:\n");
    for (int i = 0; i < np; i++) {
        printf("Process %d: ", i + 1);
        scanf("%d", &processSize[i]);
    }

    int allocation[np];
    for (int i = 0; i < np; i++)
        allocation[i] = -1; // initially unallocated

    // Best Fit allocation logic
    for (int i = 0; i < np; i++) {
        int bestIdx = -1;
        for (int j = 0; j < nb; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx])
                    bestIdx = j;
            }
        }

        // If a suitable block found
        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blockSize[bestIdx] -= processSize[i];
        }
    }

    // Display results
    printf("\n-----------------------------------------------");
    printf("\nProcess No.\tProcess Size\tBlock No.");
    printf("\n-----------------------------------------------\n");
    for (int i = 0; i < np; i++) {
        printf("P%d\t\t%d\t\t", i + 1, processSize[i]);
        if (allocation[i] != -1)
            printf("B%d", allocation[i] + 1);
        else
            printf("Not Allocated");
        printf("\n");
    }
    printf("-----------------------------------------------\n");

    return 0;
}
