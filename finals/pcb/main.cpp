#include <forward_list>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>

class CPCB {
    struct TNode {
        std::set<std::string> connections;
    };

    std::unordered_map<std::string, TNode> nodes;

public:
    CPCB() {
    }

    ~CPCB() {
    }

    void Add(const std::string& x, const std::string& y) {
        nodes[x].connections.insert(y);
        nodes[y].connections.insert(x);
    }

    void Del(const std::string& x, const std::string& y) {
        nodes[x].connections.erase(y);
        nodes[y].connections.erase(x);
    }

    std::forward_list<std::string> Short(const std::string& x, const std::string& y) {
        std::set<std::string_view> visited;
        std::unordered_map<std::string_view, std::string_view> routes;
        std::queue<std::string_view> nodeQueue;

        nodeQueue.push(x);

        for (; !nodeQueue.empty(); nodeQueue.pop()) {
            auto& current = nodeQueue.front();

            for (const auto& connection : nodes[std::string(current)].connections) {
                if (visited.find(connection) != visited.end())
                    continue;

                visited.insert(connection);

                routes[connection] = current;
                if (connection == y)
                    goto breakout;

                nodeQueue.push(connection);
            }
        }

    breakout:

        std::forward_list<std::string> route;

        if (routes.find(y) == routes.end())
            return route;

        route.emplace_front(y);
        for (auto& current = routes[y];; current = routes[current]) {
            route.emplace_front(current);

            if (current == x)
                break;
        }

        return route;
    }
};

int main(int argc, char** argv) {
    CPCB pcb;
    pcb.Add("a", "b");
    pcb.Add("b", "c");
    pcb.Add("c", "d");
    pcb.Add("a", "c");

    auto ad = pcb.Short("a", "d");

    for (const auto& node : ad)
        std::cout << node << std::endl;
}
