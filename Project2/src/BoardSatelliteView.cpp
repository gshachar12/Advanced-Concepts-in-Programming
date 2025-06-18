#include "BoardSatelliteView.h"

char BoardSatelliteView::getObjectAt(size_t x, size_t y) const {
    // Check if the location is outside the board
    if (!board.inBounds(x, y)) {
        return '&'; // Out of bounds character
    }
    
    // Check if the requested position is where the requesting tank is
    if (requestingTank.getX() == static_cast<int>(x) && requestingTank.getY() == static_cast<int>(y)) {
        return '%'; // The tank that requested the info
    }
    
    // Check if there's a shell at this position (shells have priority over mines)
    for (const auto& shell : shells) {
        if (shell->getX() == static_cast<int>(x) && shell->getY() == static_cast<int>(y)) {
            return '*'; // Artillery shell
        }
    }
    
    // Get the cell type at this position
    CellType cellType = board.getCellType(x, y);
    
    // Map the cell type to the appropriate character
    switch (cellType) {
        case CellType::WALL:
        case CellType::WEAK_WALL:
            return '#'; // Wall
            
        case CellType::MINE:
            return '@'; // Mine
            
        case CellType::TANK1:
            return '1'; // Tank owned by player 1
            
        case CellType::TANK2:
            return '2'; // Tank owned by player 2
            
        case CellType::EMPTY:
            return ' '; // Empty space
            
        default:
            return ' '; // Any other character is empty space
    }
}