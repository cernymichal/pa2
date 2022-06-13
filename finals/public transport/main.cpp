#include <algorithm>
#include <cassert>
#include <deque>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

class CTransport {
    std::unordered_map<std::string, std::set<std::string>> connections;

public:
    CTransport() {
    }

    ~CTransport() {
    }

    void AddLine(std::istream& is) {
        std::string first;
        std::string from;
        std::string to;
        std::getline(is, from);
        std::getline(is, to);
        first = from;

        while (true) {
            connections[from].emplace(to);

            from = to;

            std::string next;
            std::getline(is, next);

            if (next.size() == 0) {
                connections[to].emplace(first);
                break;
            }

            to = next;
        }
    }
    std::map<std::string, int> FindDest(const std::string& from, int costMax) const {
        std::map<std::string, int> costMap;
        std::queue<std::string> stopQueue;

        stopQueue.emplace(from);
        costMap[from] = 0;

        while (!stopQueue.empty()) {
            auto& current = stopQueue.front();
            auto currentCost = costMap[current] + 1;

            if (currentCost > costMax)
                break;

            auto currentConnectionsIter = connections.find(current);
            stopQueue.pop();

            if (currentConnectionsIter == connections.end())
                continue;

            for (const auto& connection : currentConnectionsIter->second) {
                if (costMap.find(connection) != costMap.end())
                    continue;

                costMap[connection] = currentCost;
                stopQueue.emplace(connection);
            }
        }

        return costMap;
    }

    friend int main();
};

