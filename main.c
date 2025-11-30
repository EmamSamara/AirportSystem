#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Name : Emam Samara
//ID : 1220022
//section : 1

typedef enum {ARRIVAL, DEPARTURE, EMERGENCY} FlightState;
typedef enum {ACTIVE, LANDED, DEPARTED, CANCELLED} FlightStatus;

typedef struct Passenger {
    char name[64];
    char passport[32];
    struct Passenger* next;
} Passenger;

typedef struct Flight {
    char id[16];
    FlightState state;
    FlightStatus status;
    char date[16];
    char time[8];
    Passenger* header;
    Passenger* last;
} Flight;

typedef struct Queue {
    Flight** items;
    int front;
    int rear;
    int size;
    int capacity;
} Queue;

Queue createQueue(int capacity) {
    Queue q;
    q.items = (Flight**)malloc(sizeof(Flight*) * capacity);
    q.front = 0;
    q.rear = -1;
    q.size = 0;
    q.capacity = capacity;
    return q;
}

int isEmpty(Queue* q) {
    return q->size == 0;
}

int isFull(Queue* q) {
    return q->size == q->capacity;
}

void enqueue(Queue* q, Flight* f) {
    if (isFull(q)) {
        printf("Queue is full!\n");
        return;
    }
    q->rear = (q->rear + 1) % q->capacity;
    q->items[q->rear] = f;
    q->size++;
}

Flight* dequeue(Queue* q) {
    if (isEmpty(q)) return NULL;
    Flight* f = q->items[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return f;
}

// ============================================
// INPUT VALIDATION FUNCTIONS
// ============================================

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getValidInteger(int min, int max) {
    int value;
    int valid = 0;
    
    while (!valid) {
        if (scanf("%d", &value) != 1) {
            clearInputBuffer();
            printf("[ERROR] Please enter a number between %d and %d: ", min, max);
            continue;
        }
        
        if (value < min || value > max) {
            printf("[ERROR] Please enter a number between %d and %d: ", min, max);
            continue;
        }
        
        valid = 1;
    }
    
    clearInputBuffer();
    return value;
}

int isValidDate(const char* date) {
    if (strlen(date) != 10) return 0;
    if (date[2] != '-' || date[5] != '-') return 0;
    
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(date[i])) return 0;
    }
    
    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + 
               (date[8] - '0') * 10 + (date[9] - '0');
    
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (year < 2025) return 0;
    
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        daysInMonth[2] = 29;
    }
    
    if (day > daysInMonth[month]) return 0;
    
    return 1;
}

int isValidTime(const char* time) {
    if (strlen(time) != 5) return 0;
    if (time[2] != ':') return 0;
    
    if (!isdigit(time[0]) || !isdigit(time[1]) || 
        !isdigit(time[3]) || !isdigit(time[4])) return 0;
    
    int hours = (time[0] - '0') * 10 + (time[1] - '0');
    int minutes = (time[3] - '0') * 10 + (time[4] - '0');
    
    if (hours < 0 || hours > 23) return 0;
    if (minutes < 0 || minutes > 59) return 0;
    
    return 1;
}

int isValidFlightID(const char* id) {
    if (strlen(id) < 3 || strlen(id) > 10) return 0;
    
    for (int i = 0; i < strlen(id); i++) {
        if (!isalnum(id[i])) return 0;
    }
    
    return 1;
}

int isValidPassport(const char* passport) {
    if (strlen(passport) < 3 || strlen(passport) > 20) return 0;
    
    for (int i = 0; i < strlen(passport); i++) {
        if (!isalnum(passport[i])) return 0;
    }
    
    return 1;
}

int isValidName(const char* name) {
    if (strlen(name) < 2 || strlen(name) > 63) return 0;
    
    for (int i = 0; i < strlen(name); i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
    }
    
    return 1;
}

char* getValidDate() {
    static char date[16];
    int valid = 0;
    
    while (!valid) {
        printf("Enter Date (dd-mm-yyyy): ");
        fgets(date, sizeof(date), stdin);
        date[strcspn(date, "\n")] = '\0';
        
        if (strlen(date) == 0) {
            printf("[ERROR] Date cannot be empty!\n");
            continue;
        }
        
        if (!isValidDate(date)) {
            printf("[ERROR] Invalid date format! Use dd-mm-yyyy (e.g., 25-12-2025)\n");
            continue;
        }
        
        valid = 1;
    }
    
    return date;
}

