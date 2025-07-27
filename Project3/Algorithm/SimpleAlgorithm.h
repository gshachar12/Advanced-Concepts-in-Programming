#ifndef SIMPLEALGORITHM_H
#define SIMPLEALGORITHM_H

#include "../common/TankAlgorithm.h"
#include "../common/ActionRequest.h"

namespace Algorithm_123456789_987654321 {

class SimpleAlgorithm final : public TankAlgorithm {
public:
    SimpleAlgorithm() = default;

    std::string getName() const override { return "SimpleAlgorithm"; }

    ActionRequest act(const SatelliteView& battle_info) override;

private:
    bool was_threatened = false;
};

} // namespace Algorithm_123456789_987654321

#endif //SIMPLEALGORITHM_H
