#include "MyPlayerFactory.h"

#include <iostream>

#include "BfsPlayer.h"
#include "SimplePlayer.h"

unique_ptr<Player> MyPlayerFactory::create(int player_index, size_t x, size_t y, size_t max_steps,
                                           size_t shells_count) const {
    if (player_index % 2 == 0) {
        return std::make_unique<BfsPlayer>(player_index, x, y, max_steps, shells_count);
    }
    return std::make_unique<SimplePlayer>(player_index, x, y, max_steps, shells_count);
}