char* getValidTime() {
    static char time[8];
    int valid = 0;
    
    while (!valid) {
        printf("Enter Time (hh:mm): ");
        fgets(time, sizeof(time), stdin);
        time[strcspn(time, "\n")] = '\0';
        
        if (strlen(time) == 0) {
            printf("[ERROR] Time cannot be empty!\n");
            continue;
        }
        
        if (!isValidTime(time)) {
            printf("[ERROR] Invalid time format! Use hh:mm (e.g., 14:30)\n");
            continue;
        }
        
        valid = 1;
    }
    
    return time;
}

char* getValidFlightID() {
    static char id[16];
    int valid = 0;
    
    while (!valid) {
        printf("Enter Flight ID: ");
        fgets(id, sizeof(id), stdin);
        id[strcspn(id, "\n")] = '\0';
        
        if (strlen(id) == 0) {
            printf("[ERROR] Flight ID cannot be empty!\n");
            continue;
        }
        
        if (!isValidFlightID(id)) {
            printf("[ERROR] Invalid Flight ID! Use alphanumeric characters (3-10 chars)\n");
            continue;
        }
        
        valid = 1;
    }
    
    return id;
}

char* getValidPassport() {
    static char passport[32];
    int valid = 0;
    
    while (!valid) {
        printf("Enter Passport Number: ");
        fgets(passport, sizeof(passport), stdin);
        passport[strcspn(passport, "\n")] = '\0';
        
        if (strlen(passport) == 0) {
            printf("[ERROR] Passport cannot be empty!\n");
            continue;
        }
        
        if (!isValidPassport(passport)) {
            printf("[ERROR] Invalid Passport! Use alphanumeric characters (3-20 chars)\n");
            continue;
        }
        
        valid = 1;
    }
    
    return passport;
}

char* getValidName() {
    static char name[64];
    int valid = 0;
    
    while (!valid) {
        printf("Enter Name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';
        
        if (strlen(name) == 0) {
            printf("[ERROR] Name cannot be empty!\n");
            continue;
        }
        
        if (!isValidName(name)) {
            printf("[ERROR] Invalid Name! Use only letters and spaces\n");
            continue;
        }
        
        valid = 1;
    }
    
    return name;
}

// ============================================
// ORIGINAL FUNCTIONS
// ============================================

Passenger* createPassenger(char* name, char* passport) {
    Passenger* p = (Passenger*)malloc(sizeof(Passenger));
    strncpy(p->name, name, sizeof(p->name) - 1);
    p->name[sizeof(p->name) - 1] = '\0';
    strncpy(p->passport, passport, sizeof(p->passport) - 1);
    p->passport[sizeof(p->passport) - 1] = '\0';
    p->next = NULL;
    return p;
}

void addPassenger(Flight* f, char* name, char* passport) {
    Passenger* p = createPassenger(name, passport);
    if (f->header == NULL) {
        f->header = p;
        f->last = p;
    } else {
        f->last->next = p;
        f->last = p;
    }
}

Flight* createFlight(char* id, FlightState state, char* date, char* time) {
    Flight* f = (Flight*)malloc(sizeof(Flight));
    strncpy(f->id, id, sizeof(f->id) - 1);
    f->id[sizeof(f->id) - 1] = '\0';
    f->state = state;
    f->status = ACTIVE;
    strncpy(f->date, date, sizeof(f->date) - 1);
    f->date[sizeof(f->date) - 1] = '\0';
    strncpy(f->time, time, sizeof(f->time) - 1);
    f->time[sizeof(f->time) - 1] = '\0';
    f->header = NULL;
    f->last = NULL;
    return f;
}

void printPassengers(Flight* f) {
    Passenger* current = f->header;
    if (current == NULL) {
        printf("  No passengers on this flight.\n");
        return;
    }
    while (current != NULL) {
        printf("  Passenger: %s, Passport: %s\n", current->name, current->passport);
        current = current->next;
    }
}

void printFlightDetail(Flight* f) {
    const char* stateStr[] = {"ARRIVAL", "DEPARTURE", "EMERGENCY"};
    const char* statusStr[] = {"ACTIVE", "LANDED", "DEPARTED", "CANCELLED"};
    
    printf("Flight %s | Date: %s | Time: %s | State: %s | Status: %s\n",
           f->id, f->date, f->time, stateStr[f->state], statusStr[f->status]);
    printPassengers(f);
}

void loadFlights(const char* filename, Queue* arrivals, Queue* departures, Queue* emergencies) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("[WARNING] File '%s' not found. Continuing without preloaded flights.\n", filename);
        return;
    }

    char id[16], stateStr[16], date[16], time[8];
    while (fscanf(file, "%15[^;];%15[^;];%15[^;];%7s\n", id, stateStr, date, time) == 4) {
        FlightState state = ARRIVAL;
        if (strcmp(stateStr, "Departure") == 0) state = DEPARTURE;
        else if (strcmp(stateStr, "Emergency") == 0) state = EMERGENCY;

        Flight* f = createFlight(id, state, date, time);

        if (state == ARRIVAL) enqueue(arrivals, f);
        else if (state == DEPARTURE) enqueue(departures, f);
        else enqueue(emergencies, f);
    }

    fclose(file);
    printf("[OK] Flights loaded successfully from %s\n", filename);
}

