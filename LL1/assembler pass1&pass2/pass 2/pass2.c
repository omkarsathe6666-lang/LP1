#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20

struct symbol {
    char sym[10];
    int addr;
} S[MAX];

struct litab {
    char lit[10];
    int addr;
} L[MAX];

void print_file(char *);
void print_symtab();
void print_littab();

char optab[][6] = {"STOP", "ADD", "SUB", "MULT", "MOVER", "MOVEM", "COMP", "BC",
"DIV", "READ", "PRINT"};
char regtab[][5] = {"AREG", "BREG", "CREG", "DREG"};
char adtab[][7] = {"START", "END", "ORIGIN", "EQU", "LTORG"};
char condtab[][4] = {"LT", "LE", "EQ", "GT", "GE", "ANY"};

FILE *fs, *ft;
char buffer[80], source[80], tok1[10], tok2[10], tok3[10], tok4[10];
int lc = 0, sc = 0, poolcnt = 0, litcnt = 0;
int pooltab[10] = {0}; // Initialize pooltab[0] to zero

int search_optab(char *s) {
    int i;
    for (i = 0; i < 11; i++) {
        if (strcmp(optab[i], s) == 0) {
            return i;
        }
    }
    return -1;
}

int search_regtab(char *s) {
    int i;
    for (i = 0; i < 4; i++) {
        if (strcmp(regtab[i], s) == 0) {
            return i + 1;
        }
    }
    return -1;
}

int search_condtab(char *s) {
    int i;
    for (i = 0; i < 6; i++) {
        if (strcmp(condtab[i], s) == 0) {
            return (i + 1);
        }
    }
    return -1;
}

int search_adtab(char *s) {
    int i;
    for (i = 0; i < 5; i++) {
        if (strcmp(adtab[i], s) == 0) {
            return i + 1;
        }
    }
    return -1;
}

int search_symtab(char *s) {
    int i;
    for (i = 0; i < sc; i++) {
        if (strcmp(S[i].sym, s) == 0) {
            return i;
        }
    }
    return -1;
}

int search_littab(char *s) {
    int i;
    for (i = pooltab[poolcnt]; i < litcnt; i++) {
        if (strcmp(L[i].lit, s) == 0) {
            return i;
        }
    }
    return -1;
}

void pass1() {
    int p;
    int n, i = 0, j = 0, k = 0;

    fs = fopen(source, "r");
    if (fs == NULL) {
        printf("\nFile does not exist!!!!");
        exit(0);
    }

    ft = fopen("id.txt", "w");
    if (ft == NULL) {
        printf("\nCannot create intermediate file!!!");
        fclose(fs);
        exit(0);
    }

    while (fgets(buffer, 80, fs)) {
        n = sscanf(buffer, "%s %s %s %s", tok1, tok2, tok3, tok4);
        switch (n) {
            case 1: // ltorg, end
                i = search_adtab(tok1);
                if (i == 2 || i == 5) {
                    for (j = pooltab[poolcnt]; j < litcnt; j++) {
                        L[j].addr = lc++;
                    }
                    lc--;
                    pooltab[++poolcnt] = litcnt;
                    fprintf(ft, "(AD, %02d)\n", i);
                    break;
                }
                break;

            case 2: // start
                i = search_adtab(tok1);
                if (i == 1) {
                    lc = atoi(tok2) - 1;
                    fprintf(ft, "(AD, %02d)  (C, %s)\n", i, tok2);
                    break;
                }
                break;

            case 3:
                i = search_optab(tok1);
                if (i >= 1 && i <= 8) {
                    tok2[strlen(tok2) - 1] = '\0';
                    k = search_regtab(tok2);

                    // mover areg, ='5'
                    if (tok3[0] == '=') {
                        j = search_littab(tok3);
                        if (j == -1) {
                            strcpy(L[litcnt].lit, tok3);
                            fprintf(ft, "(IS, %02d)%d(L, %02d)\n", i, k, litcnt);
                            litcnt++;
                        } else {
                            fprintf(ft, "(IS, %02d) %d (L, %02d)\n", i, k, j);
                        }
                        break;

                    } else { // mover areg, A
                        p = search_symtab(tok3);
                        if (p == -1) {
                            strcpy(S[sc].sym, tok3);
                            fprintf(ft, "(IS, %02d) %d (S, %02d)\n", i, k, sc);
                            sc++;
                        } else {
                            fprintf(ft, "(IS, %02d) %d (S, %02d)\n", i, k, p);
                        }
                        break;
                    }
                }

                // A DS 2
                if (strcmp(tok2, "DS") == 0) {
                    p = search_symtab(tok1);
                    if (p == -1) {
                        strcpy(S[sc].sym, tok1);
                        S[sc].addr = lc;
                        fprintf(ft, "(DL, 2) (C, %s)\n", tok3);
                        sc++;
                    } else {
                        S[p].addr = lc;
                        fprintf(ft, "(DL, 2) (C, %s)\n", tok3);
                    }
                    lc = lc + atoi(tok3) - 1;
                    break;
                }
                break;

            default:
                break;
        }
        lc++;
    }

    fclose(fs);
    fclose(ft);
}

void print_file(char *target) {
    FILE *fp;
    fp = fopen(target, "r");
    if (fp == NULL) {
        printf("\nFile does not exist!!!");
        exit(0);
    }
    printf("\n\n");
    while (fgets(buffer, 80, fp)) {
        printf("%s", buffer);
    }
    fclose(fp);
}

void print_littab() {
    int i;
    printf("\nLITERAL\tADDRESS\n");
    for (i = 0; i < litcnt; i++) {
        printf("%s\t%d\n", L[i].lit, L[i].addr);
    }
}

void print_symtab() {
    int p = 0;
    printf("\nSYMBOL\tADDRESS\n");
    while (p < sc) {
        printf("%s\t%d\n", S[p].sym, S[p].addr);
        p = p + 1;
    }
}

int main() {
    printf("\nEnter source file name: \n");
    scanf("%s", source);

    printf("\nSource code is:\n");
    print_file(source);

    pass1();

    printf("\n\nLiteral table:\n");
    print_littab();

    printf("\n\nSymbol table:\n");
    print_symtab();

    printf("\nIntermediate code is:\n");
    print_file("id.txt");

    return 0;
}
