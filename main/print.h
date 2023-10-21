#ifndef _PRINT_H_
#define _PRINT_H_

#include "types.h"
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

void print_terminals_time_tables(uint8_t x, uint8_t y, train_t active_trains[NUM_TRAINS], station_t station){
    //Checks distance between train and terminal and shows the time remaining.

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
