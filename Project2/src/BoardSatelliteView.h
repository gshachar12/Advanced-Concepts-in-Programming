#ifndef BOARD_SATELLITE_VIEW_H
#define BOARD_SATELLITE_VIEW_H

#include "common/SatelliteView.h"
#include "Board.h"
#include "GameObject.h"
#include "Tank.h"
#include <vector>
#include <memory>

class BoardSatelliteView : public SatelliteView {
private:
    const Board& board;
    const std::vector<std::unique_ptr<Tank>>& tanks;
    const std::vector<std::unique_ptr<GameObject>>& shells;
    const Tank& requestingTank;

public:
    BoardSatelliteView(const Board& board, 
                      const std::vector<std::unique_ptr<Tank>>& tanks,
                      const std::vector<std::unique_ptr<GameObject>>& shells,
                      const Tank& requestingTank)
        : board(board), tanks(tanks), shells(shells), requestingTank(requestingTank) {}

    char getObjectAt(size_t x, size_t y) const override;
};

#endif // BOARD_SATELLITE_VIEW_H