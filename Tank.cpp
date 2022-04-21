#include "Tank.h"

Tank::Tank(int new_x, int new_y): x_(new_x), y_(new_y), y_draw_(new_y) {
  this->time_unit_tick_.SetDelayTime(300);
  this->time_unit_bang_.SetDelayTime(130);
  this->Spawn(this->x_, this->y_);
}

bool Tank::Tick(unsigned long now, bool need_draw) {
  bool ticked{};
  if (this->time_unit_tick_.IsTickTime(now)) {
    ticked = true;
    this->y_++;
    if (this->y_ >= Tank::y_max_) {
      if (!this->ready_to_spawn_) {
        if(this->alive_){
          Tank::win_count_++;
        }
        this->ready_to_spawn_ = true;
      }
      this->alive_ = false;
      this->bang_ = false;
      if (this->auto_spawn) {
        this->Spawn(this->x_, -3 - random(1));
      }
      return ticked;
    }
    if (this->alive_) {
      this->y_draw_ = this->y_;
    }
    this->Draw();
    return ticked;
  }
  if (this->bang_ && this->time_unit_bang_.IsTickTime(now)) {
    this->bang_anim_num_ = ++this->bang_anim_num_ >= this->sprite_bang_count_ ? 0 : this->bang_anim_num_;
    this->bang_frame_++;
    if (this->bang_frame_ >= this->bang_frame_count_) {
      this->bang_ = false;
    }
  }
  if (need_draw) {
    this->Draw();
  }
  return ticked;
}

void Tank::Spawn(int new_x, int new_y) {
  this->x_ = new_x;
  this->y_ = new_y;
  this->y_draw_ = new_y;
  this->alive_ = true;
  this->bang_ = false;
  this->ready_to_spawn_ = false;
  this->bang_anim_num_ = 0;
  this->bang_frame_ = 0;
  this->time_unit_tick_.ForceTick(millis());
  this->time_unit_bang_.ForceTick(millis());
  Tank::spawned_count_++;
}

void Tank::Draw(void (*DrawFunction)(int, int, bool)) {
  if (!DrawFunction) {
    Serial.println("DrawFunction invalid");
    return;
  }
  if (!this->alive_ && !this->bang_ || this->alive_ && this->bang_) {
    return;
  }

  for (int i = 0; i < Tank::sprite_imax_; i++) {
    for (int j = 0; j < Tank::sprite_jmax_; j++) {
      if (this->alive_) {
        if (this->sprite_[i][j]) {
          DrawFunction(this->x_ + j, this->y_draw_ + i, this->sprite_[i][j]);
        }
      } else {
        if (this->sprite_bang_anim_[this->bang_anim_num_][i][j]) {
          DrawFunction(this->x_ + j, this->y_draw_ + i, this->sprite_bang_anim_[this->bang_anim_num_][i][j]);
        }
      }
    }
  }
}

void Tank::Draw() {
  this->Draw(this->StoredDrawFunction);
}

void Tank::SetDrawFunction(void (*DrawFunction)(int, int, bool)) {
  if (!DrawFunction) {
    Serial.println("Tank DrawFunction invalid");
    return;
  }
  Tank::StoredDrawFunction = DrawFunction;
}

int Tank::GetDestroyedCount() {
  return  Tank::destroyed_count_;
}

int Tank::GetWinCount() {
  return Tank::win_count_;
}

bool Tank::IsTanksWin(){
  return Tank::win_count_ > 0;
}

bool Tank::IsTanksLose(){
  return Tank::destroyed_count_ == Tank::spawn_max_;
}

void Tank::Bang() {
  if (this->alive_ && !this->bang_) {
    this->alive_ = false;
    this->bang_ = true;
    this->time_unit_bang_.ForceTick(millis());
    Tank::destroyed_count_++;
  }
}

void Tank::HardReset(int new_x, int new_y) {
  this->time_unit_tick_.SetDelayTime(1000);
  this->Spawn(new_x, new_y);
}

void Tank::ResetMain(){
  Tank::destroyed_count_ = 0;
  Tank::win_count_ = 0;
  Tank::spawned_count_ = 0;  
}

void Tank::SetDelayTime(unsigned long new_delay_time) {
  this->time_unit_tick_.SetDelayTime(new_delay_time);
}

bool Tank::GetIsAlive() const {
  return this->alive_;
}

int Tank::GetCenterX() const {
  return this->x_ + 1;
}

int Tank::GetCenterY() const {
  return this->y_ + 1;
}

int Tank::GetRealX() const {
  return this->x_;
}

int Tank::GetRealY() const {
  return this->y_;
}

bool Tank::IsReadyToSpawn() const {
  return this->ready_to_spawn_ && Tank::spawned_count_ < Tank::spawn_max_;
}

//-------------------------
//-------------------------
//-------------------------
void SpawnController::Tick() {
  for (auto& line : this->lines_) {
    if (line.need_move) {
      if(line.spawn_y_ < SpawnController::spawn_safe_line_y_ - SpawnController::spawn_y_distance_min_){
        line.spawn_y_++;
      }
    }
    line.need_move = false;
  }
}

void SpawnController::AddLine(int line_x) {
  if (this->lines_count_ >= SpawnController::lines_max_) {
    return;
  }

  this->lines_[lines_count_++].x_ = line_x;
}

uint8_t SpawnController::GetLineIndexByXOr255(int obj_x) {
  for (int i = 0; i < this->lines_count_; i++) {
    if (this->lines_[i].x_ == obj_x) {
      return i;
    }
  }
  Serial.println("SpawnController: index of GetLineIndexByX invalid");
  return 255;
}

bool SpawnController::TrySpawn(int obj_x, int& spawn_y) {
  uint8_t line_index = this->GetLineIndexByXOr255(obj_x);
  if (line_index == 255) {
    return false;
  }

  spawn_y = this->lines_[line_index].spawn_y_;
  this->lines_[line_index].spawn_y_ += - SpawnController::spawn_y_distance_min_ - SpawnController::spawn_distance_additional_ - random(SpawnController::spawn_distance_randmax_);

  return true;
}

bool SpawnController::IsObjectOutOfSpawnZone(int obj_y) {
  return obj_y >= SpawnController::spawn_safe_line_y_;
}

void SpawnController::SetNeedMoveLine(int obj_x){
  uint8_t line_index = this->GetLineIndexByXOr255(obj_x);
  if (line_index == 255) {
    return;
  }
  this->lines_[line_index].need_move = true;
}

void SpawnController::SetNoNeedMoveLine(int obj_x){
  uint8_t line_index = this->GetLineIndexByXOr255(obj_x);
  if (line_index == 255) {
    return;
  }
  this->lines_[line_index].need_move = false;
}

void SpawnController::SetLevel(int level){
  int reverse_level = constrain(5 - level, 1, 5);
  level = 6 - reverse_level;
  SpawnController::spawn_distance_additional_ = reverse_level;
  SpawnController::spawn_distance_randmax_ = 5 + level * 2;
}
