#ifndef MYPLAYERFACTORY_H
#define MYPLAYERFACTORY_H
#include "PlayerFactory.h"

class MyPlayerFactory final : public PlayerFactory {
public:
    unique_ptr<Player> create(int player_index, size_t x, size_t y,
                              size_t max_steps, size_t shells_count) const override;
};

#endif //MYPLAYERFACTORY_H
