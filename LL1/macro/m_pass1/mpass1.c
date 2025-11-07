#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_TOKENS 3
#define MAX_TOKEN_LEN 200
#define MAX_PARAMS 50
#define MAX_PARAM_LEN 50

int main(void)
{
    FILE *f1, *f2, *f3, *f4;
    char label[MAX_TOKEN_LEN], code[MAX_TOKEN_LEN], operand[MAX_TOKEN_LEN];
    char param_list[MAX_PARAMS][MAX_PARAM_LEN]; /* stores parameter names (without &), e.g. "ARG" */
    int param_count = 0;

    int mntc = 1; /* Macro Name Table counter */
    int mdtc = 1; /* Macro Definition Table counter */

    /* Open files */
    f1 = fopen("input.txt", "r");
    if (!f1) {
        perror("Error opening input.txt");
        return 1;
    }
    f2 = fopen("mnt_table.txt", "w");
    if (!f2) {
        perror("Error opening mnt_table.txt");
        fclose(f1);
        return 1;
    }
    f3 = fopen("mdt_table.txt", "w");
    if (!f3) {
        perror("Error opening mdt_table.txt");
        fclose(f1);
        fclose(f2);
        return 1;
    }
    f4 = fopen("output_file1.txt", "w");
    if (!f4) {
        perror("Error opening output_file1.txt");
        fclose(f1);
        fclose(f2);
        fclose(f3);
        return 1;
    }

    /* Read input tokens line by line (label code operand) */
    while (fscanf(f1, "%s %s %s", label, code, operand) != EOF) {
        if (strcmp(code, "MACRO") == 0) {
            /* Next line should contain macro header: label (macro name), code (probably parameter list may be in operand) */
            if (fscanf(f1, "%s %s %s", label, code, operand) == EOF) {
                fprintf(stderr, "Unexpected EOF after MACRO\n");
                break;
            }

            /* Reset parameter list */
            param_count = 0;
            for (int i = 0; i < MAX_PARAMS; ++i) param_list[i][0] = '\0';

            /* operand is expected to be parameter list like &A,&B,&C  OR ( &A,&B ) depending on input format.
               We'll extract tokens that start with '&' and capture the name (skip initial '&') */
            {
                char tmp[MAX_TOKEN_LEN];
                strncpy(tmp, operand, MAX_TOKEN_LEN - 1);
                tmp[MAX_TOKEN_LEN - 1] = '\0';

                /* Replace parentheses with commas/space if present to simplify parsing */
                for (int i = 0; tmp[i] != '\0'; ++i) {
                    if (tmp[i] == '(' || tmp[i] == ')' ) tmp[i] = ' ';
                }

                char *tok = strtok(tmp, ", \t");
                while (tok != NULL) {
                    if (tok[0] == '&') {
                        /* store without '&' */
                        strncpy(param_list[param_count], tok + 1, MAX_PARAM_LEN - 1);
                        param_list[param_count][MAX_PARAM_LEN - 1] = '\0';
                        param_count++;
                        if (param_count >= MAX_PARAMS) break;
                    }
                    tok = strtok(NULL, ", \t");
                }
            }

            /* Write entry to MNT: mntc, macro name (label), code, operand (original header), mdt index */
            fprintf(f2, "%d\t%s\t%s\t%s\t%d\n", mntc, label, code, operand, mdtc);
            /* Also add first MDT entry with the header (for reference) */
            fprintf(f3, "%d\t%s\t%s\t%s\n", mdtc, label, code, operand);
            mntc++;
            mdtc++;

            /* Now read macro body lines until MEND is encountered.
               For each operand in body replace occurrences of &param with #index */
            while (1) {
                /* Reset tokens */
                if (fscanf(f1, "%s %s %s", label, code, operand) == EOF) {
                    fprintf(stderr, "Unexpected EOF while reading macro body for %s\n", label);
                    break;
                }

                /* If this line is MEND, write it to MDT and break */
                if (strcmp(code, "MEND") == 0) {
                    fprintf(f3, "%d\t%s\t%s\t%s\n", mdtc, label, code, operand);
                    mdtc++;
                    break;
                }

                /* Substitute any &param in operand with #index (index starts from 0) */
                char new_operand[MAX_TOKEN_LEN];
                int ni = 0;
                int oi = 0;
                while (operand[oi] != '\0' && ni < (MAX_TOKEN_LEN - 1)) {
                    if (operand[oi] == '&') {
                        /* read parameter name */
                        oi++;
                        char pname[MAX_PARAM_LEN];
                        int pj = 0;
                        while (operand[oi] != '\0' && operand[oi] != ',' && operand[oi] != ')' && operand[oi] != '('
                               && operand[oi] != ' ' && operand[oi] != '\t') {
                            if (pj < (MAX_PARAM_LEN - 1)) pname[pj++] = operand[oi];
                            oi++;
                        }
                        pname[pj] = '\0';

                        /* find parameter index */
                        int found = -1;
                        for (int p = 0; p < param_count; ++p) {
                            if (strcmp(param_list[p], pname) == 0) {
                                found = p;
                                break;
                            }
                        }
                        if (found != -1) {
                            /* write '#' + digit(s) */
                            int written = snprintf(new_operand + ni, MAX_TOKEN_LEN - ni, "#%d", found);
                            if (written < 0) written = 0;
                            ni += written;
                        } else {
                            /* if not found, keep as &pname (fallback) */
                            int written = snprintf(new_operand + ni, MAX_TOKEN_LEN - ni, "&%s", pname);
                            if (written < 0) written = 0;
                            ni += written;
                        }
                    } else {
                        new_operand[ni++] = operand[oi++];
                    }
                }
                new_operand[ni] = '\0';

                /* Write line to MDT with substituted operand */
                fprintf(f3, "%d\t%s\t%s\t%s\n", mdtc, label, code, new_operand);
                mdtc++;
            } /* end macro body reading loop */
        } else {
            /* Not a MACRO — write the token triplet directly to output_file1 */
            fprintf(f4, "%s\t%s\t%s\n", label, code, operand);
        }
    } /* end while fscanf */

    /* Close files */
    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f4);

    printf("Macro pass1 completed. MNT written to mnt_table.txt, MDT to mdt_table.txt, non-macro lines to output_file1.txt\n");
    return 0;
}
