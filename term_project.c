#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

// Structure to store a single participant's data from the file
typedef struct {
    char line[MAX_LINE_LENGTH]; // Original line from the file
    int age;                    // Extracted age of the participant
} ParticipantData;

// Node structure for linked list of participants
typedef struct ParticipantNode {
    char line[MAX_LINE_LENGTH]; // Original line from the file
    struct ParticipantNode *next; // Pointer to the next node in the list
} ParticipantNode;

// Structure to store detailed data of a participant
typedef struct Data {
    int tagNumber;                  // Tag number of the participant
    char registrationDate[11];      // Registration date
    char fee[10];                   // Fee paid status
    char name[20];                  // Participant's name
    int age;                        // Participant's age
    char organization[50];          // Participant's organization
    char occupation[20];            // Participant's occupation
} Data;

// Function declarations
int countLinesInFile(char []);
FILE *openFile(char [], char []);

ParticipantData *projectOne(char [], char [], int);
ParticipantData *allocateParticipantDataArray(int);
void readFileAndStoreData(char [], ParticipantData *, int, int);
int extractIntIndex(char *, int);
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
char *extractCharIndex(char *, int);
void readParticipantData(Data *, char *);
void printParticipantData(Data *);

void projectFour(ParticipantNode *, int);
void extractStaffIndex(ParticipantNode *, int);
ParticipantNode *deleteStaffNode(ParticipantNode *, ParticipantNode *);

ParticipantNode *projectFive(ParticipantNode *, char [], int);
ParticipantNode *insertKangInOrder(ParticipantNode *, char [], int);

void freeAllDynamicMemory(ParticipantData *, ParticipantNode *, Data *);

int main() {
    // File names for input and output
    char inputFileName[] = "registration_data.txt";
    char outputFileName[] = "sorted_data.txt";

    // Field indices for age, fee, and occupation
    int ageFieldIndex = 5, feeFiledIndex = 3, occupationFiledIndex = 7;
    char kangData[] = "31/2022-11-30/yes/Kang/25/Gachon University/Student\n";

    // Project One: Reading data from file and sorting it
    ParticipantData *participants = projectOne(inputFileName, outputFileName, ageFieldIndex);

    // Project Two: Creating a linked list from the data
    ParticipantNode *head = projectTwo(inputFileName, participants);

    // Project Three: Extracting participants who have paid the fee
    Data *data = projectThree(head, feeFiledIndex);

    // Project Four: Removing staff members from the linked list
    projectFour(head, occupationFiledIndex);

    // Project Five: Inserting a new participant (Kang) into the list
    head = projectFive(head, kangData, ageFieldIndex);

    // Freeing all dynamically allocated memory
    freeAllDynamicMemory(participants, head, data);
    
    return 0;
}

// Main function to process a file and sort participant data by age.
ParticipantData *projectOne(char inputFileName[], char outputFileName[], int ageFieldIndex) {
    int numberOfLines = countLinesInFile(inputFileName);

    // Allocate memory for storing participant data.
    ParticipantData *participants = allocateParticipantDataArray(numberOfLines);

    // Read file and store data in the participants array.
    readFileAndStoreData(inputFileName, participants, numberOfLines, ageFieldIndex);

    // Sort the participants based on their age.
    sortParticipantDataByAge(participants, numberOfLines);

    // Write the sorted data back to a file.
    writeSortedDataToFile(outputFileName, participants, numberOfLines);
    
    return participants;
}

// Counts the number of lines in a given file.
int countLinesInFile(char inputFileName[]) {
    char line[MAX_LINE_LENGTH];
    int numberOfLines = 0;

    // Open the file for reading.
    FILE *file = openFile(inputFileName, "r");

    // Count each line in the file.
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) numberOfLines++;
    
    fclose(file);

    return numberOfLines;
}

// Opens a file with the given mode (e.g., read, write).
FILE *openFile(char fileName[], char mode[]) {
    FILE *file = fopen(fileName, mode);

    // Check if the file was successfully opened.
    if (!file) {
        printf("Error: Cannot open '%s'.\n", fileName);
        exit(1);
    }

    return file;
}

// Allocates memory for an array of ParticipantData.
ParticipantData *allocateParticipantDataArray(int numberOfLines) {
    // Allocate memory based on the number of lines.
    ParticipantData *participants = malloc(numberOfLines * sizeof(ParticipantData));

    // Check for successful memory allocation.
    if (!participants) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    return participants;
}

