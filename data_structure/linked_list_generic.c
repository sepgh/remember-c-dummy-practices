#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef enum {
    LL_INT = sizeof(int),
    LL_CHAR = sizeof(char),
    LL_UINT64 = sizeof(unsigned long long)
} DataType;


typedef struct Node {
    DataType type;
    void* value;
    size_t* next;
} Node;


typedef struct LinkedList {
    size_t* root;
    int size;
} LinkedList;


size_t* create_node(DataType type, void* value){
    size_t* p = malloc(sizeof(DataType) + sizeof(void*) + sizeof(size_t));  // TODO: find valid size
    if (p == NULL){
        return NULL;
    }

    *p = type;
    *(&((Node*) p)->next) = NULL;

    if(memcpy(&((Node*) p)->value, value, type) == NULL){
        free(p);
        return NULL;
    }

    return p;
}

size_t* append(LinkedList* linkedList, DataType type, void* value){
    size_t* node;
    if (linkedList->root == NULL){
        node = create_node(type, value);
        linkedList->root = node;
    }else{
        Node* last = (Node*) linkedList->root;
        while(last->next != NULL){
            last = (Node*) last->next;
        }
        node = create_node(type, value);
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
        last = next;
    } while(last != NULL);
    linkedList->root = NULL;
}


bool get(LinkedList* linkedList, int index, void* value){
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
        memcpy(value, &node->value, node->type);
        return true;
    }
    return false;
}


int main(void){
    LinkedList list = {};
    int v1 = 5;
    int v2 = 10;
    append(&list, LL_INT, &v1);
    append(&list, LL_INT, &v2);

    unsigned long long v3 = 64;
    if (append(&list, LL_UINT64, &v3) == NULL){
        printf("Failed to store UINT64\n");
    }

    int v = 0;
    if(get(&list, 0, &v)){
        printf("i=0, v=%d \n", v);
    }

    if(get(&list, 1, &v)){
        printf("i=1, v=%d \n", v);
    }

    unsigned long long get_v2 = 0;
    if(get(&list, 2, &get_v2)){
        printf("i=2, v=%llu \n", get_v2);
    }else{
        printf("WTF?\n");
    }

    printf("Can I get i=2? %c \n", get(&list, 2, &v) ? 'Y' : 'N');
    
    destroy(&list);
    printf("Can I get i=0 after destroy? %c \n", get(&list, 0, &v) ? 'Y' : 'N');


    return 0;
}


