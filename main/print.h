#ifndef _PRINT_H_
#define _PRINT_H_

#include "stdio.h"
#include "types.h"
#include "conio_linux.h"
#include "utils.h"

// ╔═══════════════════════╗
// ║        TERMINAL       ║
// ║           A           ║
// ║  TREM 1 120 segundos  ║
// ║  TREM 2  30 segundos  ║
// ║  TREM 3  Na estação   ║
// ║  TREM 4 240 segundos  ║
// ║                       ║
// ╚═══════════════════════╝
void print_train_status(train_t train, station_t station){
    printf("║  TREM ");
    setfontcolor(train.train_color);
    printf("%d",(train.train_number));
    setfontcolor(WHITE);
    if(train.current_index == station.station_index){
        //Train in station
        setfontcolor(GREEN);
        printf(" Na estação    ");
        setfontcolor(WHITE);
        printf("║");
    } else {
        //Calcula tempo até chegada.
        // printf("%d", time_till_arrival);
        printf(" n segundos    ║");
    }
    setfontcolor(WHITE);
}

void print_station_time_table(uint8_t x, uint8_t y, train_t active_trains[NUM_TRAINS], station_t station){
    //Checks distance between train and terminal and shows the time remaining.
    gotoxy(x,y);
    printf("╔═══════════════════════╗");
    gotoxy(x,y+1);
    printf("║        ESTAÇÃO        ║");
    gotoxy(x,y+2);
    printf("║           ");
    printf("%c",(65+station.station_id));
    printf("           ║");
    gotoxy(x,y+3);
    for (int i = 0; i < NUM_TRAINS; i++)
    {
        gotoxy(x,y+3+i);
        print_train_status(active_trains[i], station);
    }
    gotoxy(x,y+3+NUM_TRAINS);
    printf("║                       ║");
    gotoxy(x,y+4+NUM_TRAINS);
    printf("╚═══════════════════════╝");
}

#endif //_PRINT_H_