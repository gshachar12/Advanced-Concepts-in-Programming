/**
 * @file SatelliteView.h
 * @brief Defines the interface for battlefield observation.
 * @author Advanced Programming Team
 */
#pragma once

#include <cstddef>

/**
 * @class SatelliteView
 * @brief Interface for observing the battlefield from above
 *
 * Provides an abstract mechanism to view the battlefield grid
 * and identify objects at specific coordinates.
 */
class SatelliteView {
public:
    /**
     * @brief Virtual destructor for proper inheritance cleanup
     */
    virtual ~SatelliteView() = default;
    
    /**
     * @brief Retrieves the character representation of an object at the specified coordinates
     * 
     * @param x X-coordinate to examine
     * @param y Y-coordinate to examine
     * @return char Character representing the object at the specified location
     */
    virtual char getObjectAt(size_t x, size_t y) const = 0;
};
