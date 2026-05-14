#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "bot.hpp"

std::vector<int> parse_adj(std::string s) {
    std::vector<int> res;
    std::replace(s.begin(), s.end(), ',', ' ');
    std::stringstream ss(s);
    int temp;
    while (ss >> temp) res.push_back(temp);
    return res;
}

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    std::ifstream in(argv[1]);
    std::ofstream out("result.txt");
    if (!in) return 1;

    GameState state;
    std::string line;

    try {
        if (!std::getline(in, line)) return 0;
        state.N = std::stoi(line);

        for (int i = 0; i <= state.N; ++i) {
            if (!std::getline(in, line)) throw std::runtime_error(line);
            std::stringstream ss(line);
            std::vector<std::string> tokens;
            std::string t;
            while (ss >> t) tokens.push_back(t);

            if (tokens.size() < 6) throw std::runtime_error(line);
            
            Room r;
            r.id = std::stoi(tokens[0]);
            r.adj = parse_adj(tokens[1]);
            r.res.iron = std::stoi(tokens[2]);
            r.res.gold = std::stoi(tokens[3]);
            r.res.gems = std::stoi(tokens[4]);
            r.res.exp = std::stoi(tokens[5]);
            state.rooms[r.id] = r;
        }

        if (!std::getline(in, line)) throw std::runtime_error("");
        std::stringstream ss_end(line);
        if (!(ss_end >> state.M >> state.target_res)) throw std::runtime_error(line);

    } catch (const std::exception& e) {
        out << e.what() << "\n";
        return 0;
    }

    AliceBot bot;
    bot.run(state, out);
    return 0;
}