void loadPassengers(const char* filename, Queue* arrivals, Queue* departures, Queue* emergencies) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("[WARNING] File '%s' not found. Continuing without preloaded passengers.\n", filename);
        return;
    }

    char name[64], passport[32], flightID[16];
    while (fscanf(file, "%63[^;];%31[^;];%15s\n", name, passport, flightID) == 3) {
        Flight* target = NULL;

        for (int i = 0; i < arrivals->size; i++)
            if (strcmp(arrivals->items[(arrivals->front + i) % arrivals->capacity]->id, flightID) == 0)
                target = arrivals->items[(arrivals->front + i) % arrivals->capacity];
        for (int i = 0; i < departures->size; i++)
            if (strcmp(departures->items[(departures->front + i) % departures->capacity]->id, flightID) == 0)
                target = departures->items[(departures->front + i) % departures->capacity];
        for (int i = 0; i < emergencies->size; i++)
            if (strcmp(emergencies->items[(emergencies->front + i) % emergencies->capacity]->id, flightID) == 0)
                target = emergencies->items[(emergencies->front + i) % emergencies->capacity];

        if (target) {
            addPassenger(target, name, passport);
        }
    }

    fclose(file);
    printf("[OK] Passengers loaded successfully from %s\n", filename);
}

int countPassengers(Flight* f) {
    int count = 0;
    Passenger* current = f->header;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void removePassenger(Flight* f, const char* passport) {
    Passenger* current = f->header;
    Passenger* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->passport, passport) == 0) {
            if (prev == NULL) f->header = current->next;
            else prev->next = current->next;
            if (current == f->last) f->last = prev;
            free(current);
            printf("[OK] Passenger with passport %s removed.\n", passport);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("[ERROR] Passenger with passport %s not found.\n", passport);
}

void showMenu() {
    printf("\n");
    printf("==========================================\n");
    printf("    AIRPORT MANAGEMENT SYSTEM\n");
    printf("==========================================\n");
    printf("1.  Add a flight\n");
    printf("2.  Add passenger to first flight\n");
    printf("3.  Land a flight (Emergency priority)\n");
    printf("4.  Depart a flight\n");
    printf("5.  Cancel a flight\n");
    printf("6.  Show all flights\n");
    printf("7.  Show flight detail by ID\n");
    printf("8.  Show passengers of a flight\n");
    printf("9.  Add passenger to a flight by ID\n");
    printf("10. Remove passenger from a flight\n");
    printf("11. Show flight status\n");
    printf("12. Exit\n");
    printf("Choose an option (1-12): ");
}

// ============================================
// MEMORY CLEANUP FUNCTIONS
// ============================================

// Frees all passengers linked list in a flight
void cleanupPassengers(Flight* f) {
    if (f == NULL) return;
    Passenger* current = f->header;
    while (current != NULL) {
        Passenger* temp = current;
        current = current->next;
        free(temp);
    }
}

