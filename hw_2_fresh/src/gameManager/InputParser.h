#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include <unordered_set>

#include "Board.h"

class InputParser {
    std::vector<std::string> error_messages;
    std::unique_ptr<Board> board;
    std::string board_description;
    size_t width{};
    size_t height{};
    size_t max_steps{};
    size_t shells_count{};
    std::vector<std::pair<int, int> > tanks;
    const char default_symbol = ' ';
    const std::unordered_set<char> validSymbols = {'1', '2', '@', '#', '=', ' '};

    bool parseBoardConfig(std::ifstream &inFile, size_t &retrieved_data, const std::string &expected_field_name);

    void populateBoard(std::ifstream &inFile);

    void addErrorMessage(const std::string &message);

    void processLine(size_t row, const std::string &line);

    bool isValidSymbol(char c);

    void validateLineLength(size_t row, const std::string &line);

    bool parseBoardInfo(std::ifstream &inFile);

    void addErrorMessagesToLog();

    InputParser(const InputParser &) = delete;

    InputParser &operator=(const InputParser &) = delete;

    InputParser(InputParser &&) = delete;

    InputParser &operator=(InputParser &&) = delete;

public:
    InputParser() = default;

    std::unique_ptr<Board> parseInputFile(const std::string &file_name);

    bool hasErrors() const { return !error_messages.empty(); }

    std::vector<std::pair<int, int> > getTanks() const { return tanks; }

    const std::vector<std::string> &getErrorMessages() const { return error_messages; }

    ~InputParser() = default;
};

#endif //INPUTPARSER_H
