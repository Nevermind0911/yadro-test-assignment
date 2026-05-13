#ifndef BOT_STRATEGY_HPP
#define BOT_STRATEGY_HPP

#include "dungeon_alg.hpp"

class BotStrategy {
public:
  virtual int pick_next_room(const Simulator &sim) = 0;
  virtual Resource pick_resource(const Simulator &sim) = 0;
  virtual ~BotStrategy() = default;
};

#endif // BOT_STRATEGY_HPP
