#include <stdio.h>
#include <stdlib.h>

int main()
{
    typedef enum {ARRIVAL ,DEPARTURE, EMERGENCY} FlightState;
    typedef enum{Active ,LANDED, DEPARTED, CANCELLED};



    typedef struct Passenger{
    char name[64];
    char passport[32];
    struct Passenger*next;
    }

    typedef struct Flight{
    char id[16];
    FlightState state;
    char date[16];
    char time[8];
    Passenger*header;
    Passenger*last;
    }


    return 0;
}
