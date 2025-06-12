#include "MyTankAlgorithmFactory.h"

#include "AdvancedPathfinder.h"
#include "BfsAlgorithm.h"
#include "SimpleAlgorithm.h"
#include "TacticalEngine.h"

unique_ptr<TankAlgorithm> MyTankAlgorithmFactory::create(int player_index, int tank_index) const {
    // Even numbered players use SimpleAlgorithm
    if (player_index % 2 == 0) {
        return std::make_unique<SimpleAlgorithm>(player_index, tank_index);
    }
    
    // Odd numbered players use more advanced algorithms
    // Different algorithms based on tank index to create more diverse behavior
    switch (tank_index % 3) {
        case 0:
            return std::make_unique<BfsAlgorithm>(player_index, tank_index);
        case 1:
            return std::make_unique<AdvancedPathfinder>(player_index, tank_index);
        case 2:
            return std::make_unique<TacticalEngine>(player_index, tank_index);
        default:
            return std::make_unique<BfsAlgorithm>(player_index, tank_index);
    }
}
