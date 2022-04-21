#include <GameBoy.h>
#include "Display.h"
#include "Tank.h"
#include "Aim.h"

GameBoy gb;
Display16x8* gb_display;

const int tanks_amount{6};
Tank* tanks[tanks_amount]{};

SpawnController spawn_controller;

Aim* aim{};

static unsigned long game_start{};
int level{};

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
    
  gb_display = new Display16x8(&gb);

  spawn_controller.AddLine(0);
  spawn_controller.AddLine(5);

  Tank::SetDrawFunction(&DrawPixelOnDisplay16x8);
  bool x_left{true};
  int new_y{};
  for(auto &tank : tanks){
    spawn_controller.TrySpawn(x_left ? 0 : 5, new_y);
    tank = new Tank(x_left ? 0 : 5, new_y);
    x_left = !x_left;
  }

  Aim::SetDrawFunction(&DrawPixelOnDisplay16x8);
  aim = new Aim();

  SetLevelToAll();
}

void loop() {
  static unsigned long now{};
  static bool drawed{false};
  now = millis();

  if(Tank::IsTanksLose()){
    Win();
    Restart();
    return;
  }
  if(Tank::IsTanksWin()){
    gb.testMatrix(10);
    Restart();
    return;
  }

  ApplyButtons();

  int new_level = constrain( (millis() - game_start) / 2000, 0, 4);
  if(new_level > level){
    level = new_level;
    SetLevelToAll();
  }  
  
  for(auto &tank : tanks){
    bool ticked = tank->Tick(now, drawed);

    if(ticked){
      spawn_controller.SetNeedMoveLine(tank->GetRealX());
    }
    
    if(tank->IsReadyToSpawn()){
      int new_y{};
      if(spawn_controller.TrySpawn(tank->GetRealX(), new_y)){
        tank->Spawn(tank->GetRealX(), new_y );
      }
    }
  }

  
  spawn_controller.Tick();
  
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
  Tank::ResetMain();
  for(auto &tank : tanks){
    tank->HardReset( x_left ? 0 : 5, y_start);
    x_left = !x_left;
    y_start -= x_left ? 5 : 0;
  }

  level = 0;
  SetLevelToAll();

  aim->GoToCenter();

  game_start = millis();
}

void SetLevelToAll(){
  for(auto &tank : tanks){
    //tank->SetDelayTime( (4 - level) * 50  + (5 - level) * 75);
    tank->SetDelayTime( (5 - level) * 50 + 50);
  }

  aim->SetReloadTime( pow((5 - level), 2) * 50 );

  spawn_controller.SetLevel(level);
}
