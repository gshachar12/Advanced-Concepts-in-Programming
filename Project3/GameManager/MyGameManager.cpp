#include "MyGameManager.h"
#include "../include/Logger.h"
#include <fstream>
#include <iostream>

namespace GameManager_123456789_987654321 {

GameResult MyGameManager::run(
    size_t map_width, size_t map_height,
    SatelliteView& map, // <= assume it is a snapshot, NOT updated
    size_t max_steps, size_t num_shells,
    Player& player1, Player& player2,
    TankAlgorithmFactory player1_tank_algo_factory,
    TankAlgorithmFactory player2_tank_algo_factory) {
    
    if (verbose_) {
        std::cout << "Running game on map of size " << map_width << "x" << map_height << std::endl;
        std::cout << "Max steps: " << max_steps << ", Shells: " << num_shells << std::endl;
    }

    // Create a factory that returns the players and algorithms we have
    class SimplePlayerFactory : public PlayerFactory {
        Player& p1_;
        Player& p2_;
    public:
        SimplePlayerFactory(Player& p1, Player& p2) : p1_(p1), p2_(p2) {}
        std::unique_ptr<Player> createPlayer(int player_id) override {
            if (player_id == 1) {
                return std::make_unique<Player>(p1_);
            } else {
                return std::make_unique<Player>(p2_);
            }
        }
    } simple_player_factory(player1, player2);

    class SimpleTankAlgorithmFactory : public TankAlgorithmFactory {
        TankAlgorithmFactory& factory1_;
        TankAlgorithmFactory& factory2_;
    public:
        SimpleTankAlgorithmFactory(TankAlgorithmFactory& f1, TankAlgorithmFactory& f2) 
            : factory1_(f1), factory2_(f2) {}
        std::unique_ptr<TankAlgorithm> createTankAlgorithm(int player_id) override {
            if (player_id == 1) {
                return factory1_.createTankAlgorithm(1);
            } else {
                return factory2_.createTankAlgorithm(2);
            }
        }
    } simple_tank_factory(player1_tank_algo_factory, player2_tank_algo_factory);

    // Create the original GameManager
    GameManager game_manager(simple_player_factory, simple_tank_factory);
    game_manager.setVisual(verbose_);

    // Write the map to a temporary file
    std::string temp_filename = map_name + "_temp.txt";
    std::ofstream temp_file(temp_filename);
    
    // Write map dimensions
    temp_file << map_height << " " << map_width << " " << max_steps << " " << num_shells << std::endl;
    
    // Write the map data
    for (size_t row = 0; row < map_height; ++row) {
        for (size_t col = 0; col < map_width; ++col) {
            CellType cell = map.getCellType(row, col);
            char cell_char;
            switch (cell) {
                case CellType::EMPTY: cell_char = ' '; break;
                case CellType::WALL: cell_char = '#'; break;
                case CellType::WEAK_WALL: cell_char = '@'; break;
                case CellType::TANK1: cell_char = '1'; break;
                case CellType::TANK2: cell_char = '2'; break;
                case CellType::MINE: cell_char = '*'; break;
                default: cell_char = ' '; break;
            }
            temp_file << cell_char;
        }
        temp_file << std::endl;
    }
    temp_file.close();

    // Read the board and run the game
    game_manager.readBoard(temp_filename);
    game_manager.run();

    // Get the result
    std::string result_str = game_manager.getGameResult();
    
    // Convert to GameResult
    GameResult result;
    if (result_str.find("Player 1") != std::string::npos) {
        result.winner = GameResult::PLAYER1;
    } else if (result_str.find("Player 2") != std::string::npos) {
        result.winner = GameResult::PLAYER2;
    } else {
        result.winner = GameResult::TIE;
    }
    
    result.player1_score = 0; // TODO: Extract actual scores if available
    result.player2_score = 0;
    result.steps_taken = 0;   // TODO: Extract actual step count if available

    // Clean up temp file
    std::remove(temp_filename.c_str());

    return result;
}

} // namespace GameManager_123456789_987654321
