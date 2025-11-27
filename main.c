#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
printf("6. show arrivals \n");
printf("7.exit \n");
printf("choose an option : ");



}



int main() {
    Flight* arrivals[100]; int arrivalCount = 0;
    Flight* departures[100]; int departureCount = 0;
    Flight* emergencies[100]; int emergencyCount = 0;
    Flight* removed[100]; int removedCount = 0;

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

    if (state == ARRIVAL) enqueueFlight(arrivals, &arrivalCount, f);
    else if (state == DEPARTURE) enqueueFlight(departures, &departureCount, f);
    else enqueueFlight(emergencies, &emergencyCount, f);

    printf("Flight %s added.\n", id);
    break;
}

            case 2: {
                // إضافة راكب
                if (arrivalCount > 0) {
                    addPassenger(arrivals[0], "Ahmed", "123123");
                    printf("Passenger added.\n");
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
            case 6:
                for (int i = 0; i < arrivalCount; i++) {
                    printFlightDetail(arrivals[i]);
                }
                break;
            case 7:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 7);

    return 0;
}