// Frees all flights in a queue
void cleanupQueue(Queue* q) {
    if (q == NULL || q->items == NULL) return;
    
    while (!isEmpty(q)) {
        Flight* f = dequeue(q);
        if (f != NULL) {
            cleanupPassengers(f);
            free(f);
        }
    }
    free(q->items);
}

// Main cleanup function - call before exit
void cleanupAllResources(Queue* arrivals, Queue* departures, Queue* emergencies,
                         Queue* landed, Queue* departed, Queue* cancelled) {
    printf("\n[INFO] Cleaning up resources...\n");
    cleanupQueue(arrivals);
    cleanupQueue(departures);
    cleanupQueue(emergencies);
    cleanupQueue(landed);
    cleanupQueue(departed);
    cleanupQueue(cancelled);
    printf("[OK] All resources freed successfully.\n");
}

int main() {
    Queue arrivals = createQueue(100);
    Queue departures = createQueue(100);
    Queue emergencies = createQueue(100);
    Queue landed = createQueue(100);
    Queue departed = createQueue(100);
    Queue cancelled = createQueue(100);

    printf("==========================================\n");
    printf("  Loading Airport System Data...\n");
    printf("==========================================\n");

    loadFlights("flights.txt", &arrivals, &departures, &emergencies);
    loadPassengers("passengers.txt", &arrivals, &departures, &emergencies);

    int choice;
    do {
        showMenu();
        choice = getValidInteger(1, 12);

        switch(choice) {
            case 1: {
                printf("\n--- Add New Flight ---\n");
                char* id = getValidFlightID();
                
                printf("Flight State:\n");
                printf("  0 = ARRIVAL\n");
                printf("  1 = DEPARTURE\n");
                printf("  2 = EMERGENCY\n");
                printf("Select (0-2): ");
                int state = getValidInteger(0, 2);
                
                char* date = getValidDate();
                char* time = getValidTime();

                Flight* f = createFlight(id, (FlightState)state, date, time);

                if (state == ARRIVAL) enqueue(&arrivals, f);
                else if (state == DEPARTURE) enqueue(&departures, f);
                else enqueue(&emergencies, f);

                printf("[OK] Flight %s added successfully!\n", id);
                break;
            }

            case 2: {
                if (isEmpty(&arrivals)) {
                    printf("[ERROR] No arrival flights available.\n");
                    break;
                }
                
                printf("\n--- Add Passenger to First Arrival ---\n");
                char* name = getValidName();
                char* passport = getValidPassport();

                addPassenger(arrivals.items[arrivals.front], name, passport);
                printf("[OK] Passenger %s added to flight %s.\n",
                       name, arrivals.items[arrivals.front]->id);
                break;
            }

            case 3: {
                printf("\n--- Land Flight ---\n");
                Flight* f = NULL;
                
                // Emergency flights have priority
                if (!isEmpty(&emergencies)) {
                    f = dequeue(&emergencies);
                    printf("[OK] Emergency flight prioritized!\n");
                } else if (!isEmpty(&arrivals)) {
                    f = dequeue(&arrivals);
                    printf("[OK] Regular arrival flight landing.\n");
                } else {
                    printf("[ERROR] No flights available to land.\n");
                    break;
                }
                
                if (f != NULL) {
                    f->status = LANDED;
                    enqueue(&landed, f);
                    printf("[OK] Flight %s has LANDED.\n", f->id);
                }
                break;
            }

            case 4: {
                printf("\n--- Depart Flight ---\n");
                if (isEmpty(&departures)) {
                    printf("[ERROR] No departure flights available.\n");
                    break;
                }
                
                Flight* f = dequeue(&departures);
                f->status = DEPARTED;
                enqueue(&departed, f);
                printf("[OK] Flight %s has DEPARTED.\n", f->id);
                break;
            }

            case 5: {
                printf("\n--- Cancel Flight ---\n");
                if (isEmpty(&arrivals)) {
                    printf("[ERROR] No arrival flights to cancel.\n");
                    break;
                }
                
                Flight* f = dequeue(&arrivals);
                f->status = CANCELLED;
                enqueue(&cancelled, f);
                printf("[OK] Flight %s has been CANCELLED.\n", f->id);
                break;
            }

            case 6: {
                printf("\n==========================================\n");
                printf("     ALL FLIGHTS STATUS\n");
                printf("==========================================\n");
                
                printf("\n=== ACTIVE ARRIVALS (%d) ===\n", arrivals.size);
                if (isEmpty(&arrivals)) {
                    printf("  No active arrival flights.\n");
                } else {
                    for (int i = 0; i < arrivals.size; i++) {
                        printFlightDetail(arrivals.items[(arrivals.front + i) % arrivals.capacity]);
                    }
                }

                printf("\n=== ACTIVE DEPARTURES (%d) ===\n", departures.size);
                if (isEmpty(&departures)) {
                    printf("  No active departure flights.\n");
                } else {
                    for (int i = 0; i < departures.size; i++) {
                        printFlightDetail(departures.items[(departures.front + i) % departures.capacity]);
                    }
                }

                printf("\n=== ACTIVE EMERGENCIES (%d) ===\n", emergencies.size);
                if (isEmpty(&emergencies)) {
                    printf("  No active emergency flights.\n");
                } else {
                    for (int i = 0; i < emergencies.size; i++) {
                        printFlightDetail(emergencies.items[(emergencies.front + i) % emergencies.capacity]);
                    }
                }

                printf("\n=== LANDED FLIGHTS (%d) ===\n", landed.size);
                if (isEmpty(&landed)) {
                    printf("  No landed flights.\n");
                } else {
                    for (int i = 0; i < landed.size; i++) {
                        printFlightDetail(landed.items[(landed.front + i) % landed.capacity]);
                    }
                }

                printf("\n=== DEPARTED FLIGHTS (%d) ===\n", departed.size);
                if (isEmpty(&departed)) {
                    printf("  No departed flights.\n");
                } else {
                    for (int i = 0; i < departed.size; i++) {
                        printFlightDetail(departed.items[(departed.front + i) % departed.capacity]);
                    }
                }

                printf("\n=== CANCELLED FLIGHTS (%d) ===\n", cancelled.size);
                if (isEmpty(&cancelled)) {
                    printf("  No cancelled flights.\n");
                } else {
                    for (int i = 0; i < cancelled.size; i++) {
                        printFlightDetail(cancelled.items[(cancelled.front + i) % cancelled.capacity]);
                    }
                }
                break;
            }

            case 7: {
                printf("\n--- Search Flight by ID ---\n");
                char* searchID = getValidFlightID();

                Flight* target = NULL;

                for (int i = 0; i < arrivals.size; i++)
                    if (strcmp(arrivals.items[(arrivals.front + i) % arrivals.capacity]->id, searchID) == 0)
                        target = arrivals.items[(arrivals.front + i) % arrivals.capacity];
                for (int i = 0; i < departures.size; i++)
                    if (strcmp(departures.items[(departures.front + i) % departures.capacity]->id, searchID) == 0)
                        target = departures.items[(departures.front + i) % departures.capacity];
                for (int i = 0; i < emergencies.size; i++)
                    if (strcmp(emergencies.items[(emergencies.front + i) % emergencies.capacity]->id, searchID) == 0)
                        target = emergencies.items[(emergencies.front + i) % emergencies.capacity];
                for (int i = 0; i < landed.size; i++)
                    if (strcmp(landed.items[(landed.front + i) % landed.capacity]->id, searchID) == 0)
                        target = landed.items[(landed.front + i) % landed.capacity];
                for (int i = 0; i < departed.size; i++)
                    if (strcmp(departed.items[(departed.front + i) % departed.capacity]->id, searchID) == 0)
                        target = departed.items[(departed.front + i) % departed.capacity];
                for (int i = 0; i < cancelled.size; i++)
                    if (strcmp(cancelled.items[(cancelled.front + i) % cancelled.capacity]->id, searchID) == 0)
                        target = cancelled.items[(cancelled.front + i) % cancelled.capacity];

                if (target) {
                    printFlightDetail(target);
                } else {
                    printf("[ERROR] Flight %s not found.\n", searchID);
                }
                break;
            }

            case 8: {
                printf("\n--- Show Flight Passengers ---\n");
                char* searchID = getValidFlightID();

                Flight* target = NULL;
                for (int i = 0; i < arrivals.size; i++)
                    if (strcmp(arrivals.items[(arrivals.front + i) % arrivals.capacity]->id, searchID) == 0)
                        target = arrivals.items[(arrivals.front + i) % arrivals.capacity];
                for (int i = 0; i < departures.size; i++)
                    if (strcmp(departures.items[(departures.front + i) % departures.capacity]->id, searchID) == 0)
                        target = departures.items[(departures.front + i) % departures.capacity];
                for (int i = 0; i < emergencies.size; i++)
                    if (strcmp(emergencies.items[(emergencies.front + i) % emergencies.capacity]->id, searchID) == 0)
                        target = emergencies.items[(emergencies.front + i) % emergencies.capacity];
                for (int i = 0; i < landed.size; i++)
                    if (strcmp(landed.items[(landed.front + i) % landed.capacity]->id, searchID) == 0)
                        target = landed.items[(landed.front + i) % landed.capacity];
                for (int i = 0; i < departed.size; i++)
                    if (strcmp(departed.items[(departed.front + i) % departed.capacity]->id, searchID) == 0)
                        target = departed.items[(departed.front + i) % departed.capacity];
                for (int i = 0; i < cancelled.size; i++)
                    if (strcmp(cancelled.items[(cancelled.front + i) % cancelled.capacity]->id, searchID) == 0)
                        target = cancelled.items[(cancelled.front + i) % cancelled.capacity];

                if (target) {
                    printf("Passengers on Flight %s (%d total):\n", searchID, countPassengers(target));
                    printPassengers(target);
                } else {
                    printf("[ERROR] Flight %s not found.\n", searchID);
                }
                break;
            }

            case 9: {
                printf("\n--- Add Passenger to Flight ---\n");
                char* searchID = getValidFlightID();

                Flight* target = NULL;
                for (int i = 0; i < arrivals.size; i++)
                    if (strcmp(arrivals.items[(arrivals.front + i) % arrivals.capacity]->id, searchID) == 0)
                        target = arrivals.items[(arrivals.front + i) % arrivals.capacity];
                for (int i = 0; i < departures.size; i++)
                    if (strcmp(departures.items[(departures.front + i) % departures.capacity]->id, searchID) == 0)
                        target = departures.items[(departures.front + i) % departures.capacity];
                for (int i = 0; i < emergencies.size; i++)
                    if (strcmp(emergencies.items[(emergencies.front + i) % emergencies.capacity]->id, searchID) == 0)
                        target = emergencies.items[(emergencies.front + i) % emergencies.capacity];

                if (target) {
                    char* name = getValidName();
                    char* passport = getValidPassport();
                    addPassenger(target, name, passport);
                    printf("[OK] Passenger %s added to flight %s.\n", name, searchID);
                } else {
                    printf("[ERROR] Flight %s not found or has already departed/landed.\n", searchID);
                }
                break;
            }

            case 10: {
                printf("\n--- Remove Passenger from Flight ---\n");
                char* searchID = getValidFlightID();

                Flight* target = NULL;
                for (int i = 0; i < arrivals.size; i++)
                    if (strcmp(arrivals.items[(arrivals.front + i) % arrivals.capacity]->id, searchID) == 0)
                        target = arrivals.items[(arrivals.front + i) % arrivals.capacity];
                for (int i = 0; i < departures.size; i++)
                    if (strcmp(departures.items[(departures.front + i) % departures.capacity]->id, searchID) == 0)
                        target = departures.items[(departures.front + i) % departures.capacity];
                for (int i = 0; i < emergencies.size; i++)
                    if (strcmp(emergencies.items[(emergencies.front + i) % emergencies.capacity]->id, searchID) == 0)
                        target = emergencies.items[(emergencies.front + i) % emergencies.capacity];
                for (int i = 0; i < landed.size; i++)
                    if (strcmp(landed.items[(landed.front + i) % landed.capacity]->id, searchID) == 0)
                        target = landed.items[(landed.front + i) % landed.capacity];
                for (int i = 0; i < departed.size; i++)
                    if (strcmp(departed.items[(departed.front + i) % departed.capacity]->id, searchID) == 0)
                        target = departed.items[(departed.front + i) % departed.capacity];
                for (int i = 0; i < cancelled.size; i++)
                    if (strcmp(cancelled.items[(cancelled.front + i) % cancelled.capacity]->id, searchID) == 0)
                        target = cancelled.items[(cancelled.front + i) % cancelled.capacity];

                if (target) {
                    char* passport = getValidPassport();
                    removePassenger(target, passport);
                } else {
                    printf("[ERROR] Flight %s not found.\n", searchID);
                }
                break;
            }

            case 11: {
                printf("\n==========================================\n");
                printf("  ALL FLIGHT STATUSES\n");
                printf("==========================================\n");
                
                const char* statusStr[] = {"ACTIVE", "LANDED", "DEPARTED", "CANCELLED"};
                const char* stateStr[] = {"ARRIVAL", "DEPARTURE", "EMERGENCY"};
                
                printf("\n=== ACTIVE ARRIVALS ===\n");
                if (isEmpty(&arrivals)) {
                    printf("  No flights.\n");
                } else {
                    for (int i = 0; i < arrivals.size; i++) {
                        Flight* f = arrivals.items[(arrivals.front + i) % arrivals.capacity];
                        printf("  Flight %s [%s]: %s (Date: %s, Time: %s)\n", 
                               f->id, stateStr[f->state], statusStr[f->status], f->date, f->time);
                    }
                }
                
                printf("\n=== ACTIVE DEPARTURES ===\n");
                if (isEmpty(&departures)) {
                    printf("  No flights.\n");
                } else {
                    for (int i = 0; i < departures.size; i++) {
                        Flight* f = departures.items[(departures.front + i) % departures.capacity];
                        printf("  Flight %s [%s]: %s (Date: %s, Time: %s)\n", 
                               f->id, stateStr[f->state], statusStr[f->status], f->date, f->time);
                    }
                }
                
                printf("\n=== ACTIVE EMERGENCIES ===\n");
                if (isEmpty(&emergencies)) {
                    printf("  No flights.\n");
                } else {
                    for (int i = 0; i < emergencies.size; i++) {
                        Flight* f = emergencies.items[(emergencies.front + i) % emergencies.capacity];
                        printf("  Flight %s [%s]: %s (Date: %s, Time: %s)\n", 
                               f->id, stateStr[f->state], statusStr[f->status], f->date, f->time);
                    }
                }

                printf("\n=== LANDED FLIGHTS ===\n");
                if (isEmpty(&landed)) {
                    printf("  No flights.\n");
                } else {
                    for (int i = 0; i < landed.size; i++) {
                        Flight* f = landed.items[(landed.front + i) % landed.capacity];
                        printf("  Flight %s [%s]: %s (Date: %s, Time: %s)\n", 
                               f->id, stateStr[f->state], statusStr[f->status], f->date, f->time);
                    }
                }

                printf("\n=== DEPARTED FLIGHTS ===\n");
                if (isEmpty(&departed)) {
                    printf("  No flights.\n");
                } else {
                    for (int i = 0; i < departed.size; i++) {
                        Flight* f = departed.items[(departed.front + i) % departed.capacity];
                        printf("  Flight %s [%s]: %s (Date: %s, Time: %s)\n", 
                               f->id, stateStr[f->state], statusStr[f->status], f->date, f->time);
                    }
                }

                printf("\n=== CANCELLED FLIGHTS ===\n");
                if (isEmpty(&cancelled)) {
                    printf("  No flights.\n");
                } else {
                    for (int i = 0; i < cancelled.size; i++) {
                        Flight* f = cancelled.items[(cancelled.front + i) % cancelled.capacity];
                        printf("  Flight %s [%s]: %s (Date: %s, Time: %s)\n", 
                               f->id, stateStr[f->state], statusStr[f->status], f->date, f->time);
                    }
                }
                break;
            }

            case 12:
                printf("\n==========================================\n");
                printf("  Exiting Program...\n");
                printf("  Thank you for using the Airport System!\n");
                printf("==========================================\n\n");
                
                // CALL CLEANUP FUNCTION BEFORE EXIT - EMPTY ALL MEMORY
                cleanupAllResources(&arrivals, &departures, &emergencies,
                                   &landed, &departed, &cancelled);
                break;

            default:
                printf("[ERROR] Invalid choice. Please try again.\n");
        }
    } while (choice != 12);

    return 0;
}
