#include <stdio.h>
#include <string.h>

#define LENGTH 5

struct NODE {
    int key;
    struct NODE* node;
};

struct NODE* searchKey(struct NODE*, int);

int main() {
    struct NODE node[LENGTH];

    int i;

    for (i = 0; i < LENGTH - 1; i++) {  // except last node
        node[i].node = &(node[i + 1]); // Fixed index from i + i to i + 1
    }
    node[i].node = NULL;  // assignment NULL value to last node

    for (i = 0; i < LENGTH; i++) {  // read key value
        printf("Enter the value of key: ");
        scanf_s("%d", &(node[i].key)); // Changed scanf_s to scanf
    }

    for (i = 0; i < LENGTH; i++) {
        struct NODE* key = searchKey(node, i);
        printf("\nnode%d's key value: %d", i + 1, (*key).key);
    }

    return 0;
}

struct NODE* searchKey(struct NODE* node_local, int count) {  
    if (count == 0) {
        return node_local;  
    }
    else {
        return searchKey((*node_local).node, count - 1);  // return struct -> only return the number of the structure in the array value (the value is address), it does not return what arguments are in the structure.
    }
}
