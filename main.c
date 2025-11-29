#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Name : Emam Samara
//ID : 1220022
//section : 1



 typedef enum {ARRIVAL ,DEPARTURE, EMERGENCY} FlightState;
typedef enum {ACTIVE, LANDED, DEPARTED, CANCELLED} FlightStatus;


    typedef struct Passenger{
    char name[64];
    char passport[32];
    struct Passenger*next;
    }Passenger;

    typedef struct Flight{
    char id[16];
    FlightState state;
    FlightStatus status;
    char date[16];
    char time[8];
    Passenger*header;
    Passenger*last;

    }Flight;

    Passenger* createPassenger(char* name ,char* passport){
    Passenger* p = (Passenger*)malloc(sizeof(Passenger));

        strcpy((*p).name, name);
        strcpy((*p).passport, passport);
        (*p).next = NULL;
        return p;


    }

    void addPassenger(Flight* f,char* name,char* passport){
    Passenger* p =createPassenger(name,passport);

    if ((*f).header==NULL){
        (*f).header=p;
        (*f).last=p;
    }else{
                (*((*f).last)).next = p;
                        (*f).last = p;
    }

    }


    Flight* createFlight(char* id, FlightState state, char* date, char* time) {
    Flight* f = (Flight*)malloc(sizeof(Flight));
    strcpy((*f).id, id);
    (*f).state = state;
    (*f).status = ACTIVE;
    strcpy((*f).date, date);
    strcpy((*f).time, time);
    (*f).header = NULL;
    (*f).last = NULL;
    return f;
}

   void printPassengers(Flight* f) {
    Passenger* current = (*f).header;
    while (current != NULL) {
        printf("Passenger: %s, Passport: %s\n", (*current).name, (*current).passport);
        current = (*current).next;
    }
}

void printFlightDetail(Flight* f) {
    printf("Flight %s | Date: %s | Time: %s | State: %d | Status: %d\n",
           (*f).id, (*f).date, (*f).time, (*f).state, (*f).status);
    printPassengers(f);
}



// إضافة رحلة لقائمة (enqueue)
void enqueueFlight(Flight* flights[], int* count, Flight* f) {
    flights[(*count)++] = f;
}

// إزالة أول رحلة من القائمة (dequeue)
Flight* dequeueFlight(Flight* flights[], int* count) {
    if (*count == 0) return NULL;
    Flight* f = flights[0];
    for (int i = 1; i < *count; i++) {
        flights[i-1] = flights[i];
    }
    (*count)--;
    return f;
}

// هبوط رحلة (الأولوية للطوارئ)
void landFlight(Flight* arrivals[], int* arrivalCount,
                Flight* emergencies[], int* emergencyCount) {
    Flight* f = NULL;

    if (*emergencyCount > 0) {
        f = dequeueFlight(emergencies, emergencyCount);
    } else if (*arrivalCount > 0) {
        f = dequeueFlight(arrivals, arrivalCount);
    }

    if (f != NULL) {
        f->status = LANDED;
        printf("Flight %s has LANDED.\n", f->id);
    } else {
        printf("No flights available to land.\n");
    }
}

// إقلاع رحلة
void departFlight(Flight* departures[], int* departureCount) {
    if (*departureCount == 0) {
        printf("No flights available to depart.\n");
        return;
    }

    Flight* f = dequeueFlight(departures, departureCount);
    f->status = DEPARTED;
    printf("Flight %s has DEPARTED.\n", f->id);
}

// إلغاء رحلة
void cancelFlight(Flight* f, Flight* removed[], int* removedCount) {
    f->status = CANCELLED;
    enqueueFlight(removed, removedCount, f);
    printf("Flight %s has been CANCELLED.\n", f->id);
}

void showMenu(){
printf("..........Airport System.............. \n");
printf("1. add a flight \n ");
printf("2. add passenger to flight \n");
printf("3. land a flight \n");
printf("4. Depart a flight");
printf("5. Cancel a flight \n");
printf("6. show all flights (arrivals, departure, emergency) \n");
printf("7. show flight detail by ID \n");
printf("8. Show passengers of a flight\n");
printf("9. Add passenger to a flight by ID\n");
printf("10. Remove passenger from a flight\n");
printf("11. show flight status \n");
printf("12.exit \n");
printf("choose an option : ");



}

void loadFlights(const char* filename, Queue* arrivals, Queue* departures, Queue* emergencies) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening %s\n", filename);
        return;
    }

    char id[16], stateStr[16], date[16], time[8];
    while (fscanf(file, "%[^;];%[^;];%[^;];%s\n", id, stateStr, date, time) == 4) {
        FlightState state = ARRIVAL;
        if (strcmp(stateStr, "Departure") == 0) state = DEPARTURE;
        else if (strcmp(stateStr, "Emergency") == 0) state = EMERGENCY;

        Flight* f = createFlight(id, state, date, time);

        if (state == ARRIVAL) enqueue(arrivals, f);
        else if (state == DEPARTURE) enqueue(departures, f);
        else enqueue(emergencies, f);
    }

    fclose(file);
}

