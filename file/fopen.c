#include <stdio.h>
#include <stdlib.h>

#define SIZE 20

int main() {
    FILE *readFile = fopen("inFile.txt", "r");  // road data type of FILE struct

    if (readFile == NULL) {  // safe code
        printf("Error: file open");
        exit(1);
    }

    // use: fscanf
    char name[10];
    int age;
    while (fscanf(readFile, "%s %d", name, &age) != EOF) {  // return: count of read data (use: EOF(-1))
        printf("%s %d", name, age);
    }
    printf("\n\n");
    fseek(readFile, 0, SEEK_SET);

    while (fscanf(readFile, "%s %d", name, &age) == 2) {  // return: count of read data (use: count)
        printf("%s %d", name, age);
    }
    printf("\n\n");
    fseek(readFile, 0, SEEK_SET);

    int i = 0;
    while (fscanf(readFile, "%s", name) != EOF) {  // return: count of read data (5 times)
        i++;
        printf("%s", name);
    }
    printf("%d\n\n", i);
    fseek(readFile, 0, SEEK_SET);

    // use: fgets
    char str[SIZE];
    while (fgets(str, SIZE, readFile) != NULL) {  // return: offset (available) or NULL
        puts(str);
    }
    printf("\n\n");
    fseek(readFile, 0, SEEK_SET);

    // use: fgetchar
    char c;
    while ((c = fgetc(readFile)) != EOF) {  // return: ASCII value(0~255) or EOF
        printf("%c", c);
    }
    printf("\n\n");
    fseek(readFile, 0, SEEK_SET);

    fclose(readFile);  // clear buffer about FILE struct

    return 0;
}
