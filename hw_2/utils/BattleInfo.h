/**
 * @file BattleInfo.h
 * @brief Defines the interface for battlefield information access.
 * @author Advanced Programming Team
 */
#pragma once

/**
 * @class BattleInfo
 * @brief Abstract interface for accessing battlefield status information
 * 
 * Provides a common interface for various battlefield information implementations.
 * Concrete implementations will provide specific details about the current battle state.
 */
class BattleInfo {
public:
    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes
     */
    virtual ~BattleInfo() = default;
};