// Reads data from a file and stores it in the participants array.
void readFileAndStoreData(char inputFileName[], ParticipantData *participants, int numberOfLines, int ageFieldIndex) {
    char line[MAX_LINE_LENGTH];

    FILE *file = openFile(inputFileName, "r");

    // Read each line and store it along with the extracted age.
    for (int i = 0; i < numberOfLines; i++) {
        fgets(line, MAX_LINE_LENGTH, file);
        strcpy(participants[i].line, line);
        participants[i].age = extractIntIndex(line, ageFieldIndex);
    }

    fclose(file);
}

// Extracts an integer from a string at a given index.
int extractIntIndex(char *line, int index) {
    char *lineCopy = strdup(line);
    if (lineCopy == NULL) return -1;

    const char delimiter[2] = "/";
    char *token = strtok(lineCopy, delimiter);

    // Tokenize the string to find the integer at the specified index.
    for (int i = 0; i < (index - 1); i++) token = strtok(NULL, delimiter);

    int result = atoi(token);
    free(lineCopy); // Free the memory allocated for line copy.
    return result;
}

// Sorts the participant data by age using bubble sort.
void sortParticipantDataByAge(ParticipantData *participants, int numberOfLines) {
    for (int i = 0; i < numberOfLines - 1; i++) {
        for (int j = 0; j < numberOfLines - i - 1; j++) {
            if (participants[j].age > participants[j + 1].age) {
                // Swap the participants if they are in the wrong order.
                ParticipantData temp = participants[j];
                participants[j] = participants[j + 1];
                participants[j + 1] = temp;
            }
        }
    }
}

// Writes sorted participant data to a file.
void writeSortedDataToFile(char outputFileName[], ParticipantData *participants, int numberOfLines) {
    FILE *file = openFile(outputFileName, "w");

    // Write each line of participant data to the file.
    for (int i = 0; i < numberOfLines; i++) fputs(participants[i].line, file);

    fclose(file);
}

// Converts an array of ParticipantData to a linked list.
ParticipantNode *projectTwo(char inputFileName[], ParticipantData *participants) {
    int numberOfLines = countLinesInFile(inputFileName);

    // Allocate memory for the linked list nodes.
    ParticipantNode *head = allocateParticipantNodes(numberOfLines);

    // Assign data from the array to the linked list nodes.
    assignDataToNodes(participants, head, numberOfLines);

    // printLinkedList(head);

    return head;
}

// Allocates memory for a linked list of ParticipantNodes.
ParticipantNode *allocateParticipantNodes(int numberOfLines) {
    ParticipantNode *head = NULL, *prev = NULL;

    for (int i = 0; i < numberOfLines; i++) {
        // Create a new node.
        ParticipantNode *newNode = (ParticipantNode *)malloc(sizeof(ParticipantNode));
        if (!newNode) {
            printf("Error: Memory allocation failed.\n");
            exit(1);
        }
        newNode->next = NULL;

        // Link the new node to the list.
        if (prev != NULL) {
            prev->next = newNode;
        } else {
            head = newNode;
        }
        prev = newNode;
    }

    return head;
}

// Assigns participant data to each node in the linked list.
void assignDataToNodes(ParticipantData *participants, ParticipantNode *head, int numberOfLines) {
    ParticipantNode *current = head;

    for (int i = 0; i < numberOfLines; i++) {
        strcpy((*current).line, participants[i].line);
        current = (*current).next;
    }
}

// Prints all nodes in the linked list.
void printLinkedList(ParticipantNode *head) {
    ParticipantNode *current = head;

    while (current != NULL) {
        printf("%s", (*current).line);
        current = (*current).next;
    }
}

// Frees memory allocated for the linked list.
void freeLinkedList(ParticipantNode *head) {
    ParticipantNode *tmp;
    while (head != NULL) {
        tmp = head;
        head = (*head).next;
        free(tmp);
    }
}

// Processes participant data to extract and store information about participants who paid the fee.
Data *projectThree(ParticipantNode *head, int feeFiledIndex) {
    Data *data = allocateParticipantData();

    // Extract participants who paid the fee and store their information.
    extractYesFeeParticipants(head, data, feeFiledIndex);

    return data;
}

// Allocates memory for storing participant data.
Data *allocateParticipantData() {
    Data *data = malloc(sizeof(Data));

    if (!data) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    return data;
}

