#include "Tank.h"

Tank::Tank(int new_x, int new_y): x_(new_x), y_(new_y), y_draw_(new_y){
  this->time_unit_tick_.SetDelayTime(300);
  this->time_unit_bang_.SetDelayTime(130);
  this->Spawn(this->x_, this->y_);
}

void Tank::Tick(unsigned long now, bool need_draw){
  if(this->time_unit_tick_.IsTickTime(now)){
    this->y_++;
    if(this->y_ >= this->y_max_){
      if(this->alive_){
        Tank::win_count_++;
      }
      this->alive_ = false;
      this->bang_ = false;
      this->Spawn(this->x_, -3 - random(1));
      return;
    }
    if(this->alive_){
      this->y_draw_ = this->y_;    
    }
    this->Draw();
    return;
  }
  if(this->bang_ && this->time_unit_bang_.IsTickTime(now)){
    this->bang_anim_num_ = ++this->bang_anim_num_ >= this->sprite_bang_count_ ? 0 : this->bang_anim_num_;
    this->bang_frame_++;
    if(this->bang_frame_ >= this->bang_frame_count_){
      this->bang_ = false;
    }
  }
  if(need_draw){
    this->Draw();
  } 
}

void Tank::Spawn(int new_x, int new_y){
  this->x_ = new_x;
  this->y_ = new_y;
  this->y_draw_ = new_y;
  this->alive_ = true;
  this->bang_ = false;
  this->bang_anim_num_ = 0;
  this->bang_frame_ = 0;
  this->time_unit_tick_.ForceTick(millis());
  this->time_unit_bang_.ForceTick(millis());
}

void Tank::Draw(void (*DrawFunction)(int, int, bool)){
  if(!DrawFunction){
    Serial.println("DrawFunction invalid");
    return;
  }
  if(!this->alive_ && !this->bang_ || this->alive_ && this->bang_){
    return;
  }
  
  for(int i = 0; i < Tank::sprite_imax_; i++){
    for(int j = 0; j < Tank::sprite_jmax_; j++){
      if(this->alive_){
        if(this->sprite_[i][j]){
          DrawFunction(this->x_ + j, this->y_draw_ + i, this->sprite_[i][j]);
        }
      }else{
        if(this->sprite_bang_anim_[this->bang_anim_num_][i][j]){
          DrawFunction(this->x_ + j, this->y_draw_ + i, this->sprite_bang_anim_[this->bang_anim_num_][i][j]);
        }
      }
    }
  }
}

void Tank::Draw(){
  this->Draw(this->StoredDrawFunction);
}

void Tank::SetDrawFunction(void (*DrawFunction)(int, int, bool)){
  if(!DrawFunction){
    Serial.println("Tank DrawFunction invalid");
    return;
  }
  Tank::StoredDrawFunction = DrawFunction;
}

int Tank::GetDestroyedCount(){
  return  Tank::destroyed_count_;
}

int Tank::GetWinCount(){
  return  Tank::win_count_;
}

void Tank::Bang(){
  if(this->alive_ && !this->bang_){
    this->alive_ = false;
    this->bang_ = true;
    this->time_unit_bang_.ForceTick(millis());
    Tank::destroyed_count_++;
  }
}

void Tank::HardReset(int new_x, int new_y){
  this->time_unit_tick_.SetDelayTime(1000);
  this->Spawn(new_x, new_y);
  Tank::destroyed_count_ = 0;
  Tank::win_count_ = 0;
}

void Tank::SetDelayTime(unsigned long new_delay_time){
  this->time_unit_tick_.SetDelayTime(new_delay_time);
}
  
bool Tank::GetIsAlive() const{
  return this->alive_;
}

int Tank::GetCenterX() const{
  return this->x_ + 1;
}
  
int Tank::GetCenterY() const{
  return this->y_ + 1;  
}
