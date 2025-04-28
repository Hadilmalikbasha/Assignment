#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUEUE 100
#define MAX_STACK 100
#define MAX_LOG 6

// -------------------- Queue --------------------
// For managing delivery requests in FIFO manner
char queue[MAX_QUEUE][50];
int front = -1, rear = -1;

void enqueue() {
    if (rear == MAX_QUEUE - 1) {
        printf("Queue is full.\n");
        return;
    }
    char item[50];
    printf("Enter delivery item: ");
    getchar(); // flush leftover newline
    fgets(item, sizeof(item), stdin);
    item[strcspn(item, "\n")] = 0; // remove newline char

    if (front == -1) front = 0;
    strcpy(queue[++rear], item); // add to queue
    printf("Added to queue.\n");
}

// -------------------- Stack --------------------
// For urgent dispatches in LIFO order
char stack[MAX_STACK][50];
int top = -1;

void dispatchUrgent() {
    // Move everything from queue to stack
    while (front <= rear) {
        strcpy(stack[++top], queue[front++]);
    }
    front = rear = -1; // queue becomes empty

    // Now dispatch from stack (LIFO)
    printf("Dispatch Order (LIFO):\n");
    while (top != -1) {
        printf("Dispatched: %s\n", stack[top--]);
    }
}

// -------------------- Array Log --------------------
// Simple fixed-size log to track last few flights
char logArray[MAX_LOG][50];
int logSize = 0;

void logFlight() {
    char log[50];
    printf("Enter flight delivery log: ");
    getchar(); // flush newline
    fgets(log, sizeof(log), stdin);
    log[strcspn(log, "\n")] = 0;

    // If log is full, remove the oldest one (shift left)
    if (logSize == MAX_LOG) {
        for (int i = 1; i < MAX_LOG; i++) {
            strcpy(logArray[i - 1], logArray[i]);
        }
        logSize--;
    }
    strcpy(logArray[logSize++], log); // add new log
    printf("Logged successfully.\n");
}

void showLogs() {
    printf("Flight Log:\n");
    for (int i = 0; i < logSize; i++) {
        printf("%s\n", logArray[i]);
    }
}

// -------------------- Singly Linked List (Overloaded) --------------------
// Keeping track of drones that are currently overloaded
typedef struct OverloadedDrone {
    char id[50];
    struct OverloadedDrone* next;
} OverloadedDrone;

OverloadedDrone* overloadedHead = NULL;

void addOverloaded() {
    OverloadedDrone* newNode = (OverloadedDrone*)malloc(sizeof(OverloadedDrone));
    printf("Enter overloaded drone ID: ");
    getchar(); // flush newline
    fgets(newNode->id, sizeof(newNode->id), stdin);
    newNode->id[strcspn(newNode->id, "\n")] = 0;
    newNode->next = overloadedHead; // insert at beginning
    overloadedHead = newNode;
    printf("Overloaded drone added.\n");
}

// -------------------- Doubly Linked List (Serviced) --------------------
// List of serviced drones; can view forward and backward
typedef struct ServicedDrone {
    char id[50];
    struct ServicedDrone* next;
    struct ServicedDrone* prev;
} ServicedDrone;

ServicedDrone* servicedHead = NULL;
ServicedDrone* servicedTail = NULL;

void serviceDrone() {
    if (!overloadedHead) {
        printf("No overloaded drones to service.\n");
        return;
    }

    // Take one drone from overloaded list
    OverloadedDrone* temp = overloadedHead;
    overloadedHead = overloadedHead->next;

    // Add to serviced list at the tail
    ServicedDrone* newNode = (ServicedDrone*)malloc(sizeof(ServicedDrone));
    strcpy(newNode->id, temp->id);
    newNode->next = NULL;
    newNode->prev = servicedTail;

    if (servicedTail)
        servicedTail->next = newNode;
    else
        servicedHead = newNode;

    servicedTail = newNode;

    free(temp); // done with original node
    printf("Drone serviced and added to inspection list.\n");
}

void showServiced() {
    ServicedDrone* temp = servicedHead;
    printf("Serviced Drones (Forward):\n");
    while (temp) {
        printf("%s\n", temp->id);
        if (!temp->next) break;
        temp = temp->next;
    }
    printf("Serviced Drones (Backward):\n");
    while (temp) {
        printf("%s\n", temp->id);
        temp = temp->prev;
    }
}

// -------------------- Circular Linked List (Emergency) --------------------
// Circular list to repeatedly monitor emergency drones
typedef struct EmergencyDrone {
    char id[50];
    struct EmergencyDrone* next;
} EmergencyDrone;

EmergencyDrone* emergencyHead = NULL;

void addEmergency() {
    EmergencyDrone* newNode = (EmergencyDrone*)malloc(sizeof(EmergencyDrone));
    printf("Enter emergency drone ID: ");
    getchar(); // flush newline
    fgets(newNode->id, sizeof(newNode->id), stdin);
    newNode->id[strcspn(newNode->id, "\n")] = 0;

    if (!emergencyHead) {
        emergencyHead = newNode;
        newNode->next = emergencyHead;
    } else {
        EmergencyDrone* temp = emergencyHead;
        while (temp->next != emergencyHead)
            temp = temp->next;
        temp->next = newNode;
        newNode->next = emergencyHead;
    }

    printf("Emergency drone added.\n");
}

void monitorEmergency() {
    if (!emergencyHead) {
        printf("No emergency drones.\n");
        return;
    }

    // Print IDs in circular fashion (2 rounds)
    EmergencyDrone* temp = emergencyHead;
    printf("Emergency Drone Monitoring (2 rounds):\n");
    for (int i = 0; i < 4; i++) { // print 4 nodes to simulate 2 rounds
        printf("%s\n", temp->id);
        temp = temp->next;
    }
}

// -------------------- Main Menu --------------------
int main() {
    int choice;
    do {
        printf("\n--- Cargo Drone Traffic Controller ---\n");
        printf("1. Add Delivery Request\n");
        printf("2. Dispatch Urgent Deliveries\n");
        printf("3. Log Completed Flight\n");
        printf("4. Show Flight Logs\n");
        printf("5. Add Overloaded Drone\n");
        printf("6. Service a Drone\n");
        printf("7. Show Serviced Drones\n");
        printf("8. Add Emergency Drone\n");
        printf("9. Monitor Emergency Drones\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: enqueue(); break;
            case 2: dispatchUrgent(); break;
            case 3: logFlight(); break;
            case 4: showLogs(); break;
            case 5: addOverloaded(); break;
            case 6: serviceDrone(); break;
            case 7: showServiced(); break;
            case 8: addEmergency(); break;
            case 9: monitorEmergency(); break;
            case 0: printf("Exiting simulation.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);

    return 0;
}