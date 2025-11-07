#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main() {
    FILE *f1, *f2, *f3, *f4;
    char label[100], code[100], operand[100];
    char du_label[100], du_code[100], du_operand[100], ala[100];
    int mntc, mdtc, i = 0, j = 0, k = 0, flag = 0;

    f1 = fopen("output_file2.txt", "w");  // Final expanded output
    f2 = fopen("mnt_table.txt", "r");     // Macro Name Table
    f3 = fopen("mdt_table.txt", "r");     // Macro Definition Table
    f4 = fopen("output_file1.txt", "r");  // Intermediate file after Pass 1

    if (!f1 || !f2 || !f3 || !f4) {
        printf("Error opening one or more files.\n");
        exit(1);
    }

    while (fscanf(f4, "%s%s%s", label, code, operand) != EOF) {
        while (fscanf(f2, "%d%s%s%s%d", &mntc, du_label, du_code, du_operand, &mdtc) != EOF) {
            // Search for macro name in MNT
            if (strcmp(code, du_code) == 0) {
                flag = 1; // Match found
                break;
            }
        }

        if (flag == 1) { // Macro expansion begins
            i = 0;
            j = 0;

            // Setup ALA (Argument List Array)
            while (operand[i] != '\0') {
                ala[j++] = operand[i++];
                if (operand[i] == ',') {
                    i++;
                    ala[j++] = ' ';
                }
            }
            ala[j] = ' ';

            // Move MDT pointer to the macro definition
            for (i = 1; i <= mdtc; i++)
                fscanf(f3, "%d%s%s%s", &j, du_label, du_code, du_operand);

            while (!(strcmp(du_code, "MEND") == 0)) {
                fscanf(f3, "%d%s%s%s", &mdtc, du_label, du_code, du_operand);
                i = 0;

                // Substitute arguments from ALA into MDT
                while (du_operand[i] != '\0') {
                    if (du_operand[i] == '#') {
                        k = ((int)du_operand[i + 1]) - 48; // Convert #index to integer
                        j = 0;
                        while (k != 0) {
                            if (ala[j] == ' ')
                                k = k - 1;
                            j++;
                        }

                        // Replace with corresponding argument
                        while (ala[j] != ' ')
                            du_operand[i++] = ala[j++];
                        du_operand[i] = '\0';
                    } else {
                        i++;
                    }
                }

                if (!(strcmp(du_code, "MEND") == 0))
                    fprintf(f1, "%s\t%s\t%s\n", du_label, du_code, du_operand);
            }
        } else {
            // Not a macro call — copy as is
            fprintf(f1, "%s\t%s\t%s\n", label, code, operand);
        }

        flag = 0;
        fseek(f2, 0, SEEK_SET);
        fseek(f3, 0, SEEK_SET);
    }

    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f4);

    printf("\nMacro Pass 2 processing completed. Output written to output_file2.txt\n");
}
