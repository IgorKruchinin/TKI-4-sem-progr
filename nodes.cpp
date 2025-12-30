#include <iostream>
#include <vector>
#include <limits>
#include <random>
#include <iomanip>

class Graph {
private:
    std::vector<std::vector<int>> adjacencyMatrix;
    int n;

public:
    Graph(int nodes) : n(nodes) {
        adjacencyMatrix.resize(n, std::vector<int>(n, std::numeric_limits<int>::max()));

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 10);

        for (int i = 0; i < n; ++i) {
            adjacencyMatrix[i][i] = 0;  // Vii = 0

            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    std::uniform_int_distribution<> connectionDist(1, 10);
                    if (connectionDist(gen) > 3) {  // 70% вероятность наличия связи
                        adjacencyMatrix[i][j] = dist(gen);
                    }
                    // Иначе остается бесконечность (отсутствие связи)
                }
            }
        }
    }

    void printGraph() const {
        std::cout << "\nМатрица смежности графа:\n";
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (adjacencyMatrix[i][j] == std::numeric_limits<int>::max()) {
                    std::cout << std::setw(3) << "∞ ";
                } else {
                    std::cout << std::setw(3) << adjacencyMatrix[i][j] << " ";
                }
            }
            std::cout << "\n";
        }
    }

    std::vector<std::vector<int>> floydWarshall() const {
        std::vector<std::vector<int>> dist = adjacencyMatrix;

        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (dist[i][k] != std::numeric_limits<int>::max() &&
                        dist[k][j] != std::numeric_limits<int>::max()) {
                        dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }

        return dist;
    }

    int findShortestPath(int start, int end) const {
        if (start < 0 || start >= n || end < 0 || end >= n) {
            throw std::out_of_range("Неверные индексы узлов");
        }

        auto distances = floydWarshall();

        return distances[start][end];
    }

    int getNodeCount() const {
        return n;
    }
};

int main() {
    try {
        int n;
        std::cout << "Введите количество узлов (n): ";
        std::cin >> n;

        if (n <= 0) {
            std::cerr << "Количество узлов должно быть положительным числом.\n";
            return 1;
        }

        Graph graph(n);

        graph.printGraph();

        int start, end;
        std::cout << "\nВведите начальный узел (от 0 до " << n-1 << "): ";
        std::cin >> start;
        std::cout << "Введите конечный узел (от 0 до " << n-1 << "): ";
        std::cin >> end;

        int shortestDistance = graph.findShortestPath(start, end);

        std::cout << "\nРезультат:\n";
        std::cout << "Кратчайшее расстояние от узла " << start
                  << " до узла " << end << ": ";

        if (shortestDistance == std::numeric_limits<int>::max()) {
            std::cout << "Путь не существует (∞)\n";
        } else {
            std::cout << shortestDistance << "\n";
        }

        char showAll;
        std::cout << "\nПоказать все кратчайшие расстояния между узлами? (y/n): ";
        std::cin >> showAll;

        if (showAll == 'y' || showAll == 'Y') {
            auto allDistances = graph.floydWarshall();

            std::cout << "\nМатрица кратчайших расстояний:\n";
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (allDistances[i][j] == std::numeric_limits<int>::max()) {
                        std::cout << std::setw(3) << "∞ ";
                    } else {
                        std::cout << std::setw(3) << allDistances[i][j] << " ";
                    }
                }
                std::cout << "\n";
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
