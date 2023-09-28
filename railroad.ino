#include <FastLED.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "inttypes.h"
#include "ansi.h"

#define NUM_LEDS 64 // Matriz de LED 8*8 (Ajustar NUM_LEDS para 64)
#define DATA_PIN 13
#define COLOR_ORDER GRB //Para WS2812B é necessário utilizar esse padrão de cores
#define CHIPSET WS2812B
#define VOLTS 5

#define BRIGHTNESS 100  //Ajustar para 60 no sistema real
#define MAX_AMPS 500 //mA (Ajustar para 500 no sistema real)

#define NUMBER_OF_STATIONS 8
#define TRACK_SIZE 40
#define NUMBER_OF_TRAINS 5

// uint8_t railroad[] = [];
int train_pos = 0;
uint8_t current_track;
uint8_t stations[NUMBER_OF_STATIONS] = {0+0*8,0+7*8,3+7*8,2+2*8,5+3*8,5+7*8,7+3*8,6+0*8};
uint8_t tracks[TRACK_SIZE] = {
  0+0*8,0+1*8,0+2*8,0+3*8,0+4*8,0+5*8,0+6*8,0+7*8,1+7*8,2+7*8,
  3+7*8,3+6*8,3+5*8,3+4*8,2+4*8,2+3*8,2+2*8,3+2*8,4+2*8,5+2*8,
  5+3*8,5+4*8,5+5*8,5+6*8,5+7*8,6+7*8,7+7*8,7+6*8,7+5*8,7+4*8,
  7+3*8,7+2*8,7+1*8,7+ 0*8,6+0*8,5+0*8,4+0*8,3+0*8,2+0*8,1+0*8
};

// Array de LEDS
CRGB leds[NUM_LEDS];

ANSI ansi(&Serial);

void setup_led_matrix(){
  FastLED.addLeds<CHIPSET,DATA_PIN,COLOR_ORDER>(leds,NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS); //Limita a potência máxima para proteger dependendo do tipo de alimentação.
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();  //Reinicia a matriz de LED para estado inicial
  FastLED.show();
}

void put_on_tracks(){
  for (uint8_t track : tracks) {
    leds[track] = CRGB(255, 255, 255);
  }
  FastLED.show();
}

void put_on_stations(){
  for (uint8_t station : stations) {
    leds[station] = CRGB(0, 255, 0);
  }
  FastLED.show();
}

void setup() {
  setup_led_matrix();
  put_on_tracks();
  put_on_stations();
}

void loop() {
  current_track = tracks[train_pos];
  leds[current_track] = CRGB(255, 0, 0);
  train_pos++;
  if(train_pos > TRACK_SIZE){
    train_pos = 0;
  }
  FastLED.show();
  delay(1000);
}