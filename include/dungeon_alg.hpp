#ifndef DUNGEON_ALG_HPP
#define DUNGEON_ALG_HPP

#include <vector>

enum class Resource { IRON = 0, GOLD = 1, GEMS = 2, EXP = 3 };

class Room {
private:
  int id_;
  std::vector<int> neighbors_;
  int res_[4];
  bool collected_[4];
  bool visited_;

  friend class Simulator;

public:
  // конструктор
  Room(int id, std::vector<int> neighbors, const int res[4]);

  // геттеры
  int get_id() const;
  const std::vector<int> &get_neighbors() const;
  int get_resource(Resource r) const;
  bool is_collected(Resource r) const;
  bool is_visited() const;
};

class Simulator {
private:
  std::vector<Room> rooms_;
  int current_room_;
  int food_;
  int collected_[4];
  Resource target_;

  void mark_visited(int id);

public:
  //конструктор 
  Simulator(std::vector<Room> rooms, int food, Resource target);

  // функции класса
  void move_to(int room_id);
  void collect(Resource r);

  // геттеры
  const Room &get_room(int id) const;
  int get_current_room() const;
  int get_food() const;
  int get_collected(Resource r) const;
  Resource get_target() const;
  int num_rooms() const;
  int effective_value(Resource r) const;
};

#endif // DUNGEON_ALG_HPP
