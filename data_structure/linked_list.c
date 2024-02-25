#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct Node {
    int value;
    size_t* next;
} Node;


typedef struct LinkedList {
    size_t* root;
    int size;
} LinkedList;


size_t* create_node(int value){
    size_t* p = malloc(sizeof(Node));
    if (p == NULL){
        return NULL;
    }

    *p = value;
    return p;
}

size_t* append(LinkedList* linkedList, int data){
    size_t* node;
    if (linkedList->root == NULL){
        node = create_node(data);
        linkedList->root = node;
    }else{
        Node* last = (Node*) linkedList->root;
        while(last->next != NULL){
            last = (Node*) last->next;
        }
        node = create_node(data);
        last->next = node;
    }
    if (node == NULL){
        return NULL;
    }
    linkedList->size++;
    return node;
}

void destroy(LinkedList* linkedList){
    Node* last = (Node*) linkedList->root;
    do {
        Node* next = (Node*) last->next;
        free((size_t*) last);
        last->next = NULL;
        last = next;
    } while(last != NULL);
    linkedList->root = NULL;
}


bool get(LinkedList* linkedList, int index, int* value){
    if (index < 0){
        return false;
    }
    
    Node* node = (Node*) linkedList->root;
    for (int i = 0; i < index; i++){
        if (node != NULL){
            node = (Node*) node->next;
        }else{
            return false;
        }
    }

    if (node != NULL){
        *value = node->value;
        return true;
    }
    return false;
}


void main(void){
    LinkedList list = {};
    append(&list, 5);
    append(&list, 10);

    int v;
    get(&list, 0, &v);
    printf("i=0, v=%d \n", v);

    get(&list, 1, &v);
    printf("i=1, v=%d \n", v);

    printf("Can I get i=2? %c \n", get(&list, 2, &v) ? 'Y' : 'N');
    
    destroy(&list);
    printf("Can I get i=0 after destroy? %c \n", get(&list, 0, &v) ? 'Y' : 'N');
}


