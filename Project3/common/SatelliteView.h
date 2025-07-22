#ifndef SATELLITE_VIEW_H
#define SATELLITE_VIEW_H

#include <cstddef>

/**
 * Interface for providing a read-only view of the game board
 * This allows algorithms to see the current state without being able to modify it
 */
class SatelliteView {
public:
    virtual ~SatelliteView() {}
    
    /**
     * Get the object at specified coordinates
     * @param x X coordinate
     * @param y Y coordinate
     * @return Character representing the object at that position
     */
    virtual char getObjectAt(size_t x, size_t y) const = 0;
};

#endif // SATELLITE_VIEW_H
