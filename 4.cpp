#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>

struct less_abs {
    bool operator()(int a, int b) const {
        return std::abs(a) < std::abs(b);
    }
};

int main() {
    int K = 5;
    std::vector<int> V = {3, -7, 0, 4, -5, 8, -2};

    // Создаём бинарный предикат как лямбду, используя функтор less_abs
    auto pred_binary = [K](int x) {
        return less_abs{}(x, K);  // |x| < |K|
    };

    // Инвертируем условие: !(|K| < |x|) => |K| >= |x| => |x| <= K
    auto pred_unary = [K](int x) {
        return !(less_abs{}(K, x));  // |x| <= K
    };

    // Считаем количество элементов, удовлетворяющих условию
    int count = std::count_if(V.begin(), V.end(), pred_unary);

    std::cout << "Количество элементов, для которых |x| <= " << K << ": " << count << "\n";
}
