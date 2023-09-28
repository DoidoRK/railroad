#include <FastLED.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "inttypes.h"

//Ajustar NUM_LEDS
#define NUM_LEDS 64 // Matriz de LED 8*8 (Ajustar NUM_LEDS para 64)
#define DATA_PIN 13
#define COLOR_ORDER GRB //Para WS2812B é necessário utilizar esse padrão de cores
#define CHIPSET WS2812B
#define VOLTS 5
#define BRIGHTNESS 100  //Ajustar para 60 no sistema real
#define MAX_AMPS 1000 //mA (Ajustar para 500 no sistema real)

// Array de LEDS
CRGB leds[NUM_LEDS];

void setup_led_matrix(){
  FastLED.addLeds<CHIPSET,DATA_PIN,COLOR_ORDER>(leds,NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS); //Limita a potência máxima para proteger dependendo do tipo de alimentação.
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();  //Reinicia a matriz de LED para estado inicial
  FastLED.show();
}

//Animação de Arco Íris. (Créditos: ChatGPT)
void rainbow(int delay_ms) {  
  int numColors = 256; // Número de cores que podemos trabalhar
  for (int j = 0; j < numColors; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      // Define o valor de cada LED usando o valor de matiz e iluminação
      leds[i] = CHSV(j, 255, 255);
    }
    FastLED.show(); // Atualiza as cores nas LEDs
  }
  delay(delay_ms); // Pausa entre os quadros
}

void setup() {
  setup_led_matrix();
}

void loop() { 
  rainbow(100);
}
