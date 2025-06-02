#include <iostream>
#include <vector>
#include <map>
#include <cmath>

int last_digit_key(int x) {
    return std::abs(x) % 10;
}

std::map<int, std::vector<int>> group_by_last_digit(const std::vector<int>& V) {
    std::map<int, std::vector<int>> M;

    for (auto it = V.begin(); it != V.end(); ++it) {
        int key = last_digit_key(*it);
        M[key].push_back(*it); // сохраняем порядок
    }

    return M;
}

void print_map(const std::map<int, std::vector<int>>& M) {
    for (auto it = M.begin(); it != M.end(); ++it) {
        const auto& [key, vec] = *it;
        std::cout << key << ": ";
        for (const auto& x : vec)
            std::cout << x << " ";
        std::cout << "\n";
    }
}

int main() {
    std::vector<int> V = {12, 23, 32, 45, 13, 22};

    std::map<int, std::vector<int>> grouped = group_by_last_digit(V);

    print_map(grouped);
}
