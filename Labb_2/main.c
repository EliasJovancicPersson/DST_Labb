#include <stdio.h>
#include <stdlib.h>

struct LinkedList {
    int id;
    double sensorData;
    struct LinkedList *next;    
};

void insertFirst( struct LinkedList **first, struct LinkedList *el);
int isMember (struct LinkedList **first, struct LinkedList *el);
void printList(struct LinkedList *first);
void removeElem(struct LinkedList **first, struct LinkedList *el);
struct LinkedList* readSensor(int id);
struct LinkedList* sortByData(struct LinkedList **first);
void freeList(struct LinkedList **first);



int main(){
    struct LinkedList *list = NULL;

    for (int i = 0; i < 10; i++) {
        struct LinkedList *newElem = readSensor(i);
        insertFirst(&list, newElem);
    }

    printf("Original list:\n");
    printList(list);

    printf("Sorted list:\n");
    printList(sortByData(&list));

    freeList(&list);
}

void insertFirst(struct LinkedList **first, struct LinkedList *el){
    el->next = *first;
    *first = el;
}

int isMember (struct LinkedList **first, struct LinkedList *el){
    struct LinkedList *curr = *first;
    while (curr != NULL){
        if(curr == el){
            return 1;
        }
        else{
            curr = curr->next;
        }
    }
    return 0;
}

void printList(struct LinkedList *first){
    struct LinkedList *curr = first;
    while (curr != NULL)
    {
        printf("id: %d, sensorData: %lf\n", curr->id, curr->sensorData);
        curr = curr->next;
    }
    
}

void removeElem(struct LinkedList **first, struct LinkedList *el){
    if(isMember(first,el) == 1){
        struct LinkedList *curr = *first;
        struct LinkedList *prev = NULL;
        while (curr != NULL)
        {
            if(curr == el){
                if(prev == NULL){
                    *first = curr->next;
                } else{
                    prev->next = curr->next;
                }
                el->next = NULL;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
        
    }
}

struct LinkedList* readSensor(int id){
    struct LinkedList *el = (struct LinkedList *)malloc(sizeof(struct LinkedList));
    if (el == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }
    el->sensorData = (double)rand()/RAND_MAX;
    el->id = id;
    el->next = NULL;
    return el;
} 

struct LinkedList* sortByData(struct LinkedList **first) { //could do selection sort with max instead to reverse the order of the list?
    struct LinkedList* sorted = NULL;

    while (*first != NULL) {
        struct LinkedList* curr = *first;
        struct LinkedList* min = *first;
        struct LinkedList* prev = NULL;
        struct LinkedList* minPrev = NULL;

        // Find node with smallest sensorData
        while (curr != NULL) {
            if (curr->sensorData < min->sensorData) {
                min = curr;
                minPrev = prev; // Track previous node of min
            }
            prev = curr;
            curr = curr->next;
        }

        // Remove min from the original list
        if (minPrev == NULL) { // min is first node
            *first = min->next;
        } else { 
            minPrev->next = min->next;
        }
        min->next = NULL; 

        insertFirst(&sorted, min);
    }

    return sorted;
}


void freeList(struct LinkedList **first) {
    struct LinkedList *current = *first;
    struct LinkedList *nextNode;

    while (current != NULL) {
        nextNode = current->next; // Save the next node
        free(current);            // Free the current node
        current = nextNode;       // Move to the next node
    }

    *first = NULL; // Ensure the list pointer is set to NULL after freeing
}


