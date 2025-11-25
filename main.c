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










int main()
{
    Flight*arrivals[100];
    int arrivalCount=0;

    Flight*departures[100];
    int departureCount=0;

    Flight*emergencies[100];
    int emergencyCount=0;

    Flight*removed[100];
    int removedCount=0;

   // إنشاء رحلة جديدة
    Flight* f1 = createFlight("AMM220", ARRIVAL, "20-12-2025", "23:00");

    // إضافة ركاب
    addPassenger(f1, "Ahmed", "123123");
    addPassenger(f1, "Sara", "456456");

    // تخزين الرحلة في قائمة الوصول
    arrivals[arrivalCount++] = f1;

    // طباعة تفاصيل الرحلة
    printFlightDetail(f1);


    return 0;
}
