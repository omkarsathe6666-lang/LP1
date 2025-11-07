#include <stdio.h>
#include <stdlib.h>

typedef struct frame {
    int data;
    int position;
} f;

int main() {
    f frame[10][10];
    int m, n;
    int i, j;
    int string[10];
    char pf[10];
    int flag;
    int index;

    printf("\nENTER THE NUMBER OF DATA IN REF. STRING : ");
    scanf("%d", &m);

    printf("\nENTER THE REF. STRING : ");
    for (i = 0; i < m; i++) {
        scanf("%d", &string[i]);
    }

    for (i = 0; i < m; i++) {
        pf[i] = ' ';
    }

    printf("\nENTER THE FRAME SIZE : ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++)
            frame[i][j].data = -1;
    }

    i = 0;

    while (i < m && j != (n - 1)) {
        flag = 0;
        if (i != 0) {
            for (j = 0; j < n; j++) {
                if (frame[j][i - 1].data == string[i]) {
                    flag = 1;
                    break;
                }
            }
        }

        if (flag != 1) {
            for (j = 0; j < n; j++) {
                pf[i] = '*';
                if (i != 0) {
                    frame[j][i] = frame[j][i - 1];
                }

                if (frame[j][i].data == -1) {
                    frame[j][i].data = string[i];
                    frame[j][i].position = i;
                    break;
                }
            }
        } else {
            for (j = 0; j < n; j++) {
                frame[j][i].data = frame[j][i - 1].data;
                frame[j][i].position = i;
            }
        }
        i++;
    }

    for (; i < m; i++) {
        flag = 0;
        for (j = 0; j < n; j++)
            if (frame[j][i - 1].data == string[i]) {
                flag = 1;
                break;
            }

        if (flag == 1)
            for (j = 0; j < n; j++)
                frame[j][i] = frame[j][i - 1];
        else {
            flag = 10000;
            for (j = 0; j < n; j++) {
                if (flag > frame[j][i - 1].position) {
                    flag = frame[j][i - 1].position;
                    index = j;
                }
            }
            for (j = 0; j < n; j++) {
                if (j != index)
                    frame[j][i] = frame[j][i - 1];
                else {
                    frame[j][i].data = string[i];
                    frame[j][i].position = i;
                    pf[i] = '*';
                }
            }
        }
    }

    printf("\n\nREF. STRING\t");
    for (i = 0; i < m; i++) {
        printf("  %d", string[i]);
    }

    printf("\n\n");

    for (i = 0; i < n; i++) {
        printf("\nFRAME[%d]\t", i);
        for (j = 0; j < m; j++) {
            if (frame[i][j].data == -1)
                printf("   ");
            else
                printf("  %d", frame[i][j].data);
        }
    }

    printf("\n\nPAGE FAULT\t");
    j = 0;
    for (i = 0; i < m; i++) {
        if (pf[i] == '*')
            j++;
        printf("  %c", pf[i]);
    }

    printf("\n\nTHUS THE NUMBER OF PAGE FAULTS : %d\n", j);
    return 0;
}
