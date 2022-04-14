#ifndef Tank_h
#define Tank_h

#include <stdint.h>
#include <Arduino.h>

#include "TimeUnit.h"
//----------------
class Tank{
public:
  inline static const uint8_t sprite_imax_{3}, sprite_jmax_{3}, sprite_bang_count_{2}, bang_frame_count_{5};
  inline static const int y_max_{17};
  
  static void SetDrawFunction(void (*DrawFunction)(int, int, bool));
  static int GetDestroyedCount();
  static int GetWinCount();
protected:
  inline static int destroyed_count_{};
  inline static int win_count_{};
  inline static void(*StoredDrawFunction)(int, int, bool){};

  bool sprite_[sprite_imax_][sprite_jmax_]{
    {1,0,1},
    {1,1,1},
    {0,1,0}
    }; 
  bool sprite_bang_anim_[sprite_bang_count_][sprite_imax_][sprite_jmax_]{
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
  int bang_anim_num_{};
  int bang_frame_{};

  bool alive_{true};
  bool bang_{false};

  int x_{}, y_{}, y_draw_{};

  TimeUnit time_unit_tick_{};
  TimeUnit time_unit_bang_{};

  
  void Draw();
public:
  Tank() = delete;
  Tank(int new_x, int new_y);
  virtual ~Tank(){}

  void Tick(unsigned long now, bool need_draw);
  void Spawn(int new_x, int new_y);
  void Draw(void (*DrawFunction)(int, int, bool));
  void Bang();
  void HardReset(int new_x, int new_y);
  void SetDelayTime(unsigned long new_delay_time);
  bool GetIsAlive() const;
  int GetCenterX() const;
  int GetCenterY() const;
};
//----------------
#endif
