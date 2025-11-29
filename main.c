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

void loadFlights(const char* filename,
                 Flight* arrivals[], int* arrivalCount,
                 Flight* departures[], int* departureCount,
                 Flight* emergencies[], int* emergencyCount) {
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

        if (state == ARRIVAL) arrivals[(*arrivalCount)++] = f;
        else if (state == DEPARTURE) departures[(*departureCount)++] = f;
        else emergencies[(*emergencyCount)++] = f;
    }

    fclose(file);
}


void loadPassengers(const char* filename,
                    Flight* arrivals[], int arrivalCount,
                    Flight* departures[], int departureCount,
                    Flight* emergencies[], int emergencyCount) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening %s\n", filename);
        return;
    }

    char name[64], passport[32], flightID[16];
    while (fscanf(file, "%[^;];%[^;];%s\n", name, passport, flightID) == 3) {
        Flight* target = NULL;

        // ابحث عن الرحلة المناسبة حسب الـ FlightID
        for (int i = 0; i < arrivalCount; i++)
            if (strcmp(arrivals[i]->id, flightID) == 0) target = arrivals[i];
        for (int i = 0; i < departureCount; i++)
            if (strcmp(departures[i]->id, flightID) == 0) target = departures[i];
        for (int i = 0; i < emergencyCount; i++)
            if (strcmp(emergencies[i]->id, flightID) == 0) target = emergencies[i];

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
    Flight* arrivals[100]; int arrivalCount = 0;
    Flight* departures[100]; int departureCount = 0;
    Flight* emergencies[100]; int emergencyCount = 0;
    Flight* removed[100]; int removedCount = 0;

    int choice;
    do {
loadFlights("flights.txt", arrivals, &arrivalCount, departures, &departureCount, emergencies, &emergencyCount);
loadPassengers("passengers.txt", arrivals, arrivalCount, departures, departureCount, emergencies, emergencyCount);

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

    if (state == ARRIVAL) enqueueFlight(arrivals, &arrivalCount, f);
    else if (state == DEPARTURE) enqueueFlight(departures, &departureCount, f);
    else enqueueFlight(emergencies, &emergencyCount, f);

    printf("Flight %s added.\n", id);
    break;
}

            case 2: {
    if (arrivalCount > 0) {
        char name[64], passport[32];
        printf("Enter passenger name: ");
        scanf("%s", name);
        printf("Enter passport number: ");
        scanf("%s", passport);

        addPassenger(arrivals[0], name, passport);
        printf("Passenger %s added to flight %s.\n", name, arrivals[0]->id);
    } else {
        printf("No arrival flights available to add passenger.\n");
    }
    break;
}

            case 3:
                landFlight(arrivals, &arrivalCount, emergencies, &emergencyCount);
                break;
            case 4:
                departFlight(departures, &departureCount);
                break;
            case 5:
                if (arrivalCount > 0) {
                    cancelFlight(arrivals[0], removed, &removedCount);
                    // إزالة من قائمة الوصول
                    dequeueFlight(arrivals, &arrivalCount);
                }
                break;
            case 6: {
    printf("=== Arrivals ===\n");
    for (int i = 0; i < arrivalCount; i++) {
        printFlightDetail(arrivals[i]);
    }

    printf("\n=== Departures ===\n");
    for (int i = 0; i < departureCount; i++) {
        printFlightDetail(departures[i]);
    }

    printf("\n=== Emergencies ===\n");
    for (int i = 0; i < emergencyCount; i++) {
        printFlightDetail(emergencies[i]);
    }
    break;
}

            case 7: {
    char searchID[16];
    printf("Enter Flight ID: ");
    scanf("%s", searchID);

    Flight* target = NULL;

    // ابحث في كل القوائم
    for (int i = 0; i < arrivalCount; i++)
        if (strcmp(arrivals[i]->id, searchID) == 0) target = arrivals[i];
    for (int i = 0; i < departureCount; i++)
        if (strcmp(departures[i]->id, searchID) == 0) target = departures[i];
    for (int i = 0; i < emergencyCount; i++)
        if (strcmp(emergencies[i]->id, searchID) == 0) target = emergencies[i];
    for (int i = 0; i < removedCount; i++)
        if (strcmp(removed[i]->id, searchID) == 0) target = removed[i];

    if (target) {
        printFlightDetail(target);
    } else {
        printf("Flight %s not found.\n", searchID);
    }
    break;
}
case 8: {
    char searchID[16];
    printf("Enter Flight ID: ");
    scanf("%s", searchID);

    Flight* target = NULL;
    for (int i = 0; i < arrivalCount; i++)
        if (strcmp(arrivals[i]->id, searchID) == 0) target = arrivals[i];
    for (int i = 0; i < departureCount; i++)
        if (strcmp(departures[i]->id, searchID) == 0) target = departures[i];
    for (int i = 0; i < emergencyCount; i++)
        if (strcmp(emergencies[i]->id, searchID) == 0) target = emergencies[i];

    if (target) printPassengers(target);
    else printf("Flight %s not found.\n", searchID);
    break;
}
case 9: {
    char searchID[16], name[64], passport[32];
    printf("Enter Flight ID: ");
    scanf("%s", searchID);

    Flight* target = NULL;
    for (int i = 0; i < arrivalCount; i++)
        if (strcmp(arrivals[i]->id, searchID) == 0) target = arrivals[i];
    for (int i = 0; i < departureCount; i++)
        if (strcmp(departures[i]->id, searchID) == 0) target = departures[i];
    for (int i = 0; i < emergencyCount; i++)
        if (strcmp(emergencies[i]->id, searchID) == 0) target = emergencies[i];

    if (target) {
        printf("Enter passenger name: ");
        scanf("%s", name);
        printf("Enter passport number: ");
        scanf("%s", passport);
        addPassenger(target, name, passport);
        printf("Passenger %s added to flight %s.\n", name, target->id);
    } else {
        printf("Flight %s not found.\n", searchID);
    }
    break;
}

case 10: {
    char searchID[16], passport[32];
    printf("Enter Flight ID: ");
    scanf("%s", searchID);

    Flight* target = NULL;
    for (int i = 0; i < arrivalCount; i++)
        if (strcmp(arrivals[i]->id, searchID) == 0) target = arrivals[i];
    for (int i = 0; i < departureCount; i++)
        if (strcmp(departures[i]->id, searchID) == 0) target = departures[i];
    for (int i = 0; i < emergencyCount; i++)
        if (strcmp(emergencies[i]->id, searchID) == 0) target = emergencies[i];

    if (target) {
        printf("Enter passport number to remove: ");
        scanf("%s", passport);
        removePassenger(target, passport);
    } else {
        printf("Flight %s not found.\n", searchID);
    }
    break;
}

case 11: {
    printf("=== Active Flights ===\n");
    for (int i = 0; i < arrivalCount; i++)
        if (arrivals[i]->status == ACTIVE)
            printf("%s %s %s | Passengers: %d\n",
                   arrivals[i]->id, arrivals[i]->date, arrivals[i]->time,
                   countPassengers(arrivals[i]));
    for (int i = 0; i < departureCount; i++)
        if (departures[i]->status == ACTIVE)
            printf("%s %s %s | Passengers: %d\n",
                   departures[i]->id, departures[i]->date, departures[i]->time,
                   countPassengers(departures[i]));

    printf("\n=== Emergency Flights ===\n");
    for (int i = 0; i < emergencyCount; i++)
        printf("%s %s %s | Passengers: %d\n",
               emergencies[i]->id, emergencies[i]->date, emergencies[i]->time,
               countPassengers(emergencies[i]));

    printf("\n=== Cancelled/Departed Flights ===\n");
    for (int i = 0; i < removedCount; i++)
        printf("%s %s %s | Status: %d | Passengers: %d\n",
               removed[i]->id, removed[i]->date, removed[i]->time,
               removed[i]->status, countPassengers(removed[i]));
    break;
}




            case 12:printf("Exiting program...\n");
                break;

            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 12);

    return 0;
}
