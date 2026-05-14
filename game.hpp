#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>
#include <map>
#include <set>

struct Resources {
    int iron = 0, gold = 0, gems = 0, exp = 0;
    std::set<std::string> collected; 
};

struct Room {
    int id;
    std::vector<int> adj;
    Resources res;
};

struct GameState {
    int N;
    int M;
    std::string target_res;
    std::map<int, Room> rooms;
};

inline int get_val(const std::string& name, const std::string& target) {
    int base = 0;
    if (name == "iron") base = 7;
    else if (name == "gold") base = 11;
    else if (name == "gems") base = 23;
    else if (name == "exp") base = 1;
    return (name == target) ? base * 2 : base;
}

#endif