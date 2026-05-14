#ifndef BOT_HPP
#define BOT_HPP

#include "game.hpp"
#include <fstream>

class BotAlgorithm {
public:
    virtual void run(GameState& state, std::ofstream& out) = 0;
    virtual ~BotAlgorithm() = default;
};

class AliceBot : public BotAlgorithm {
public:
    void run(GameState& state, std::ofstream& out) override;
private:
    int food;
    std::set<int> visited;
    
    void print_state(std::ofstream& out, int curr, Room& r);
    std::string find_best_res(Room& r, const std::string& target);
    std::vector<int> find_path(int start, int end, const GameState& state, bool only_visited);
};

#endif