#ifndef Aim_h
#define Aim_h

#include <stdint.h>
#include <Arduino.h>

#include "TimeUnit.h"
//----------------
class Aim{
public:
  inline static const uint8_t sprite_imax_{4}, sprite_jmax_{4};

  enum class AxisDirection: int{
    minus = -1,
    zero = 0,
    plus = 1    
  };
  
  static void SetDrawFunction(void (*DrawFunction)(int, int, bool));
protected:
  inline static const int x_max_{4}, y_max_{12};  
  inline static void(*StoredDrawFunction)(int, int, bool){};
  
  bool sprite_[sprite_imax_][sprite_jmax_]{
    {1,0,0,1},
    {0,0,0,0},
    {0,0,0,0},
    {1,0,0,1}
    }; 
    
  float x_{},y_{};
  float speed_x_{},speed_y_{};
  float speed_max_{3.0f},speed_modifier_{0.2f};
  AxisDirection axis_x_{AxisDirection::zero}, axis_y_{AxisDirection::zero};
      
  TimeUnit time_unit_{};

  void ClampSpeed();
  void ClampXY();
public:
  Aim();
  virtual ~Aim(){}

  void Tick(unsigned long now, bool need_draw);
  void SetAimDirection(AxisDirection new_axis_x, AxisDirection new_axis_y);
  void Draw();
  int GetX() const;
  int GetY() const;
};
//----------------
#endif