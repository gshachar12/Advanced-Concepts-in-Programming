#include "Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>

Logger &Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : initialized(false) {
}

Logger::~Logger() {
    close();
}

bool Logger::init(const std::string &path) {
    const int last_dot = path.find_last_of(".");
    const int last_slash = path.find_last_of("/");
    std::string name;
    if (last_dot > last_slash) name = path.substr(last_slash + 1, last_dot - last_slash - 1);
    else name = path.substr(last_slash + 1);

    const std::string out_file_path = "output_" + name + ".txt";
    const std::string log_file_path = "log.txt";
    const std::string err_file_path = "errors.txt";
    const std::string input_err_file_path = "input_errors.txt";

    if (initialized) {
        close();
    }

    out_file.open(out_file_path, std::ios::out);
    if (!out_file.is_open()) {
        std::cerr << "Failed to open out file: " << out_file_path << std::endl;
        return false;
    }

    log_file.open(log_file_path, std::ios::out);
    if (!log_file.is_open()) {
        std::cerr << "Failed to open log file: " << log_file_path << std::endl;
        return false;
    }

    err_file.open(err_file_path, std::ios::out);
    if (!err_file.is_open()) {
        std::cerr << "Failed to open error file: " << err_file_path << std::endl;
        log_file.close();
        return false;
    }

    this->input_err_file_path = input_err_file_path;

    initialized = true;
    return true;
}

void Logger::close() {
    if (out_file.is_open()) {
        out_file.close();
    }

    if (log_file.is_open()) {
        log_file.close();
    }

    if (err_file.is_open()) {
        err_file.close();
    }

    initialized = false;
}

void Logger::log(const std::string &message) {
    if (!initialized) {
        std::cerr << "Logger not initialized, message: " << message << std::endl;
        return;
    }

    log_file << getTimestamp() << " - " << message << std::endl;
    log_file.flush();
}

void Logger::logActions(std::vector<std::tuple<bool, ActionRequest, bool, bool> > actions) {
    if (!initialized) {
        std::cerr << "Logger not initialized" << std::endl;
        return;
    }

    size_t i = 0;
    for (const auto &[gone, action, result, killed]: actions) {
        if (gone) {
            out_file << "killed";
        } else {
            out_file << action_strings[action];
            if (!result) out_file << " (ignored)";
            if (killed) out_file << " (killed)";
        }
        if (i++ < actions.size() - 1) out_file << ", ";
        else out_file << std::endl;
    }

    out_file.flush();
}

void Logger::logResult(const std::string &message) {
    if (!initialized) {
        std::cerr << "Logger not initialized" << std::endl;
        return;
    }

    out_file << message << std::endl;
    out_file.flush();
}

void Logger::error(const std::string &message) {
    if (!initialized) {
        std::cerr << "Logger not initialized, error: " << message << std::endl;
        return;
    }

    err_file << getTimestamp() << " - " << message << std::endl;
    err_file.flush();
}

void Logger::inputError(const std::string &message) {
    if (!input_err_file.is_open()) {
        input_err_file.open(input_err_file_path, std::ios::out);
    }

    if (!input_err_file.is_open()) {
        std::cerr << "Failed to open input errors file: " << input_err_file_path << std::endl;
    }

    input_err_file << message << std::endl;
    input_err_file.flush();
}

std::string Logger::getTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
