#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define NODE_SIZE(NODE) ((int*) NODE)[0]


typedef enum {
    LL_INT = sizeof(int),
    LL_CHAR = sizeof(char),
    LL_UINT64 = sizeof(unsigned long long)
} DataType;


typedef struct LinkedList {
    char* root;
    int size;
} LinkedList;


// NODE REPRESENTATION
// [{SIZE(INT)}, {*NEXT(sizeof(void*))}, {VALUE}]

void* get_value_of_node(char* node){
    return (void*) &node[sizeof(int)+sizeof(void*)];
}

bool set_value_of_node(char* node, void* value){
    return (memcpy(&node[sizeof(int) + sizeof(void*)], value, NODE_SIZE(node))) != NULL;
}

void set_next_of_node(char* node, char* next){
    ((int*) node)[1] = next;
}

char* get_next_of_node(char* node){
    int val = ((int*) node)[1];
    return (char*) val;
}


char* create_node(DataType type, void* value){
    char* p = (char*) calloc(sizeof(int) + sizeof(void*) + type, sizeof(char));
    if (p == NULL){
        return NULL;
    }
    
    int* z = (int*) p;
    ((int*) p)[0] = type;


    if (!set_value_of_node(p, value)){
        free(p);
        return NULL;
    }

    return p;
}

char* append(LinkedList* linkedList, DataType type, void* value){
    char* node;
    if (linkedList->root == NULL){
        node = create_node(type, value);
        linkedList->root = node;
    }else{
        char* last = linkedList->root;

        while(get_next_of_node(last) != NULL){
            last = get_next_of_node(last);
        }
        
        node = create_node(type, value);
        set_next_of_node(last, node);
    }
    if (node == NULL){
        return NULL;
    }
    linkedList->size++;
    return node;
}

void destroy(LinkedList* linkedList){
    char* last = linkedList->root;
    do {
        char* next = get_next_of_node(last);
        free(last);
        last = next;
    } while(last != NULL);
    linkedList->root = NULL;
}


bool get(LinkedList* linkedList, int index, void* valuePtr){
    if (index < 0){
        return false;
    }
    
    char* node = linkedList->root;
    for (int i = 0; i < index; i++){
        if (node != NULL){
            node = get_next_of_node(node);
        } else {
            return false;
        }
    }

    if (node != NULL){
        void* nodeValue = get_value_of_node(node);

        // Cast the pointers appropriately and assign the value directly
        switch (NODE_SIZE(node)) {
            case LL_INT:
                *((int*)valuePtr) = *((int*)nodeValue);
                break;
            case LL_CHAR:
                *((char*)valuePtr) = *((char*)nodeValue);
                break;
            case LL_UINT64:
                *((unsigned long long*)valuePtr) = *((unsigned long long*)nodeValue);
                break;
            // Add more cases for other data types if needed
            default:
                return false;  // Unsupported data type
        }

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

    int valueFromList = 0;
    int* valuePtr = &valueFromList;
    if(get(&list, 0, valuePtr)){
        printf("i=0, value=%d \n", *valuePtr);
    } else {
        printf("Failed to get value at index 0\n");
    }

    if(get(&list, 1, valuePtr)){
        printf("i=1, updated value=%d \n", *valuePtr);
    } else {
        printf("Failed to get value at index 1\n");
    }

    unsigned long long getV2 = 0;
    unsigned long long* getV2Pointer = &getV2;
    if(get(&list, 2, getV2Pointer)){
        printf("i=2, value=%llu \n", *getV2Pointer);
    }


    return 0;
}


