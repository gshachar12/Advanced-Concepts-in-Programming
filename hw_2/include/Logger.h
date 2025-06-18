#pragma once
/**
 * @file Logger.h
 * @brief Defines a singleton Logger class for game logging functionality.
 */

#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <tuple>

#include "ActionRequest.h"

/**
 * @class Logger
 * @brief Singleton class for handling all logging activities in the game.
 * 
 * This class manages log files for standard output, errors, and input errors.
 * It follows the Singleton design pattern to ensure only one logger instance exists.
 */
class Logger {
public:
    /**
     * @brief Get the singleton instance of the logger.
     * @return Reference to the singleton Logger instance.
     */
    static Logger& getInstance();

    /**
     * @brief Log a general message to the log file.
     * @param message The message to be logged.
     */
    void log(const std::string& message);

    /**
     * @brief Log player/tank actions with their status.
     * @param actions Vector of action tuples (isValid, actionType, success, isPlayer).
     */
    void logActions(std::vector<std::tuple<bool, ActionRequest, bool, bool>> actions);

    /**
     * @brief Log game result information.
     * @param message The result message to be logged.
     */
    void logResult(const std::string& message);

    /**
     * @brief Log an error message to the error file.
     * @param message The error message to be logged.
     */
    void error(const std::string& message);

    /**
     * @brief Log input parsing errors.
     * @param message The input error message to be logged.
     */
    void inputError(const std::string& message);

    /**
     * @brief Initialize the logger with custom file paths.
     * @param path Base path for log files.
     * @return true if initialization was successful, false otherwise.
     */
    bool init(const std::string& path);

    /**
     * @brief Close all open log files.
     */
    void close();

    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Delete move constructor and assignment operator
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

private:
    /**
     * @brief Private constructor for singleton pattern.
     */
    Logger();

    /**
     * @brief Destructor ensures files are closed.
     */
    ~Logger();

    /**
     * @brief Generate a timestamp string for log entries.
     * @return Formatted timestamp string.
     */
    std::string getTimestamp() const;

    // File streams
    std::ofstream out_file;     ///< Stream for general output
    std::ofstream log_file;     ///< Stream for logging game events
    std::ofstream err_file;     ///< Stream for logging errors
    std::string input_err_file_path;  ///< Path to input error file
    std::ofstream input_err_file;     ///< Stream for logging input errors

    bool initialized{false};    ///< Flag indicating if logger is initialized
};

/**
 * @brief Mapping of action requests to human-readable strings
 */
inline std::map<ActionRequest, std::string> action_strings = {
    {ActionRequest::DoNothing, "None"},
    {ActionRequest::MoveForward, "Move Forward"},
    {ActionRequest::MoveBackward, "Move Backward"},
    {ActionRequest::RotateLeft45, "Rotate Left Eighth"},
    {ActionRequest::RotateRight45, "Rotate Right Eighth"},
    {ActionRequest::RotateLeft90, "Rotate Left Quarter"},
    {ActionRequest::RotateRight90, "Rotate Right Quarter"},
    {ActionRequest::Shoot, "Shoot"},
    {ActionRequest::GetBattleInfo, "Get Battle Info"},
};
