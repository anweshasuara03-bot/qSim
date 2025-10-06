#ifndef QSIM_H
#define QSIM_H

#include <stdio.h>
#include <stdlib.h>

// ---- Constants ----
#define MAX_CUSTOMERS 1000
#define MAX_TELLERS 20

// ---- Structures ----

// Customer structure
typedef struct {
    float arrivalTime;
    float serviceTime;
    float waitTime;
    float startTime;
    float endTime;
} Customer;

// Teller structure
typedef struct {
    int id;
    float idleTime;
    float busyTime;
} Teller;

// Event structure
typedef struct Event {
    float time;
    void (*action)(struct Event*);  // function pointer
    void *actor;                    // can point to customer/teller
    struct Event *next;
} Event;

// Queue for tellers (linked list of customers)
typedef struct QueueNode {
    Customer *cust;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int length;
} TellerQueue;

// ---- Function prototypes ----
void simulate(int numCustomers, int numTellers, float simTime, float avgService);

// Queue functions
void enqueue(TellerQueue *q, Customer *cust);
Customer* dequeue(TellerQueue *q);
int isEmpty(TellerQueue *q);

// Event queue functions
void addEvent(Event **head, Event *newEvent);
Event* popEvent(Event **head);

#endif   
