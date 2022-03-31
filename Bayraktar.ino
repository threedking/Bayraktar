#include <GameBoy.h>
#include "Display.h"
#include "Tank.h"
#include "Aim.h"

GameBoy gb;
Display16x8* gb_display;

const int tanks_amount{8};
Tank* tanks[tanks_amount]{};

Aim* aim{};

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

  Aim::SetDrawFunction(&DrawPixelOnDisplay16x8);
  aim = new Aim();
}

void loop() {
  static unsigned long now{};
  static bool drawed{false};
  now = millis();
  
  ApplyButtons();
  
  for(auto &tank : tanks){
    tank->Tick(now, drawed);
  }
  
  aim->Tick(now, drawed);

  drawed = gb_display->Tick(now);
  delay(10);
}

void ApplyButtons(){
  int key = gb.getKey();
  aim->SetAimDirection( key == 4 ? Aim::AxisDirection::minus : (key == 5 ? Aim::AxisDirection::plus : Aim::AxisDirection::zero),
    key == 3 ? Aim::AxisDirection::minus : (key == 6 ? Aim::AxisDirection::plus : Aim::AxisDirection::zero) );
}

void DrawPixelOnDisplay16x8(int x, int y, bool is_on){
  gb_display->SetPixel(x , y, is_on);
}
