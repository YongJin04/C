#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct PERSONAL {
    char name[10];
    int age;
    char hobby[10];
};

void file_size(char [], int *);
void save_file_data(struct PERSONAL [], char [], int);
void age_group(struct PERSONAL [], char [], int, int, int, int);

int main() {
    int line, startAge = 10, endAge = 50, range = 10;
    char inFileName[] = "personal.txt";
    char outFileName[] = "output.txt";

    file_size(inFileName, &line);
    struct PERSONAL personal[line];

    save_file_data(personal, inFileName, line);

    age_group(personal, outFileName, line, startAge, endAge, range);
 
    return 0;
}

void file_size(char inFileName[], int *line) {
    FILE *readFile = fopen(inFileName, "r");

    if (!readFile) {
        printf("Error: open %s file.\n", inFileName);
        exit(1);
    }

    *line = 0;
    char str[100];
    while (fgets(str, 100, readFile) != NULL) {
        (*line)++;
    }
    
    fclose(readFile);
}

void save_file_data(struct PERSONAL personal[], char inFileName[], int line) {
    FILE *readFile = fopen(inFileName, "r");

    if (!readFile) {
        printf("Error: open %s file.\n", inFileName);
        exit(1);
    }

    for (int i = 0; i <= line; i++) {
        fscanf(readFile, "%s %d %s", personal[i].name, &personal[i].age, personal[i].hobby);
    }
    
    fclose(readFile);
}

void age_group(struct PERSONAL personal[], char outFileName[], int line, int startAge, int endAge, int range) {
    FILE *writeFile = fopen(outFileName, "w");

    if (!writeFile) {
        fprintf(writeFile, "Error: open %s file.\n", outFileName);
        exit(1);
    }

    for (; startAge < endAge; startAge += range) {
        fprintf(writeFile, "\nAGE from %d to %d\n", startAge, startAge + 9);
        fprintf(writeFile, "--------------------------\n");
        for (int i = 0; i < line; i++) {
            if ((personal[i].age >= startAge) && (personal[i].age <= startAge + 9)) {
                fprintf(writeFile, "%s %d %s\n", personal[i].name, personal[i].age, personal[i].hobby);
            }
        }
    }

    printf("Hello world");

    fclose(writeFile);
}
