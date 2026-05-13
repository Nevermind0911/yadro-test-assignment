#ifndef ALICE_BOT_HPP
#define ALICE_BOT_HPP

#include "bot_strategy.hpp"

class AliceBot : public BotStrategy {
public:
  int pick_next_room(const Simulator &sim) override;
  Resource pick_resource(const Simulator &sim) override;

private:
  int bfs_nearest_unvisited(const Simulator &sim) const;
};

#endif // ALICE_BOT_HPP
