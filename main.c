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
// validation functons
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
    if (passport == NULL) return 0;
    size_t len = strlen(passport);
    if (len < 3 || len > 20) return 0;
    
    // ONLY accept digits
    for (size_t i = 0; i < len; ++i) {
        if (!isdigit((unsigned char)passport[i])) return 0;
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

// convert to lower case
void toLowerInPlace(char *s) {
    for (size_t i = 0; s && s[i]; ++i) s[i] = (char)tolower((unsigned char)s[i]);
}

char* getValidDate() {
    static char date[16];
    int valid = 0;
    
    while (!valid) {
        printf("Enter Date (dd-mm-yyyy) (future date - year >= 2025): ");
        fgets(date, sizeof(date), stdin);
        date[strcspn(date, "\n")] = '\0';
        trim(date);
        
        if (strlen(date) == 0) {
            printf("[ERROR] Date cannot be empty!\n");
            continue;
        }
        
        if (!isValidDate(date)) {
            printf("[ERROR] Invalid date. Please enter a future date in format dd-mm-yyyy (year >= 2025).\n");
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
        trim(time);
        
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
        trim(id);
        
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
        printf("Enter Passport Number (digits only, 3-20 chars): ");
        if (!fgets(passport, sizeof(passport), stdin)) {
            clearInputBuffer();
            printf("[ERROR] Input error. Try again.\n");
            continue;
        }
        passport[strcspn(passport, "\n")] = '\0';
        trim(passport);
        
        if (strlen(passport) == 0) {
            printf("[ERROR] Passport cannot be empty!\n");
            continue;
        }
        
        if (!isValidPassport(passport)) {
            printf("[ERROR] Invalid Passport! Use digits only (3-20 chars). Please try again.\n");
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
        trim(name);
        
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
// orignal functons
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

    char id[16], stateStr[32], date[16], time[8];
    while (fscanf(file, "%15[^;];%31[^;];%15[^;];%7s\n", id, stateStr, date, time) == 4) {
        trim(id);
        trim(stateStr);
        trim(date);
        trim(time);
        // normalize state string to lowercase for comparison
        toLowerInPlace(stateStr);

        FlightState state = ARRIVAL;
        if (strcmp(stateStr, "departure") == 0) state = DEPARTURE;
        else if (strcmp(stateStr, "emergency") == 0) state = EMERGENCY;

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
        trim(name);
        trim(passport);
        trim(flightID);

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

int removePassenger(Flight* f, const char* passport) {
    Passenger* current = f->header;
    Passenger* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->passport, passport) == 0) {
            if (prev == NULL) f->header = current->next;
            else prev->next = current->next;
            if (current == f->last) f->last = prev;
            free(current);
            printf("[OK] Passenger with passport %s removed.\n", passport);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    printf("[ERROR] Passenger with passport %s not found.\n", passport);
    return 0;
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
// memroy cleanup
// ============================================

// free pasengers list
void cleanupPassengers(Flight* f) {
    if (f == NULL) return;
    Passenger* current = f->header;
    while (current != NULL) {
        Passenger* temp = current;
        current = current->next;
        free(temp);
    }
}

// free all flights
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

// cleanup funtion call befre exit
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

// remove flite by id from queue
Flight* removeFlightByID(Queue* q, const char* id) {
    if (isEmpty(q)) return NULL;
    Flight* found = NULL;
    int n = q->size;
    for (int i = 0; i < n; i++) {
        Flight* f = dequeue(q);
        if (found == NULL && strcmp(f->id, id) == 0) {
            found = f;
        } else {
            enqueue(q, f);
        }
    }
    return found;
}

// find flite in queue
Flight* findFlightInQueue(Queue* q, const char* id) {
    if (isEmpty(q)) return NULL;
    for (int i = 0; i < q->size; i++) {
        Flight* f = q->items[(q->front + i) % q->capacity];
        if (strcmp(f->id, id) == 0) return f;
    }
    return NULL;
}

// find flite across queues
Flight* findFlightAcross(const char* id,
                         Queue* arrivals, Queue* departures, Queue* emergencies,
                         Queue* landed, Queue* departed, Queue* cancelled,
                         Queue** outQueue) {
    Flight* f = NULL;
    if (arrivals && (f = findFlightInQueue(arrivals, id))) {
        if (outQueue) *outQueue = arrivals;
        return f;
    }
    if (departures && (f = findFlightInQueue(departures, id))) {
        if (outQueue) *outQueue = departures;
        return f;
    }
    if (emergencies && (f = findFlightInQueue(emergencies, id))) {
        if (outQueue) *outQueue = emergencies;
        return f;
    }
    if (landed && (f = findFlightInQueue(landed, id))) {
        if (outQueue) *outQueue = landed;
        return f;
    }
    if (departed && (f = findFlightInQueue(departed, id))) {
        if (outQueue) *outQueue = departed;
        return f;
    }
    if (cancelled && (f = findFlightInQueue(cancelled, id))) {
        if (outQueue) *outQueue = cancelled;
        return f;
    }
    if (outQueue) *outQueue = NULL;
    return NULL;
}

// check pasport exists
int passengerPassportExists(Flight* f, const char* passport) {
    if (f == NULL || passport == NULL) return 0;
    Passenger* cur = f->header;
    while (cur != NULL) {
        if (strcmp(cur->passport, passport) == 0) return 1;
        cur = cur->next;
    }
    return 0;
}

// get unique pasport for flite
char* getUniquePassportForFlight(Flight* f) {
    char* p;
    while (1) {
        p = getValidPassport();
        if (!passengerPassportExists(f, p)) return p;
        printf("[ERROR] Passport %s is already used on this flight. Please enter a different passport.\n", p);
    }
}

// check if flite id exists
int flightIDExists(const char* id, Queue* arrivals, Queue* departures, Queue* emergencies,
                   Queue* landed, Queue* departed, Queue* cancelled) {
    if (id == NULL) return 0;
    if (arrivals && findFlightInQueue(arrivals, id)) return 1;
    if (departures && findFlightInQueue(departures, id)) return 1;
    if (emergencies && findFlightInQueue(emergencies, id)) return 1;
    if (landed && findFlightInQueue(landed, id)) return 1;
    if (departed && findFlightInQueue(departed, id)) return 1;
    if (cancelled && findFlightInQueue(cancelled, id)) return 1;
    return 0;
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
                char* id = NULL;

                // keep asking until valid id
                while (1) {
                    id = getValidFlightID();
                    if (flightIDExists(id, &arrivals, &departures, &emergencies, &landed, &departed, &cancelled)) {
                        printf("[ERROR] Flight ID '%s' already exists. Please enter a unique ID.\n", id);
                        continue;
                    }
                    break;
                }
                
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
                printf("\n--- Add Passenger to Flight (by ID) ---\n");
                Flight* target = NULL;
                char* searchID = NULL;

                do {
                    searchID = getValidFlightID();
                    // search active queues
                    target = findFlightAcross(searchID, &arrivals, &departures, &emergencies,
                                              NULL, NULL, NULL, NULL);
                    if (!target) {
                        printf("[ERROR] Flight %s not found or not available for adding passengers. Please try again.\n", searchID);
                    }
                } while (!target);

                {
                    char* name = getValidName();
                    char* passport = getUniquePassportForFlight(target);
                    addPassenger(target, name, passport);
                    printf("[OK] Passenger %s added to flight %s.\n", name, searchID);
                }
                break;
            }

            case 3: {
                printf("\n--- Land Flight ---\n");
                Flight* f = NULL;
                
                // emergencies first
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

            case 4: { // Depart Flight: handle emergencies first, then prompt for departure ID
                printf("\n--- Depart Flight ---\n");

                // land emergencies first
                if (!isEmpty(&emergencies)) {
                    printf("[INFO] Emergency flights detected — landing them first.\n");
                    while (!isEmpty(&emergencies)) {
                        Flight* ef = dequeue(&emergencies);
                        ef->status = LANDED;
                        enqueue(&landed, ef);
                        printf("[OK] Emergency flight %s has LANDED.\n", ef->id);
                    }
                }

                // ask which flight depart
                if (isEmpty(&departures)) {
                    printf("[ERROR] No departure flights available.\n");
                    break;
                }
                
                Flight* removed = NULL;
                char* searchID = NULL;
                do {
                    printf("Enter ID of departure flight to depart: ");
                    searchID = getValidFlightID();
                    // remove from departures
                    removed = removeFlightByID(&departures, searchID);
                    if (!removed) {
                        printf("[ERROR] Departure flight %s not found. Please try again.\n", searchID);
                    }
                } while (!removed);

                removed->status = DEPARTED;
                enqueue(&departed, removed);
                printf("[OK] Flight %s has DEPARTED.\n", removed->id);
                break;
            }

            case 5: { // ask which flight cancel
                printf("\n--- Cancel Flight ---\n");

                if (isEmpty(&arrivals)) {
                    printf("[ERROR] No arrival flights to cancel.\n");
                    break;
                }

                Flight* removed = NULL;
                char* searchID = NULL;
                do {
                    printf("Enter ID of arrival flight to cancel: ");
                    searchID = getValidFlightID();
                    // remove from arrivals
                    removed = removeFlightByID(&arrivals, searchID);
                    if (!removed) {
                        printf("[ERROR] Arrival flight %s not found. Please try again.\n", searchID);
                    }
                } while (!removed);

                removed->status = CANCELLED;
                enqueue(&cancelled, removed);
                printf("[OK] Flight %s has been CANCELLED.\n", removed->id);
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
                Flight* target = NULL;
                char* searchID = NULL;

                do {
                    searchID = getValidFlightID();
                    target = findFlightAcross(searchID, &arrivals, &departures, &emergencies,
                                              &landed, &departed, &cancelled, NULL);
                    if (!target) {
                        printf("[ERROR] Flight %s not found. Please try again.\n", searchID);
                    }
                } while (!target);

                printFlightDetail(target);
                break;
            }

            case 8: {
                printf("\n--- Show Flight Passengers ---\n");
                Flight* target = NULL;
                char* searchID = NULL;

                do {
                    searchID = getValidFlightID();
                    target = findFlightAcross(searchID, &arrivals, &departures, &emergencies,
                                              &landed, &departed, &cancelled, NULL);
                    if (!target) {
                        printf("[ERROR] Flight %s not found. Please try again.\n", searchID);
                    }
                } while (!target);

                printf("Passengers on Flight %s (%d total):\n", searchID, countPassengers(target));
                printPassengers(target);
                break;
            }

            case 9: {
                printf("\n--- Add Passenger to Flight (by ID) ---\n");
                Flight* target = NULL;
                char* searchID = NULL;

                do {
                    searchID = getValidFlightID();
                    target = findFlightAcross(searchID, &arrivals, &departures, &emergencies,
                                              NULL, NULL, NULL, NULL);
                    if (!target) {
                        printf("[ERROR] Flight %s not found or not available for adding passengers. Please try again.\n", searchID);
                    }
                } while (!target);

                {
                    char* name = getValidName();
                    char* passport = getUniquePassportForFlight(target);
                    addPassenger(target, name, passport);
                    printf("[OK] Passenger %s added to flight %s.\n", name, searchID);
                }
                break;
            }

            case 10: {
                printf("\n--- Remove Passenger from Flight ---\n");
                Flight* target = NULL;
                char* searchID = NULL;

                do {
                    searchID = getValidFlightID();
                    target = findFlightAcross(searchID, &arrivals, &departures, &emergencies,
                                              &landed, &departed, &cancelled, NULL);
                    if (!target) {
                        printf("[ERROR] Flight %s not found. Please try again.\n", searchID);
                    }
                } while (!target);

                {
                    int removed = 0;
                    while (!removed) {
                        char* passport = getValidPassport();
                        removed = removePassenger(target, passport); // returns 1 on success
                        if (!removed) {
                            printf("[ERROR] Passport not found on this flight. Try again.\n");
                        }
                    }
                }
                break;
            }

            case 11: {
                    printf("\n--- Show Flight Status (by ID) ---\n");
                    Flight* target = NULL;
                    char* searchID = NULL;

                    do {
                        searchID = getValidFlightID();
                        target = findFlightAcross(searchID,
                                                  &arrivals, &departures, &emergencies,
                                                  &landed, &departed, &cancelled,
                                                  NULL);
                        if (!target) {
                            printf("[ERROR] Flight %s not found. Please try again.\n", searchID);
                        }
                    } while (!target);

                    {
                        const char* stateStr[] = {"ARRIVAL", "DEPARTURE", "EMERGENCY"};
                        const char* statusStr[] = {"ACTIVE", "LANDED", "DEPARTED", "CANCELLED"};
                        int pax = countPassengers(target);
                        printf("Flight %s | Date: %s | Time: %s | State: %s | Status: %s | Passengers: %d\n",
                               target->id,
                               target->date,
                               target->time,
                               stateStr[target->state],
                               statusStr[target->status],
                               pax);
                    }
                    break;
                }

            case 12:
                printf("\n==========================================\n");
                printf("  Exiting Program...\n");
                printf("  Thank you for using the Airport System!\n");
                printf("==========================================\n\n");
                
                // cleanup before exit
                cleanupAllResources(&arrivals, &departures, &emergencies,
                                   &landed, &departed, &cancelled);
                break;

            default:
                printf("[ERROR] Invalid choice. Please try again.\n");
        }
    } while (choice != 12);

    return 0;
}
