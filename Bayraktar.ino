#include <GameBoy.h>
#include "Display.h"
#include "Tank.h"
#include "Aim.h"

GameBoy gb;
Display16x8* gb_display;

const int tanks_amount{8};
Tank* tanks[tanks_amount]{};

Aim* aim{};

static unsigned long game_start{};
int level{};

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

  if(Tank::GetDestroyedCount() >= 50){
    Win();
    Restart();
  }  
  if(Tank::GetWinCount() >= 10){
    gb.testMatrix(10);
    Restart();
  }

  ApplyButtons();

  int new_level = constrain(map( (millis() - game_start) / 750, 0, 10, 0, 4), 0, 4);
  if(new_level > level){
    level = new_level;
    SetLevelToTanksAndAim();
  }  
  for(auto &tank : tanks){
    tank->Tick(now, drawed);
  }
  
  aim->Tick(now, drawed);

  drawed = gb_display->Tick(now);
  delay(5);
}

void ApplyButtons(){
  int key = gb.getKey();
  aim->SetAimDirection( key == 4 ? Aim::AxisDirection::minus : (key == 5 ? Aim::AxisDirection::plus : Aim::AxisDirection::zero),
    key == 3 ? Aim::AxisDirection::minus : (key == 6 ? Aim::AxisDirection::plus : Aim::AxisDirection::zero) );
  
  if(key == 1){
    if(aim->TryShoot()){
      AimShootTanks();
    }
  }  
}

void AimShootTanks(){
  for(auto &tank : tanks){
    if(tank->GetIsAlive()){
      if(aim->HasCollisionWithBox3x3(tank->GetCenterX(), tank->GetCenterY())){
        tank->Bang();
      }
    }
  }  
}

void DrawPixelOnDisplay16x8(int x, int y, bool is_on){
  gb_display->SetPixel(x , y, is_on);
}

void Win(){
  
bool win[16][8]{
  {0,0,0,0,0,0,0,0},  
  {1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1},
  {0,1,0,1,1,0,1,0},
  {0,0,1,0,0,1,0,0},  
  {0,0,0,0,0,0,0,0},  
  {0,0,1,1,1,1,0,0},
  {0,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,0},
  {0,0,1,1,1,1,0,0},  
  {0,0,0,0,0,0,0,0},  
  {0,0,1,0,0,1,0,0},
  {0,0,1,1,0,1,0,0},
  {0,0,1,0,1,1,0,0},
  {0,0,1,0,0,1,0,0},  
  {0,0,0,0,0,0,0,0}
};

  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 8; j++){
      DrawPixelOnDisplay16x8(j , i, win[i][j]);
    }
  }
  gb_display->Draw();
  delay(5000);
}
void Restart(){  
  bool x_left{true};
  int y_start{-3};
  for(auto &tank : tanks){
    tank->HardReset( x_left ? 0 : 5, y_start);
    x_left = !x_left;
    y_start -= x_left ? 5 : 0;
  }

  level = 0;
  SetLevelToTanksAndAim();

  aim->GoToCenter();

  game_start = millis();
}

void SetLevelToTanksAndAim(){
  for(auto &tank : tanks){
    tank->SetDelayTime( (5 - level) * 100 );
  }

  aim->SetReloadTime( pow((5 - level), 2) * 50 );
}
