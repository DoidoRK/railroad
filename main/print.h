#ifndef _PRINT_H_
#define _PRINT_H_

#include <stdio.h>
#include "types.h"
#include "utils.h"
#include "conio_linux.h"

// ╔═══════════════════════╗
// ║        TERMINAL       ║
// ║           A           ║
// ║  TREM 1 120 segundos  ║
// ║  TREM 2  30 segundos  ║
// ║  TREM 3  Na estação   ║
// ║  TREM 4 240 segundos  ║
// ║                       ║
// ╚═══════════════════════╝
void print_train_status(train_t train, station_t station, int time_to_arrival){
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
        printf(" %-3d segundos  ║", time_to_arrival);
    }
    setfontcolor(WHITE);
}

void print_station_time_table(const uint8_t x, const uint8_t y, train_t active_trains[NUM_TRAINS], const station_t station){
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
        int time_to_arrival = calculateTimeToStation(active_trains[i].current_index,station.station_index,
        TRAIN_STOP_IN_STATION_DELAY_IN_MS/1000,TRAIN_NORMAL_SPEED_IN_MS/1000,TRAIN_SLOW_SPEED_IN_MS/1000,
        active_trains[i].status);
        print_train_status(active_trains[i], station, time_to_arrival);
    }
    gotoxy(x,y+3+NUM_TRAINS);
    printf("║                       ║");
    gotoxy(x,y+4+NUM_TRAINS);
    printf("╚═══════════════════════╝");
}

void print_trains(train_t active_trains[NUM_TRAINS]){
    //Prints each train in the screen.
    for (uint8_t i = 0; i < NUM_TRAINS; i++)
    {
        gotoxy(active_trains[i].current_pos.x,active_trains[i].current_pos.y);
        setfontcolor(active_trains[i].train_color);
        printf("■");
    }
    setfontcolor(WHITE);
}

#endif //_PRINT_H_