void loadPassengers(const char* filename, Queue* arrivals, Queue* departures, Queue* emergencies) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening %s\n", filename);
        return;
    }

    char name[64], passport[32], flightID[16];
    while (fscanf(file, "%[^;];%[^;];%s\n", name, passport, flightID) == 3) {
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
            printf("Passenger with passport %s removed.\n", passport);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Passenger with passport %s not found.\n", passport);
}






int main() {
    Queue arrivals = createQueue(100);
    Queue departures = createQueue(100);
    Queue emergencies = createQueue(100);
    Queue removed = createQueue(100);

    // تحميل البيانات مرة واحدة فقط
    loadFlights("flights.txt", &arrivals, &departures, &emergencies);
    loadPassengers("passengers.txt", &arrivals, &departures, &emergencies);

    int choice;
    do {
        showMenu();
        scanf("%d", &choice);

        switch(choice) {
            case 1: {
                char id[16], date[16], time[8];
                int state;
                printf("Enter Flight ID: ");
                scanf("%s", id);
                printf("Enter Flight State (0=ARRIVAL,1=DEPARTURE,2=EMERGENCY): ");
                scanf("%d", &state);
                printf("Enter Date (dd-mm-yyyy): ");
                scanf("%s", date);
                printf("Enter Time (hh:mm): ");
                scanf("%s", time);

                Flight* f = createFlight(id, (FlightState)state, date, time);

                if (state == ARRIVAL) enqueue(&arrivals, f);
                else if (state == DEPARTURE) enqueue(&departures, f);
                else enqueue(&emergencies, f);

                printf("Flight %s added.\n", id);
                break;
            }

            case 2: {
                if (!isEmpty(&arrivals)) {
                    char name[64], passport[32];
                    printf("Enter passenger name: ");
                    scanf("%s", name);
                    printf("Enter passport number: ");
                    scanf("%s", passport);

                    addPassenger(arrivals.items[arrivals.front], name, passport);
                    printf("Passenger %s added to flight %s.\n",
                           name, arrivals.items[arrivals.front]->id);
                } else {
                    printf("No arrival flights available to add passenger.\n");
                }
                break;
            }

            case 3: {
                Flight* f = NULL;
                if (!isEmpty(&emergencies)) {
                    f = dequeue(&emergencies);
                } else if (!isEmpty(&arrivals)) {
                    f = dequeue(&arrivals);
                }
                if (f != NULL) {
                    f->status = LANDED;
                    printf("Flight %s has LANDED.\n", f->id);
                } else {
                    printf("No flights available to land.\n");
                }
                break;
            }

            case 4: {
                if (!isEmpty(&departures)) {
                    Flight* f = dequeue(&departures);
                    f->status = DEPARTED;
                    printf("Flight %s has DEPARTED.\n", f->id);
                } else {
                    printf("No flights available to depart.\n");
                }
                break;
            }

            case 5: {
                if (!isEmpty(&arrivals)) {
                    Flight* f = dequeue(&arrivals);
                    f->status = CANCELLED;
                    enqueue(&removed, f);
                    printf("Flight %s has been CANCELLED.\n", f->id);
                }
                break;
            }

            case 6: {
                printf("=== Arrivals ===\n");
                for (int i = 0; i < arrivals.size; i++) {
                    printFlightDetail(arrivals.items[(arrivals.front + i) % arrivals.capacity]);
                }

                printf("\n=== Departures ===\n");
                for (int i = 0; i < departures.size; i++) {
                    printFlightDetail(departures.items[(departures.front + i) % departures.capacity]);
                }

                printf("\n=== Emergencies ===\n");
                for (int i = 0; i < emergencies.size; i++) {
                    printFlightDetail(emergencies.items[(emergencies.front + i) % emergencies.capacity]);
                }
                break;
            }

            case 7: {
                char searchID[16];
                printf("Enter Flight ID: ");
                scanf("%s", searchID);

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
                for (int i = 0; i < removed.size; i++)
                    if (strcmp(removed.items[(removed.front + i) % removed.capacity]->id, searchID) == 0)
                        target = removed.items[(removed.front + i) % removed.capacity];

                if (target) {
                    printFlightDetail(target);
                } else {
                    printf("Flight %s not found.\n", searchID);
                }
                break;
            }

            case 12:
                printf("Exiting program...\n");
                break;

            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 12);

    return 0;
}