// Extracts data of participants who have paid the fee.
void extractYesFeeParticipants(ParticipantNode *head, Data *data, int feeFiledIndex) {
    ParticipantNode *current = (*head).next;
    char *tempFee;

    while (current != NULL) {
        tempFee = extractCharIndex((*current).line, feeFiledIndex);

        if (!strcmp("yes", tempFee)) {
            readParticipantData(data, (*current).line);
        }
        current = (*current).next;
    }
}

// Extracts a character array from a string at a given index.
char *extractCharIndex(char *line, int index) {
    char *lineCopy = strdup(line);
    if (lineCopy == NULL) return NULL;

    const char delimiter[2] = "/";
    char *token = strtok(lineCopy, delimiter);

    for (int i = 0; i < (index - 1); i++) {
        token = strtok(NULL, delimiter);
        if (token == NULL) break;
    }

    char *result = NULL;
    if (token != NULL) {
        result = strdup(token);
    }
    free(lineCopy);

    return result;
}

// Reads and stores participant data from a string.
void readParticipantData(Data *data, char *line) {
    sscanf(line, "%d/%10[^/]/%3[^/]/%19[^/]/%d/%49[^/]/%19s", &((*data).tagNumber), (*data).registrationDate, (*data).fee, (*data).name, &((*data).age), (*data).organization, (*data).occupation);
}

// Prints the participant data.
void printParticipantData(Data *data) {
    printf("Tag Number: %d, Registration Date: %s, Fee: %s, Name: %s, Age: %d, Organization: %s, Occupation: %s\n", (*data).tagNumber, (*data).registrationDate, (*data).fee, (*data).name, (*data).age, (*data).organization, (*data).occupation);
}

// Processes the linked list to remove participants with a specific occupation.
void projectFour(ParticipantNode *head, int occupationFiledIndex) {
    extractStaffIndex(head, occupationFiledIndex);
}

// Function to remove nodes with specific occupation ("staff") from a linked list
void extractStaffIndex(ParticipantNode *head, int occupationFiledIndex) {
    ParticipantNode *current = head, *prev = NULL;
    char *tempOccupation;

    while (current != NULL) {
        tempOccupation = extractCharIndex((*current).line, occupationFiledIndex);
        // Remove newline character from occupation string
        tempOccupation[strcspn(tempOccupation, "\n")] = 0;

        // If occupation is 'staff', delete the node
        if (strcmp(tempOccupation, "staff\r") == 0) {
            current = deleteStaffNode(current, prev);
        } else {
            prev = current;
            current = (*current).next;
        }
    }

    // Print the updated linked list
    // printLinkedList(head);
}

// Function to delete a 'staff' node from the linked list
ParticipantNode *deleteStaffNode(ParticipantNode *current, ParticipantNode *prev) {
    ParticipantNode *toDelete = current;
    // Link the previous node to the next node, skipping the current
    (*prev).next = (*current).next;
    // Free the memory of the deleted node
    free(toDelete);
    return (*prev).next;
}

// Function to insert a new node in order based on age
ParticipantNode *insertKangInOrder(ParticipantNode *head, char kangData[], int ageFieldIndex) {
    ParticipantNode *current = head, *prev = NULL;
    ParticipantNode *kangNode = (ParticipantNode *)malloc(sizeof(ParticipantNode));
    
    // Check for successful memory allocation
    if (!kangNode) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    // Set the data for the new node
    strcpy(kangNode->line, kangData);
    kangNode->next = NULL;

    // Extract age from the data and find the correct position for insertion
    int kangAge = extractIntIndex(kangNode->line, ageFieldIndex);
    while (current != NULL && extractIntIndex(current->line, ageFieldIndex) < kangAge) {
        prev = current;
        current = current->next;
    }

    // Insert the node in the correct position
    if (prev != NULL) {
        prev->next = kangNode;
    } else {
        head = kangNode;
    }
    kangNode->next = current;

    return head;
}

// Wrapper function for inserting a new node and updating the linked list
ParticipantNode *projectFive(ParticipantNode *head, char kangData[], int ageFieldIndex) {
    head = insertKangInOrder(head, kangData, ageFieldIndex);
    return head;
}

// Function to free all dynamically allocated memory
void freeAllDynamicMemory(ParticipantData *participants, ParticipantNode *head, Data *data) {
    free(participants);
    freeLinkedList(head);
    free(data);
}
