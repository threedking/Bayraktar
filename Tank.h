#ifndef Tank_h
#define Tank_h

#include <stdint.h>
#include <Arduino.h>

#include "TimeUnit.h"
//----------------
class Tank{
public:
  inline static const uint8_t sprite_imax_{3}, sprite_jmax_{3}, sprite_count_{2};
  inline static const int y_max_{17};
  
  static void SetDrawFunction(void (*DrawFunction)(int, int, bool));
protected:
  inline static void(*StoredDrawFunction)(int, int, bool){};

  bool sprite_[sprite_imax_][sprite_jmax_]{
    {1,0,1},
    {1,1,1},
    {0,1,0}
    }; 
  bool sprite_anim_[sprite_count_][sprite_imax_][sprite_jmax_]{
      {
        {1,0,1},
        {0,0,0},
        {1,0,1}
      },
      {
        {0,1,0},
        {1,0,1},
        {0,1,0}
      }
    }; 

  bool alive_{true};
  bool bang_{false};

  int x_{}, y_{};

  TimeUnit time_unit_{};

  
  void Draw();
public:
  Tank() = delete;
  Tank(int new_x, int new_y);
  virtual ~Tank(){}

  void Tick(unsigned long now, bool need_draw);
  void Spawn(int new_x, int new_y);
  void Draw(void (*DrawFunction)(int, int, bool));
  int GetCenterX() const;
  int GetCenterY() const;
};
//----------------
#endif
