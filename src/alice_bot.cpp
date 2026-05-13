#include "alice_bot.hpp"
#include <climits>
#include <queue>
#include <vector>

int AliceBot::pick_next_room(const Simulator &sim) {
  int cur = sim.get_current_room();

  for (int nb : sim.get_room(cur).get_neighbors())
    if (!sim.get_room(nb).is_visited())
      return nb;

  return bfs_nearest_unvisited(sim);
}

Resource AliceBot::pick_resource(const Simulator &sim) {
  const Room &room = sim.get_room(sim.get_current_room());

  Resource best = Resource::IRON;
  int best_val = 0;

  for (int i = 0; i < 4; ++i) {
    Resource r = static_cast<Resource>(i);
    int val = sim.effective_value(r) * room.get_resource(r);
    if (val > best_val) {
      best_val = val;
      best = r;
    }
  }

  return best;
}

int AliceBot::bfs_nearest_unvisited(const Simulator &sim) const {
  int start = sim.get_current_room();
  int n = sim.num_rooms();

  std::vector<int> dist(n, -1);
  std::vector<int> parent(n, -1);
  std::queue<int> q;

  dist[start] = 0;
  q.push(start);

  int min_dist = INT_MAX;
  int best_target = -1;

  while (!q.empty()) {
    int cur = q.front();
    q.pop();

    if (dist[cur] >= min_dist)
      continue;

    for (int nb : sim.get_room(cur).get_neighbors()) {
      if (dist[nb] != -1)
        continue;

      dist[nb] = dist[cur] + 1;
      parent[nb] = cur;

      if (!sim.get_room(nb).is_visited()) {
        if (dist[nb] < min_dist || (dist[nb] == min_dist && nb < best_target)) {
          min_dist = dist[nb];
          best_target = nb;
        }
      } else {
        q.push(nb);
      }
    }
  }

  if (best_target == -1)
    return -1;

  int step = best_target;
  while (parent[step] != start)
    step = parent[step];
  return step;
}
