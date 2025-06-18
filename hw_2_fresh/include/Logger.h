#ifndef LOGGER_H
#define LOGGER_H

/**
 * @file Logger.h
 * @brief Logging facility for the tank game
 * 
 * Provides a singleton logger that handles different types of log messages
 * and writes them to appropriate files.
 */

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "ActionRequest.h"

/**
 * @brief Singleton logger class for the tank game
 * 
 * Implements logging functionality with different output streams
 * for regular logs, errors, and action tracking.
 */
class Logger {
public:
    /**
     * @brief Access the single instance of the Logger
     * @return Reference to the singleton Logger instance
     */
    static Logger &getInstance();

    /**
     * @brief Write a message to the main log file
     * @param message Text to log
     */
    void log(const std::string &message);

    /**
     * @brief Log tank actions with their outcomes
     * @param actions Vector of tuples containing (isPlayer1, ActionRequest, isSuccessful, isValid)
     */
    void logActions(std::vector<std::tuple<bool, ActionRequest, bool, bool> > actions);

    /**
     * @brief Log game result information
     * @param message Result message to log
     */
    void logResult(const std::string &message);

    /**
     * @brief Log error information to the error file
     * @param message Error message to log
     */
    void error(const std::string &message);

    /**
     * @brief Log input parsing errors
     * @param message Input error message to log
     */
    void inputError(const std::string &message);

    // Initialize with custom file paths
    bool init(const std::string &path);

    // Close log files
    void close();

    // Delete copy and assignment to ensure singleton pattern
    Logger(const Logger &) = delete;

    Logger &operator=(const Logger &) = delete;

    // Delete move and assignment to ensure singleton pattern
    Logger(Logger &&) = delete;

    Logger &operator=(Logger &&) = delete;

private:
    /**
     * @brief Private constructor - enforces singleton pattern
     */
    Logger();

    /**
     * @brief Destructor - ensures proper cleanup of file resources
     */
    ~Logger();

    /**
     * @brief Generate timestamp for log entry prefixes
     * @return Formatted timestamp string
     */
    std::string getTimestamp() const;

    // Output streams for different log types
    std::ofstream out_file;       ///< Output file stream for general game output
    std::ofstream log_file;       ///< Log file stream for diagnostic information
    std::ofstream err_file;       ///< Error file stream for runtime errors
    std::string input_err_file_path;  ///< Path to input error log file
    std::ofstream input_err_file;  ///< Input error file stream for parsing errors

    // Initialization status
    bool initialized;
};


/**
 * @brief Mapping from action enum to human-readable strings
 * 
 * Used for logging and display purposes to convert action codes
 * to meaningful text descriptions.
 */
inline std::map<ActionRequest, std::string> action_strings = {
    // No movement action
    {ActionRequest::DoNothing, "None"},
    
    // Movement actions
    {ActionRequest::MoveForward, "Move Forward"},
    {ActionRequest::MoveBackward, "Move Backward"},
    
    // Rotation actions - 45 degree increments
    {ActionRequest::RotateLeft45, "Rotate Left Eighth"},
    {ActionRequest::RotateRight45, "Rotate Right Eighth"},
    
    // Rotation actions - 90 degree increments
    {ActionRequest::RotateLeft90, "Rotate Left Quarter"},
    {ActionRequest::RotateRight90, "Rotate Right Quarter"},
    
    // Combat action
    {ActionRequest::Shoot, "Shoot"},
    
    // Information gathering
    {ActionRequest::GetBattleInfo, "Get Battle Info"},
};

#endif // LOGGER_H
