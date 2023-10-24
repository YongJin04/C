#include <stdio.h>
#include <string.h>

#define LENGTH 4
#define SEARCH_LENGTH 2

struct PERSON {
    char name[10];
    int age;
    char hobby[20];
};

struct INDEX {
    char name[20];
    int index;
};

void create_index(struct PERSON [], struct INDEX []);

struct PERSON* search(struct PERSON [], struct INDEX [], char*);  // function prototype pointer

int main() {
    struct PERSON dataset[LENGTH] = { {"kim", 39, "tennis"},
                                      {"ko", 15, "soccer"},
                                      {"lee", 17, "baseball"},
                                      {"choi", 21, "basketball"} };

    struct INDEX index[LENGTH];

    create_index(dataset, index);

    char searchName[2][10] = { "kim", "lee" };  // two-dimensional array

    for (int i = 0; i < SEARCH_LENGTH; i++) {
        struct PERSON *idx = search(dataset, index, searchName[i]);  // Array -> the argument is passed to the address.
        if (idx) {  // value of "idx" -> valid address or NULL
            printf("Name: %s\n", (*idx).name);
            printf("Age: %d\n", (*idx).age);
            printf("Hobby: %s\n\n", (*idx).hobby);
        } else {
            printf("Non value\n");
        }
    }

    return 0;
}

void create_index(struct PERSON dataset_local[], struct INDEX index_local[]) {
    for (int i = 0; i < LENGTH; i++) {
        strcpy_s(index_local[i].name, dataset_local[i].name);
        index_local[i].index = i;
    }
}

struct PERSON* search(struct PERSON dataset_local[], struct INDEX index_local[], char* name) {  // receive address as a pointer, and variable name is fixed to "name" in local.
    for (int i = 0; i < LENGTH; i++) {
        if (!strcmp(index_local[i].name, name)) {  // local variable name is "name" fixed
            return &dataset_local[index_local[i].index];  // return pointer type, location of the dataset
        }
    }
    return NULL;  // NULL 반환
}
