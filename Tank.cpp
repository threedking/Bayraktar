#include "Tank.h"

Tank::Tank(int new_x, int new_y): x_(new_x), y_(new_y){
  this->time_unit_.SetDelayTime(500);
  this->Spawn(this->x_, this->y_);
}

void Tank::Tick(unsigned long now, bool need_draw){
  if(!this->alive_){
    return;
  }
  if(this->time_unit_.IsTickTime(now)){
    this->y_++;
    if(this->y_ >= this->y_max_){
      this->alive_ = false;
      this->bang_ = false;
      this->Spawn(this->x_, -3);
      return;
    }    
    this->Draw();
    return;
  }
  if(need_draw){
    this->Draw();
  } 
}

void Tank::Spawn(int new_x, int new_y){
  this->x_ = new_x;
  this->y_ = new_y;
  this->alive_ = true;
}

void Tank::Draw(void (*DrawFunction)(int, int, bool)){
  if(!DrawFunction){
    Serial.println("DrawFunction invalid");
    return;
  }
  for(int i = 0; i < Tank::sprite_imax_; i++){
    for(int j = 0; j < Tank::sprite_jmax_; j++){
      if(this->sprite_[i][j]){
        DrawFunction(this->x_ + j, this->y_ + i, this->sprite_[i][j]);
      }
    }
  }
}

void Tank::Draw(){
  this->Draw(this->StoredDrawFunction);
}

void Tank::SetDrawFunction(void (*DrawFunction)(int, int, bool)){
  if(!DrawFunction){
    Serial.println("DrawFunction invalid");
    return;
  }
  Tank::StoredDrawFunction = DrawFunction;
}