int main() {
    std::istringstream iss;
    CTransport t;
    iss.clear();
    iss.str("Newton\nBlack Hill\nWood Side\nGreen Hill\nLakeside\n");
    t.AddLine(iss);
    iss.clear();
    iss.str("Great Newton\nLittle Burnside\nGreen Hill\nWood Side\n");
    t.AddLine(iss);
    iss.clear();
    iss.str("Little Newton\nLittle Burnside\nCastle Hill\nNewton Crossroad\nLakeside Central\n");
    t.AddLine(iss);
    iss.clear();
    iss.str("Tidmouth\nGordon's Hill\nSuderry\nKnapford\nGreat Waterton\nBrendam Docks\n");
    t.AddLine(iss);
    iss.clear();
    iss.str("Sodor Ironworks\nSodor Steamwork\nKnapford\nMaron\nGordon's Hill\n");
    t.AddLine(iss);
    iss.clear();
    iss.str("Lakeside Central\nLittle Waterton\n");
    t.AddLine(iss);
    iss.clear();
    iss.str("Waterton Central\nWaterton East\nWaterton Woods\nWaterton North\nWaterton East\nLittle Waterton\nWaterton West");
    t.AddLine(iss);
    assert(t.FindDest("Maron", 0) == (std::map<std::string, int>{{"Maron", 0}}));
    assert(t.FindDest("Maron", 1) == (std::map<std::string, int>{{"Gordon's Hill", 1},
                                                                 {"Maron", 0}}));
    assert(t.FindDest("Maron", 2) == (std::map<std::string, int>{{"Gordon's Hill", 1},
                                                                 {"Maron", 0},
                                                                 {"Sodor Ironworks", 2},
                                                                 {"Suderry", 2}}));
    assert(t.FindDest("Maron", 3) == (std::map<std::string, int>{{"Gordon's Hill", 1},
                                                                 {"Knapford", 3},
                                                                 {"Maron", 0},
                                                                 {"Sodor Ironworks", 2},
                                                                 {"Sodor Steamwork", 3},
                                                                 {"Suderry", 2}}));
    assert(t.FindDest("Maron", 4) == (std::map<std::string, int>{{"Gordon's Hill", 1},
                                                                 {"Great Waterton", 4},
                                                                 {"Knapford", 3},
                                                                 {"Maron", 0},
                                                                 {"Sodor Ironworks", 2},
                                                                 {"Sodor Steamwork", 3},
                                                                 {"Suderry", 2}}));
    assert(t.FindDest("Maron", 5) == (std::map<std::string, int>{{"Brendam Docks", 5},
                                                                 {"Gordon's Hill", 1},
                                                                 {"Great Waterton", 4},
                                                                 {"Knapford", 3},
                                                                 {"Maron", 0},
                                                                 {"Sodor Ironworks", 2},
                                                                 {"Sodor Steamwork", 3},
                                                                 {"Suderry", 2}}));
    assert(t.FindDest("Waterton Central", 0) == (std::map<std::string, int>{{"Waterton Central", 0}}));
    assert(t.FindDest("Waterton Central", 1) == (std::map<std::string, int>{{"Waterton Central", 0},
                                                                            {"Waterton East", 1}}));
    assert(t.FindDest("Waterton Central", 2) == (std::map<std::string, int>{{"Little Waterton", 2},
                                                                            {"Waterton Central", 0},
                                                                            {"Waterton East", 1},
                                                                            {"Waterton Woods", 2}}));
    assert(t.FindDest("Waterton Central", 3) == (std::map<std::string, int>{{"Lakeside Central", 3},
                                                                            {"Little Waterton", 2},
                                                                            {"Waterton Central", 0},
                                                                            {"Waterton East", 1},
                                                                            {"Waterton North", 3},
                                                                            {"Waterton West", 3},
                                                                            {"Waterton Woods", 2}}));
    assert(t.FindDest("Waterton Central", 4) == (std::map<std::string, int>{{"Lakeside Central", 3},
                                                                            {"Little Newton", 4},
                                                                            {"Little Waterton", 2},
                                                                            {"Waterton Central", 0},
                                                                            {"Waterton East", 1},
                                                                            {"Waterton North", 3},
                                                                            {"Waterton West", 3},
                                                                            {"Waterton Woods", 2}}));
    assert(t.FindDest("Waterton Central", 5) == (std::map<std::string, int>{{"Lakeside Central", 3},
                                                                            {"Little Burnside", 5},
                                                                            {"Little Newton", 4},
                                                                            {"Little Waterton", 2},
                                                                            {"Waterton Central", 0},
                                                                            {"Waterton East", 1},
                                                                            {"Waterton North", 3},
                                                                            {"Waterton West", 3},
                                                                            {"Waterton Woods", 2}}));
    assert(t.FindDest("Waterton Central", 6) == (std::map<std::string, int>{{"Castle Hill", 6},
                                                                            {"Green Hill", 6},
                                                                            {"Lakeside Central", 3},
                                                                            {"Little Burnside", 5},
                                                                            {"Little Newton", 4},
                                                                            {"Little Waterton", 2},
                                                                            {"Waterton Central", 0},
                                                                            {"Waterton East", 1},
                                                                            {"Waterton North", 3},
                                                                            {"Waterton West", 3},
                                                                            {"Waterton Woods", 2}}));
    assert(t.FindDest("Waterton Central", 9) == (std::map<std::string, int>{{"Black Hill", 9},
                                                                            {"Castle Hill", 6},
                                                                            {"Great Newton", 8},
                                                                            {"Green Hill", 6},
                                                                            {"Lakeside", 7},
                                                                            {"Lakeside Central", 3},
                                                                            {"Little Burnside", 5},
                                                                            {"Little Newton", 4},
                                                                            {"Little Waterton", 2},
                                                                            {"Newton", 8},
                                                                            {"Newton Crossroad", 7},
                                                                            {"Waterton Central", 0},
                                                                            {"Waterton East", 1},
                                                                            {"Waterton North", 3},
                                                                            {"Waterton West", 3},
                                                                            {"Waterton Woods", 2},
                                                                            {"Wood Side", 7}}));
    assert(t.FindDest("Waterton Central", 10) == (std::map<std::string, int>{{"Black Hill", 9},
                                                                             {"Castle Hill", 6},
                                                                             {"Great Newton", 8},
                                                                             {"Green Hill", 6},
                                                                             {"Lakeside", 7},
                                                                             {"Lakeside Central", 3},
                                                                             {"Little Burnside", 5},
                                                                             {"Little Newton", 4},
                                                                             {"Little Waterton", 2},
                                                                             {"Newton", 8},
                                                                             {"Newton Crossroad", 7},
                                                                             {"Waterton Central", 0},
                                                                             {"Waterton East", 1},
                                                                             {"Waterton North", 3},
                                                                             {"Waterton West", 3},
                                                                             {"Waterton Woods", 2},
                                                                             {"Wood Side", 7}}));
    iss.clear();
    iss.str("Newton\nNewton Crossroad\nTidmouth");
    t.AddLine(iss);
    assert(t.FindDest("Waterton East", 0) == (std::map<std::string, int>{{"Waterton East", 0}}));
    assert(t.FindDest("Waterton East", 1) == (std::map<std::string, int>{{"Little Waterton", 1},
                                                                         {"Waterton East", 0},
                                                                         {"Waterton Woods", 1}}));
    assert(t.FindDest("Waterton East", 2) == (std::map<std::string, int>{{"Lakeside Central", 2},
                                                                         {"Little Waterton", 1},
                                                                         {"Waterton East", 0},
                                                                         {"Waterton North", 2},
                                                                         {"Waterton West", 2},
                                                                         {"Waterton Woods", 1}}));
    assert(t.FindDest("Waterton East", 3) == (std::map<std::string, int>{{"Lakeside Central", 2},
                                                                         {"Little Newton", 3},
                                                                         {"Little Waterton", 1},
                                                                         {"Waterton Central", 3},
                                                                         {"Waterton East", 0},
                                                                         {"Waterton North", 2},
                                                                         {"Waterton West", 2},
                                                                         {"Waterton Woods", 1}}));
    assert(t.FindDest("Waterton East", 4) == (std::map<std::string, int>{{"Lakeside Central", 2},
                                                                         {"Little Burnside", 4},
                                                                         {"Little Newton", 3},
                                                                         {"Little Waterton", 1},
                                                                         {"Waterton Central", 3},
                                                                         {"Waterton East", 0},
                                                                         {"Waterton North", 2},
                                                                         {"Waterton West", 2},
                                                                         {"Waterton Woods", 1}}));
    assert(t.FindDest("Waterton East", 5) == (std::map<std::string, int>{{"Castle Hill", 5},
                                                                         {"Green Hill", 5},
                                                                         {"Lakeside Central", 2},
                                                                         {"Little Burnside", 4},
                                                                         {"Little Newton", 3},
                                                                         {"Little Waterton", 1},
                                                                         {"Waterton Central", 3},
                                                                         {"Waterton East", 0},
                                                                         {"Waterton North", 2},
                                                                         {"Waterton West", 2},
                                                                         {"Waterton Woods", 1}}));
    assert(t.FindDest("Waterton East", 6) == (std::map<std::string, int>{{"Castle Hill", 5},
                                                                         {"Green Hill", 5},
                                                                         {"Lakeside", 6},
                                                                         {"Lakeside Central", 2},
                                                                         {"Little Burnside", 4},
                                                                         {"Little Newton", 3},
                                                                         {"Little Waterton", 1},
                                                                         {"Newton Crossroad", 6},
                                                                         {"Waterton Central", 3},
                                                                         {"Waterton East", 0},
                                                                         {"Waterton North", 2},
                                                                         {"Waterton West", 2},
                                                                         {"Waterton Woods", 1},
                                                                         {"Wood Side", 6}}));
    assert(t.FindDest("Waterton East", 9) == (std::map<std::string, int>{{"Black Hill", 8},
                                                                         {"Castle Hill", 5},
                                                                         {"Gordon's Hill", 8},
                                                                         {"Great Newton", 7},
                                                                         {"Green Hill", 5},
                                                                         {"Lakeside", 6},
                                                                         {"Lakeside Central", 2},
                                                                         {"Little Burnside", 4},
                                                                         {"Little Newton", 3},
                                                                         {"Little Waterton", 1},
                                                                         {"Newton", 7},
                                                                         {"Newton Crossroad", 6},
                                                                         {"Sodor Ironworks", 9},
                                                                         {"Suderry", 9},
                                                                         {"Tidmouth", 7},
                                                                         {"Waterton Central", 3},
                                                                         {"Waterton East", 0},
                                                                         {"Waterton North", 2},
                                                                         {"Waterton West", 2},
                                                                         {"Waterton Woods", 1},
                                                                         {"Wood Side", 6}}));
    assert(t.FindDest("Waterton East", 10) == (std::map<std::string, int>{{"Black Hill", 8},
                                                                          {"Castle Hill", 5},
                                                                          {"Gordon's Hill", 8},
                                                                          {"Great Newton", 7},
                                                                          {"Green Hill", 5},
                                                                          {"Knapford", 10},
                                                                          {"Lakeside", 6},
                                                                          {"Lakeside Central", 2},
                                                                          {"Little Burnside", 4},
                                                                          {"Little Newton", 3},
                                                                          {"Little Waterton", 1},
                                                                          {"Newton", 7},
                                                                          {"Newton Crossroad", 6},
                                                                          {"Sodor Ironworks", 9},
                                                                          {"Sodor Steamwork", 10},
                                                                          {"Suderry", 9},
                                                                          {"Tidmouth", 7},
                                                                          {"Waterton Central", 3},
                                                                          {"Waterton East", 0},
                                                                          {"Waterton North", 2},
                                                                          {"Waterton West", 2},
                                                                          {"Waterton Woods", 1},
                                                                          {"Wood Side", 6}}));
    assert(t.FindDest("Waterton East", 11) == (std::map<std::string, int>{{"Black Hill", 8},
                                                                          {"Castle Hill", 5},
                                                                          {"Gordon's Hill", 8},
                                                                          {"Great Newton", 7},
                                                                          {"Great Waterton", 11},
                                                                          {"Green Hill", 5},
                                                                          {"Knapford", 10},
                                                                          {"Lakeside", 6},
                                                                          {"Lakeside Central", 2},
                                                                          {"Little Burnside", 4},
                                                                          {"Little Newton", 3},
                                                                          {"Little Waterton", 1},
                                                                          {"Maron", 11},
                                                                          {"Newton", 7},
                                                                          {"Newton Crossroad", 6},
                                                                          {"Sodor Ironworks", 9},
                                                                          {"Sodor Steamwork", 10},
                                                                          {"Suderry", 9},
                                                                          {"Tidmouth", 7},
                                                                          {"Waterton Central", 3},
                                                                          {"Waterton East", 0},
                                                                          {"Waterton North", 2},
                                                                          {"Waterton West", 2},
                                                                          {"Waterton Woods", 1},
                                                                          {"Wood Side", 6}}));
    assert(t.FindDest("Waterton East", 1000000) == (std::map<std::string, int>{{"Black Hill", 8},
                                                                               {"Brendam Docks", 12},
                                                                               {"Castle Hill", 5},
                                                                               {"Gordon's Hill", 8},
                                                                               {"Great Newton", 7},
                                                                               {"Great Waterton", 11},
                                                                               {"Green Hill", 5},
                                                                               {"Knapford", 10},
                                                                               {"Lakeside", 6},
                                                                               {"Lakeside Central", 2},
                                                                               {"Little Burnside", 4},
                                                                               {"Little Newton", 3},
                                                                               {"Little Waterton", 1},
                                                                               {"Maron", 11},
                                                                               {"Newton", 7},
                                                                               {"Newton Crossroad", 6},
                                                                               {"Sodor Ironworks", 9},
                                                                               {"Sodor Steamwork", 10},
                                                                               {"Suderry", 9},
                                                                               {"Tidmouth", 7},
                                                                               {"Waterton Central", 3},
                                                                               {"Waterton East", 0},
                                                                               {"Waterton North", 2},
                                                                               {"Waterton West", 2},
                                                                               {"Waterton Woods", 1},
                                                                               {"Wood Side", 6}}));
    assert(t.FindDest("Suddery Woodside", 0) == (std::map<std::string, int>{{"Suddery Woodside", 0}}));
    assert(t.FindDest("Suddery Woodside", 666) == (std::map<std::string, int>{{"Suddery Woodside", 0}}));
    return 0;
}
