#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

typedef struct {
    char line[MAX_LINE_LENGTH];
    int age;
} ParticipantData;

typedef struct ParticipantNode {
    char line[MAX_LINE_LENGTH];
    struct ParticipantNode *next;
} ParticipantNode;

typedef struct Data {
    int tagNumber;
    char registrationDate[11];
    char fee[10];
    char name[20];
    int age;
    char organization[50];
    char occupation[20];
} Data;

int countLinesInFile(char []);
FILE *openFile(char [], char []);

ParticipantData *projectOne(char [], char [], int);
ParticipantData *allocateParticipantDataArray(int);
void readFileAndStoreData(char [], ParticipantData *, int, int);
int extractAgeIndex(char *, int);
void sortParticipantDataByAge(ParticipantData *, int);
void writeSortedDataToFile(char [], ParticipantData *, int);

ParticipantNode *projectTwo(char [], ParticipantData *);
ParticipantNode *allocateParticipantNodes(int);
void assignDataToNodes(ParticipantData *, ParticipantNode *, int);
void printLinkedList(ParticipantNode *);
void freeLinkedList(ParticipantNode *);

Data *projectThree(ParticipantNode *, int);
Data *allocateParticipantData();
void extractYesFeeParticipants(ParticipantNode *, Data *, int);
char *extractFeeIndex(char *, int);
void readParticipantData(Data *, char *);
void printParticipantData(Data *);

void freeAllDynamicMemory(ParticipantData *, ParticipantNode *, Data *);

int main() {
    char inputFileName[] = "registration_data.txt";
    char outputFileName[] = "sorted_data.txt";
    int ageFieldIndex = 5, feeFiledIndex = 3;

    ParticipantData *participants = projectOne(inputFileName, outputFileName, ageFieldIndex);

    ParticipantNode *head = projectTwo(inputFileName, participants);

    Data *data = projectThree(head, feeFiledIndex);

    freeAllDynamicMemory(participants, head, data);
    
    return 0;
}

ParticipantData *projectOne(char inputFileName[], char outputFileName[], int ageFieldIndex) {
    int numberOfLines = countLinesInFile(inputFileName);

    ParticipantData *participants = allocateParticipantDataArray(numberOfLines);

    readFileAndStoreData(inputFileName, participants, numberOfLines, ageFieldIndex);

    sortParticipantDataByAge(participants, numberOfLines);

    writeSortedDataToFile(outputFileName, participants, numberOfLines);
    
    return participants;
}

int countLinesInFile(char inputFileName[]) {
    char line[MAX_LINE_LENGTH];
    int numberOfLines = 0;

    FILE *file = openFile(inputFileName, "r");

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) numberOfLines++;
    
    fclose(file);

    return numberOfLines;
}

FILE *openFile(char fileName[], char mode[]) {
    FILE *file = fopen(fileName, mode);

    if (!file) {
        printf("Error: Cannot open '%s'.\n", fileName);
        exit(1);
    }

    return file;
}

ParticipantData *allocateParticipantDataArray(int numberOfLines) {
    ParticipantData *participants = malloc(numberOfLines * sizeof(ParticipantData));

    if (!participants) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    return participants;
}

void readFileAndStoreData(char inputFileName[], ParticipantData *participants, int numberOfLines, int ageFieldIndex) {
    char line[MAX_LINE_LENGTH];

    FILE *file = openFile(inputFileName, "r");

    for (int i = 0; i < numberOfLines; i++) {
        fgets(line, MAX_LINE_LENGTH, file);
        strcpy(participants[i].line, line);
        participants[i].age = extractAgeIndex(line, ageFieldIndex);
    }

    fclose(file);
}

int extractAgeIndex(char *line, int index) {
    char *lineCopy = strdup(line);
    if (lineCopy == NULL) return NULL;
    
    const char delimiter[2] = "/";
    char *token = strtok(line, delimiter);

    for (int i = 0; i < (index - 1); i++) token = strtok(NULL, delimiter);

    char *result = strdup(token);
    free(lineCopy);

    return result;
}

