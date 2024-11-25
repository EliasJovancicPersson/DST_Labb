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


int main(){
    struct LinkedList *list = NULL;

    // Insert some elements into the list
    for (int i = 0; i < 5; i++) {
        struct LinkedList *newElem = readSensor(i);
        insertFirst(&list, newElem);
    }

    // Print the original list
    printf("Original list:\n");
    printList(list);

    // Sort the list by sensorData
    printList(sortByData(&list));
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

struct LinkedList* sortByData(struct LinkedList **first){

    struct LinkedList* sorted = NULL;

    while (*first != NULL)
    {
        struct LinkedList* curr = *first;
        struct LinkedList* prev = NULL;
        struct LinkedList* min = *first;

        while (curr->next != NULL){
            if(curr->next->sensorData < min->sensorData){
                min = curr->next;
            }
            curr = curr->next;
        }

        insertFirst(&sorted,min);
        removeElem(first,min);
    }
    
    return sorted;
}

