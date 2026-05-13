#include "dungeon_alg.hpp"

static const int BASE_VALUE[4] = {7, 11, 23, 1};

Room::Room(int id, std::vector<int> neighbors, const int res[4])
    : id_(id), neighbors_(std::move(neighbors)), res_{}, collected_{},
      visited_(false) {
  for (int i = 0; i < 4; ++i)
    res_[i] = res[i];
}

int Room::get_id() const { return id_; }
const std::vector<int> &Room::get_neighbors() const { return neighbors_; }
int Room::get_resource(Resource r) const { return res_[static_cast<int>(r)]; }
bool Room::is_collected(Resource r) const {
  return collected_[static_cast<int>(r)];
}
bool Room::is_visited() const { return visited_; }

Simulator::Simulator(std::vector<Room> rooms, int food, Resource target)
    : rooms_(std::move(rooms)), current_room_(0), food_(food), collected_{},
      target_(target) {
  mark_visited(0);
}

void Simulator::mark_visited(int id) { rooms_[id].visited_ = true; }

void Simulator::move_to(int room_id) {
  food_--;
  current_room_ = room_id;
  mark_visited(room_id);
}

void Simulator::collect(Resource r) {
  int idx = static_cast<int>(r);
  Room &room = rooms_[current_room_];

  // тк первый сбор бесплатный, пробегаемся по всем рес-ам и проверяем, не было
  // ли сбора, если нет -- сбор бесплатный
  bool first = true;
  for (int i = 0; i < 4; ++i)
    if (room.collected_[i]) {
      first = false;
      break;
    }

  if (!first)
    food_--;

  // в collected_[] добавляем для вывода
  collected_[idx] += room.res_[idx];
  room.res_[idx] = 0;
  room.collected_[idx] = true;
}

const Room &Simulator::get_room(int id) const { return rooms_[id]; }
int Simulator::get_current_room() const { return current_room_; }
int Simulator::get_food() const { return food_; }
int Simulator::get_collected(Resource r) const {
  return collected_[static_cast<int>(r)];
}
Resource Simulator::get_target() const { return target_; }
int Simulator::num_rooms() const { return static_cast<int>(rooms_.size()); }

int Simulator::effective_value(Resource r) const {
  return BASE_VALUE[static_cast<int>(r)] * (1 + (r == target_));
}
