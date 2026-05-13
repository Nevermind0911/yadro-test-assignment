#include "alice_bot.hpp"
#include "bot_strategy.hpp"
#include "dungeon_alg.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

static const char *RESOURCE_NAMES[] = {"iron", "gold", "gems", "exp"};

static void write_state(std::ofstream &out, const Simulator &sim) {
  int cur = sim.get_current_room();
  const Room &room = sim.get_room(cur);
  out << "state " << cur;
  for (int i = 0; i < 4; ++i) {
    Resource r = static_cast<Resource>(i);
    if (room.is_collected(r))
      out << " _";
    else
      out << " " << room.get_resource(r);
  }
  out << "\n";
}

static void collect_extras(std::ofstream &out, Simulator &sim, BotStrategy &bot,
                           int food_floor) {
  while (sim.get_food() > food_floor) {
    Resource r = bot.pick_resource(sim);
    int cur = sim.get_current_room();
    if (sim.get_room(cur).get_resource(r) == 0)
      break;
    sim.collect(r);
    out << "collect " << RESOURCE_NAMES[static_cast<int>(r)] << "\n";
    write_state(out, sim);
  }
}

static std::vector<int> bfs_return_path(const Simulator &sim) {
  int n = sim.num_rooms();
  std::vector<int> dist(n, -1);
  std::queue<int> q;
  dist[0] = 0;
  q.push(0);
  while (!q.empty()) {
    int cur = q.front();
    q.pop();
    for (int nb : sim.get_room(cur).get_neighbors())
      if (dist[nb] == -1 && sim.get_room(nb).is_visited()) {
        dist[nb] = dist[cur] + 1;
        q.push(nb);
      }
  }

  std::vector<int> path;
  int cur = sim.get_current_room();
  while (cur != 0) {
    int next = -1;
    for (int nb : sim.get_room(cur).get_neighbors())
      if (dist[nb] == dist[cur] - 1 && (next == -1 || nb < next))
        next = nb;
    path.push_back(next);
    cur = next;
  }
  return path;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Err: No input file." << std::endl;
    return -1;
  }

  Simulator sim = parse(argv[1]);
  AliceBot bot;
  std::ofstream out("result.txt");

  int exploration_budget = sim.get_food() / 2;
  for (int moves = 0; moves < exploration_budget; ++moves) {
    int next = bot.pick_next_room(sim);
    if (next == -1)
      break;

    sim.move_to(next);
    out << "go " << next << "\n";
    write_state(out, sim);

    Resource r = bot.pick_resource(sim);
    if (sim.get_room(sim.get_current_room()).get_resource(r) > 0) {
      sim.collect(r);
      out << "collect " << RESOURCE_NAMES[static_cast<int>(r)] << "\n";
      write_state(out, sim);
    }
  }

  std::vector<int> path = bfs_return_path(sim);
  int steps_remaining = static_cast<int>(path.size());

  collect_extras(out, sim, bot, steps_remaining);

  for (int room_id : path) {
    sim.move_to(room_id);
    steps_remaining--;
    out << "go " << room_id << "\n";

    if (room_id == 0)
      break;

    write_state(out, sim);
    collect_extras(out, sim, bot, steps_remaining);
  }

  int total = 0;
  out << "result";
  for (int i = 0; i < 4; ++i) {
    Resource r = static_cast<Resource>(i);
    int qty = sim.get_collected(r);
    out << " " << qty;
    total += qty * sim.effective_value(r);
  }
  out << " " << total << "\n";

  return 0;
}