void sortParticipantDataByAge(ParticipantData *participants, int numberOfLines) {
    for (int i = 0; i < numberOfLines - 1; i++) {
        for (int j = 0; j < numberOfLines - i - 1; j++) {
            if (participants[j].age > participants[j + 1].age) {
                ParticipantData temp = participants[j];
                participants[j] = participants[j + 1];
                participants[j + 1] = temp;
            }
        }
    }
}

void writeSortedDataToFile(char outputFileName[], ParticipantData *participants, int numberOfLines) {
    FILE *file = openFile(outputFileName, "w");

    for (int i = 0; i < numberOfLines; i++) fputs(participants[i].line, file);

    fclose(file);
}

ParticipantNode *projectTwo(char inputFileName[], ParticipantData *participants) {
    int numberOfLines = countLinesInFile(inputFileName);

    ParticipantNode *head = allocateParticipantNodes(numberOfLines);

    assignDataToNodes(participants, head, numberOfLines);

    // printLinkedList(head);

    return head;
}

ParticipantNode *allocateParticipantNodes(int numberOfLines) {
    ParticipantNode *head = NULL, *current = NULL;

    for (int i = 0; i < numberOfLines; i++) {
        ParticipantNode *newNode = (ParticipantNode *)malloc(sizeof(ParticipantNode));
        if (!newNode) {
            printf("Error: Memory allocation failed.\n");
            exit(1);
        }
        if (i == 0) head = newNode;
        else (*current).next = newNode;
        current = newNode;
    }
    if (current != NULL) (*current).next = NULL;

    return head;
}

void assignDataToNodes(ParticipantData *participants, ParticipantNode *head, int numberOfLines) {
    ParticipantNode *current = head;

    for (int i = 0; i < numberOfLines; i++) {
        strcpy((*current).line, participants[i].line);
        current = (*current).next;
    }
}

void printLinkedList(ParticipantNode *head) {
    ParticipantNode *current = head;

    while (current != NULL) {
        printf("%s", (*current).line);
        current = (*current).next;
    }
}

void freeLinkedList(ParticipantNode *head) {
    ParticipantNode *tmp;
    while (head != NULL) {
        tmp = head;
        head = (*head).next;
        free(tmp);
    }
}

Data *projectThree(ParticipantNode *head, int feeFiledIndex) {
    Data *data = allocateParticipantData();

    extractYesFeeParticipants(head, data, feeFiledIndex);

    // free(data)

    return data;
}

Data *allocateParticipantData() {
    Data *data = malloc(sizeof(Data));

    if (!data) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    return data;
}

void extractYesFeeParticipants(ParticipantNode *head, Data *data, int feeFiledIndex) {
    ParticipantNode *current = (*head).next;
    char *tempFee;

    while (current != NULL) {
        tempFee = extractFeeIndex((*current).line, feeFiledIndex);

        if (!strcmp("yes", tempFee)) {
            readParticipantData(data, (*current).line);
            printParticipantData(data);
        }
        current = (*current).next;
    }
}

char *extractFeeIndex(char *line, int index) {
    char *lineCopy = strdup(line);
    if (lineCopy == NULL) return NULL;

    const char delimiter[2] = "/";
    char *token = strtok(lineCopy, delimiter);

    for (int i = 0; i < (index - 1); i++) token = strtok(NULL, delimiter);

    char *result = strdup(token);
    free(lineCopy);

    return result;
}

void readParticipantData(Data *data, char *line) {
    sscanf(line, "%d/%10[^/]/%3[^/]/%19[^/]/%d/%49[^/]/%19s", &((*data).tagNumber), (*data).registrationDate, (*data).fee, (*data).name, &((*data).age), (*data).organization, (*data).occupation);
}

void printParticipantData(Data *data) {
    printf("Tag Number: %d, Registration Date: %s, Fee: %s, Name: %s, Age: %d, Organization: %s, Occupation: %s\n", (*data).tagNumber, (*data).registrationDate, (*data).fee, (*data).name, (*data).age, (*data).organization, (*data).occupation);
}

void freeAllDynamicMemory(ParticipantData *participants, ParticipantNode *head, Data *data) {
    free(participants);
    freeLinkedList(head);
    free(data);
}
