#include "Aim.h"

Aim::Aim():x_(x_max_ / 2), y_(y_max_ / 2){
  this->time_unit_.SetDelayTime(50);
}

void Aim::Tick(unsigned long now, bool need_draw){
  static bool drawed{false};
  if(this->time_unit_.IsTickTime(now)){ 
    this->speed_x_ = this->axis_x_ == AxisDirection::zero ? 0 : this->speed_x_ +
      (this->speed_x_ > -1.0f + 1E-5 && this->speed_x_ < 1.0f - 1E-5 ? 1.0f : this->speed_modifier_)
       * static_cast<float>(this->axis_x_);
    this->speed_y_ = this->axis_y_ == AxisDirection::zero ? 0 : this->speed_y_ +
      (this->speed_y_ > -1.0f + 1E-5 && this->speed_y_ < 1.0f - 1E-5 ? 1.0f : this->speed_modifier_)
       * static_cast<float>(this->axis_y_);
    this->ClampSpeed();
    this->x_ += this->speed_x_;    
    this->y_ += this->speed_y_;
    this->ClampXY();  

    this->axis_x_ = AxisDirection::zero;
    this->axis_y_ = AxisDirection::zero;
    
    return;
  }
  if(need_draw){
    if(!drawed){
      this->Draw();
    }
    drawed =! drawed;
  } 
}

void Aim::ClampSpeed(){
  if(this->speed_x_ > this->speed_max_ + 1E-5){
    this->speed_x_ = this->speed_max_;
  }
  if(this->speed_x_ < - this->speed_max_ - 1E-5){
    this->speed_x_ = - this->speed_max_;
  }
  if(this->speed_y_ > this->speed_max_ + 1E-5){
    this->speed_y_ = this->speed_max_;
  }
  if(this->speed_y_ < - this->speed_max_ - 1E-5){
    this->speed_y_ = - this->speed_max_;
  }
}

void Aim::ClampXY(){
  if(static_cast<int>(this->x_) < 0){
    this->x_ = 0;
  }
  if(static_cast<int>(this->x_) > this->x_max_){
    this->x_ = static_cast<float>(this->x_max_);
  }
  if(static_cast<int>(this->y_) < 0){
    this->y_ = 0;
  }
  if(static_cast<int>(this->y_) > this->y_max_){
    this->y_ = static_cast<float>(this->y_max_);
  }
}

void Aim::SetAimDirection(AxisDirection new_axis_x, AxisDirection new_axis_y){  
  if(new_axis_x != AxisDirection::zero){
    this->axis_x_ = new_axis_x;
  }
  if(new_axis_y != AxisDirection::zero){
    this->axis_y_ = new_axis_y;
  }
}

int Aim::GetX() const{
  return static_cast<int>(this->x_);
}
int Aim::GetY() const{
  return static_cast<int>(this->y_);  
}

void Aim::SetDrawFunction(void (*DrawFunction)(int, int, bool)){
  if(!DrawFunction){
    Serial.println("Aim DrawFunction invalid");
    return;
  }
  Aim::StoredDrawFunction = DrawFunction;
}

void Aim::Draw(){  
  if(!this->StoredDrawFunction){
    Serial.println("DrawFunction invalid");
    return;
  }
  for(int i = 0; i < Aim::sprite_imax_; i++){
    for(int j = 0; j < Aim::sprite_jmax_; j++){
      this->StoredDrawFunction(this->GetX() + j, this->GetY() + i, this->sprite_[i][j]);
    }
  }
}
