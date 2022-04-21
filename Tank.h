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
  static bool IsTanksWin();
  static bool IsTanksLose();
  static void ResetMain();
protected:
  inline static int destroyed_count_{};
  inline static int win_count_{};
  inline static int spawned_count_{};
  inline static const int spawn_max_{50};
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
  bool bang_{};
  bool ready_to_spawn_{};

  bool auto_spawn{false};

  int x_{}, y_{}, y_draw_{};

  TimeUnit time_unit_tick_{};
  TimeUnit time_unit_bang_{};
  
  void Draw();
public:
  Tank() = delete;
  Tank(int new_x, int new_y);
  virtual ~Tank(){}

  bool Tick(unsigned long now, bool need_draw);
  void Spawn(int new_x, int new_y);
  void Draw(void (*DrawFunction)(int, int, bool));
  void Bang();
  void HardReset(int new_x, int new_y);
  void SetDelayTime(unsigned long new_delay_time);
  bool GetIsAlive() const;
  int GetCenterX() const;
  int GetCenterY() const;
  int GetRealX() const;
  int GetRealY() const;
  bool IsReadyToSpawn() const;
};
//----------------
class SpawnController{  
public:
  
protected:

  struct Line{    
    int x_{};
    int spawn_y_{};
    bool need_move{};
    Line(int new_spawn_y):spawn_y_(new_spawn_y){}
  };
  
  inline static const uint8_t lines_max_{2};
  inline static const int spawn_y_distance_min_{3}, spawn_safe_line_y_{0};
  inline static uint8_t spawn_distance_additional_{5}, spawn_distance_randmax_{10};

  uint8_t lines_count_{};
  Line lines_[lines_max_]{-spawn_y_distance_min_, -spawn_y_distance_min_};

  uint8_t GetLineIndexByXOr255(int obj_x);
public:

  void Tick();
  void AddLine(int line_x);
  bool TrySpawn(int obj_x, int& spawn_y);
  bool IsObjectOutOfSpawnZone(int obj_y);
  void SetNeedMoveLine(int obj_x);
  void SetNoNeedMoveLine(int obj_x);
  void SetLevel(int level);
};
//----------------
#endif
