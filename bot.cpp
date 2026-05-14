#include "bot.hpp"
#include <queue>
#include <algorithm>

void AliceBot::print_state(std::ofstream& out, int curr, Room& r) {
    if (curr == 0) return;
    out << "state " << curr << " ";
    auto p = [&](int v, std::string n) {
        if (r.res.collected.count(n)) out << "_ "; else out << v << " ";
    };
    p(r.res.iron, "iron"); p(r.res.gold, "gold"); p(r.res.gems, "gems");
    out << (r.res.collected.count("exp") ? "_" : std::to_string(r.res.exp)) << "\n";
}

std::string AliceBot::find_best_res(Room& r, const std::string& target) {
    int max_v = -1; std::string best = "";
    auto check = [&](std::string n, int amt) {
        if (amt > 0 && !r.res.collected.count(n)) {
            int v = get_val(n, target);
            if (v > max_v) { max_v = v; best = n; }
        }
    };
    check("iron", r.res.iron); check("gold", r.res.gold); 
    check("gems", r.res.gems); check("exp", r.res.exp);
    return best;
}

std::vector<int> AliceBot::find_path(int start, int end, const GameState& state, bool only_visited) {
    std::queue<int> q;
    std::map<int, int> parent;
    q.push(start);
    parent[start] = -1;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == end) {
            std::vector<int> path;
            for (int v = end; v != -1; v = parent[v]) path.push_back(v);
            std::reverse(path.begin(), path.end());
            return path;
        }
        for (int v : state.rooms.at(u).adj) {
            if (parent.find(v) == parent.end() && (!only_visited || visited.count(v))) {
                parent[v] = u;
                q.push(v);
            }
        }
    }
    return {};
}

void AliceBot::run(GameState& state, std::ofstream& out) {
    food = state.M;
    int curr = 0;
    visited.insert(0);

    while (food > state.M / 2) {
        int next_r = -1;
        std::vector<int> sorted_adj = state.rooms[curr].adj;
        std::sort(sorted_adj.begin(), sorted_adj.end());

        for (int v : sorted_adj) {
            if (!visited.count(v)) { next_r = v; break; }
        }

        if (next_r == -1) break; 

        out << "go " << next_r << "\n";
        curr = next_r; visited.insert(curr); food--;

        std::string best = find_best_res(state.rooms[curr], state.target_res);
        if (!best.empty()) {
            state.rooms[curr].res.collected.insert(best);
            out << "collect " << best << "\n";
            print_state(out, curr, state.rooms[curr]);
        }
    }


    while (curr != 0) {
        std::vector<int> path = find_path(curr, 0, state, true);
        if (path.size() < 2) break;

        int dist_home = path.size() - 1;
        while (food > dist_home) {
            std::string best = find_best_res(state.rooms[curr], state.target_res);
            if (best.empty()) break;
            state.rooms[curr].res.collected.insert(best);
            food--;
            out << "collect " << best << "\n";
            print_state(out, curr, state.rooms[curr]);
        }

        curr = path[1];
        food--;
        out << "go " << curr << "\n";
        if (curr != 0) print_state(out, curr, state.rooms[curr]);
    }

    int ti = 0, tg = 0, tgm = 0, te = 0;
    for (auto const& [id, r] : state.rooms) {
        if (r.res.collected.count("iron")) ti += r.res.iron;
        if (r.res.collected.count("gold")) tg += r.res.gold;
        if (r.res.collected.count("gems")) tgm += r.res.gems;
        if (r.res.collected.count("exp"))  te  += r.res.exp;
    }
    int total = ti*get_val("iron", state.target_res) + tg*get_val("gold", state.target_res) + 
                tgm*get_val("gems", state.target_res) + te*get_val("exp", state.target_res);

    out << "result " << ti << " " << tg << " " << tgm << " " << te << " " << total << "\n";
}