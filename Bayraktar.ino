#include <GameBoy.h>
#include "Display.h"
#include "Tank.h"

GameBoy gb;
Display16x8* gb_display;

const int tanks_amount{8};
Tank* tanks[tanks_amount]{};

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  
  gb_display = new Display16x8(&gb);

  
  Tank::SetDrawFunction(&DrawPixelOnDisplay16x8);
  bool x_left{true};
  int y_start{-3};
  for(auto &tank : tanks){
    tank = new Tank( x_left ? 0 : 5, y_start);
    x_left = !x_left;
    y_start -= x_left ? 5 : 0;
  }
}

void loop() {
  static unsigned long now{};
  now = millis();
  static bool drawed{false};
  
  ApplyButtons();

  for(auto &tank : tanks){
    tank->Tick(now, drawed);
  }

  drawed = gb_display->Tick(now);
}

void ApplyButtons(){
  int key = gb.getKey();
}

void DrawPixelOnDisplay16x8(int x, int y, bool is_on){
  gb_display->SetPixel(x , y, is_on);
}
