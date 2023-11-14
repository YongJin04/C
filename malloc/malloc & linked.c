#include <stdio.h>
#include <stdlib.h>

struct NODE {
    int key;
    struct NODE *next;
};

struct NODE * create_node();
void assign_node(struct NODE *, struct NODE *, struct NODE *);
void print_node(struct NODE *, int);
void free_node(struct NODE *);

int main() {
    int nodeCount = 3;
    struct NODE *node0, *node1, *node2;

    node0 = create_node();
    node1 = create_node();
    node2 = create_node();

    assign_node(node0, node1, node2);

    print_node(node0, nodeCount);

    free_node(node0);
    free_node(node1);
    free_node(node2);

    printf("\n");

    return 0;
}

struct NODE * create_node() {
    struct NODE *node = (struct NODE *) malloc (sizeof(struct NODE));

    if (!node) {
        printf("Error: malloc\n");
        exit(1);
    }

    return node;
}

void assign_node(struct NODE *node0, struct NODE *node1, struct NODE *node2) {
    (*node0).key = 100;
    (*node0).next = node1;

    (*node1).key = 200;
    (*node1).next = node2;

    (*node2).key = 300;
    (*node2).next = NULL;
}

void print_node(struct NODE *node, int nodeCount) {
    for (int i = 0; i < nodeCount; i++) {
        printf("\nnode%d key value: %d", i, (*node).key);
        node = (*node).next;
    }
}

void free_node(struct NODE *node) {
    if (node != NULL) {
        free(node);
    }
}
