#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Constants
#define MAX_CUSTOMERS 1000
#define MAX_TELLERS 20
#define MAX_QUEUE_LEN 1000

// Structures
typedef struct Customer
{
    int id;
    double arrivalTime;
    double serviceStart;
    double serviceEnd;
    struct Customer *next;
} Customer;

typedef struct Teller
{
    int id;
    double busyUntil;
    double totalService;
    double totalIdle;
    Customer *queueHead;
    Customer *queueTail;
} Teller;

typedef enum
{
    ARRIVAL,
    DEPARTURE,
    TELLER_IDLE
} EventType;

struct Event;
typedef void (*ActionFn)(struct Event *);

typedef struct Event
{
    double time;
    EventType type;
    Customer *customer;
    Teller *teller;
    ActionFn action;
    struct Event *next;
} Event;

// Global
Event *eventQueue = NULL;
int numCustomers, numTellers;
double simulationTime, avgServiceTime;
double totalWait = 0.0, totalWaitSq = 0.0, maxWait = 0.0;
int servedCount = 0;

// Helpers
double randBetween(double max)
{
    return max * ((double)rand() / RAND_MAX);
}

void pushEvent(Event *e)
{
    if (!eventQueue || e->time < eventQueue->time)
    {
        e->next = eventQueue;
        eventQueue = e;
        return;
    }
    Event *cur = eventQueue;
    while (cur->next && cur->next->time <= e->time)
        cur = cur->next;
    e->next = cur->next;
    cur->next = e;
}

Event *popEvent()
{
    if (!eventQueue)
        return NULL;
    Event *e = eventQueue;
    eventQueue = eventQueue->next;
    return e;
}

void enqueueCustomer(Teller *t, Customer *c)
{
    c->next = NULL;
    if (!t->queueHead)
    {
        t->queueHead = t->queueTail = c;
    }
    else
    {
        t->queueTail->next = c;
        t->queueTail = c;
    }
}

Customer *dequeueCustomer(Teller *t)
{
    if (!t->queueHead)
        return NULL;
    Customer *c = t->queueHead;
    t->queueHead = c->next;
    if (!t->queueHead)
        t->queueTail = NULL;
    return c;
}

Teller *getShortestQueue(Teller tellers[])
{
    int minLen = MAX_QUEUE_LEN, idx = 0;
    for (int i = 0; i < numTellers; i++)
    {
        int len = 0;
        Customer *tmp = tellers[i].queueHead;
        while (tmp)
        {
            len++;
            tmp = tmp->next;
        }
        if (len < minLen)
        {
            minLen = len;
            idx = i;
        }
    }
    return &tellers[idx];
}

// Event Actions
void handleArrival(Event *e)
{
    Teller *t = getShortestQueue(e->teller);
    enqueueCustomer(t, e->customer);

    if (t->busyUntil <= e->time)
    {
        Customer *c = dequeueCustomer(t);
        c->serviceStart = e->time;
        double serviceTime = randBetween(2 * avgServiceTime);
        c->serviceEnd = e->time + serviceTime;
        t->busyUntil = c->serviceEnd;
        t->totalService += serviceTime;

        Event *dep = (Event *)malloc(sizeof(Event));
        dep->time = c->serviceEnd;
        dep->type = DEPARTURE;
        dep->customer = c;
        dep->teller = t;
        dep->action = handleArrival; // not used, just placeholder
        dep->next = NULL;
        dep->action = NULL;
        pushEvent(dep);
    }
}

void handleDeparture(Event *e)
{
    Customer *c = e->customer;
    double wait = (c->serviceEnd - c->arrivalTime);
    totalWait += wait;
    totalWaitSq += wait * wait;
    if (wait > maxWait)
        maxWait = wait;
    servedCount++;
    free(c);
}

// Simulation
void runSimulation(int multiQueue)
{
    Teller tellers[MAX_TELLERS] = {0};
    eventQueue = NULL;
    totalWait = totalWaitSq = maxWait = 0;
    servedCount = 0;

    for (int i = 0; i < numTellers; i++)
    {
        tellers[i].id = i;
        tellers[i].queueHead = tellers[i].queueTail = NULL;
    }

    for (int i = 0; i < numCustomers; i++)
    {
        Customer *c = (Customer *)malloc(sizeof(Customer));
        c->id = i;
        c->arrivalTime = randBetween(simulationTime);
        c->next = NULL;

        Event *e = (Event *)malloc(sizeof(Event));
        e->time = c->arrivalTime;
        e->type = ARRIVAL;
        e->customer = c;
        e->teller = tellers;
        e->action = handleArrival;
        e->next = NULL;
        pushEvent(e);
    }

    while (eventQueue)
    {
        Event *e = popEvent();
        if (e->type == ARRIVAL && e->action)
            e->action(e);
        else if (e->type == DEPARTURE)
            handleDeparture(e);
        free(e);
    }

    double avg = totalWait / servedCount;
    double stddev = sqrt(totalWaitSq / servedCount - avg * avg);

    printf("\n--- Simulation Results (%s Queue) ---\n",
           multiQueue ? "Multiple" : "Single");
    printf("Customers served: %d\n", servedCount);
    printf("Tellers: %d\n", numTellers);
    printf("Average time in bank: %.2f\n", avg);
    printf("Std Dev: %.2f\n", stddev);
    printf("Max wait: %.2f\n", maxWait);
}

// Main
int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Usage: ./qSim #customers #tellers simulationTime avgServiceTime\n");
        return 1;
    }

    srand(time(NULL));
    numCustomers = atoi(argv[1]);
    numTellers = atoi(argv[2]);
    simulationTime = atof(argv[3]);
    avgServiceTime = atof(argv[4]);

    runSimulation(0); // Single queue
    runSimulation(1); // Multiple queues

    return 0;
}