#include <iostream>
#include <random>
#include <chrono>
#include <bitset>
#include <limits>
#include <memory>
#include <string>
#include <algorithm>

namespace BigIntUtils {

    uint64_t mod_pow(uint64_t a, uint64_t b, uint64_t m) {
        if (m == 1) return 0;
        uint64_t result = 1;
        a %= m;

        while (b > 0) {
            if (b & 1) {
                result = (result * a) % m;
            }
            a = (a * a) % m;
            b >>= 1;
        }
        return result;
    }

    uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t m) {
        if (m == 0) return 0;

        uint64_t res = 0;
        a %= m;

        while (b > 0) {
            if (b & 1) {
                res = (res + a) % m;
            }
            a = (2 * a) % m;
            b >>= 1;
        }
        return res;
    }

    uint64_t gcd(uint64_t a, uint64_t b) {
        while (b != 0) {
            uint64_t t = b;
            b = a % b;
            a = t;
        }
        return a;
    }

    int jacobi_symbol(int64_t a, int64_t n) {
        if (n <= 0 || n % 2 == 0) return 0;

        a %= n;
        int result = 1;

        while (a != 0) {
            while (a % 2 == 0) {
                a /= 2;
                if (n % 8 == 3 || n % 8 == 5) {
                    result = -result;
                }
            }
            std::swap(a, n);
            if (a % 4 == 3 && n % 4 == 3) {
                result = -result;
            }
            a %= n;
        }

        if (n == 1) return result;
        return 0;
    }
}

class RandomGenerator {
private:
    std::mt19937_64 rng;
    std::uniform_int_distribution<uint64_t> dist;

public:
    RandomGenerator() : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}

    uint64_t generate(uint64_t min, uint64_t max) {
        std::uniform_int_distribution<uint64_t> local_dist(min, max);
        return local_dist(rng);
    }

    uint64_t generate_for_test(uint64_t n) {
        if (n <= 4) return 2;
        return generate(2, n - 2);
    }
};

class PrimalityTest {
protected:
    RandomGenerator rng;
    int iterations;

public:
    PrimalityTest(int iter = 10) : iterations(iter) {}
    virtual ~PrimalityTest() = default;

    virtual bool test(uint64_t n) = 0;
    virtual std::string name() const = 0;
};

class FermatTest : public PrimalityTest {
public:
    FermatTest(int iter = 10) : PrimalityTest(iter) {}

    bool test(uint64_t n) override {
        if (n < 2) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0) return false;

        for (int i = 0; i < iterations; ++i) {
            uint64_t a = rng.generate_for_test(n);

            // Проверяем, что a и n взаимно просты
            if (BigIntUtils::gcd(a, n) != 1) {
                return false;
            }

            // Проверяем a^(n-1) mod n == 1
            if (BigIntUtils::mod_pow(a, n - 1, n) != 1) {
                return false;
            }
        }

        return true;
    }

    std::string name() const override {
        return "Тест Ферма";
    }
};

class MillerRabinTest : public PrimalityTest {
private:
    void decompose(uint64_t n_minus_1, uint64_t& d, int& s) {
        s = 0;
        d = n_minus_1;

        while (d % 2 == 0) {
            d /= 2;
            ++s;
        }
    }

public:
    MillerRabinTest(int iter = 10) : PrimalityTest(iter) {}

    bool test(uint64_t n) override {
        if (n < 2) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0) return false;

        uint64_t d;
        int s;
        decompose(n - 1, d, s);

        for (int i = 0; i < iterations; ++i) {
            uint64_t a = rng.generate_for_test(n);
            uint64_t x = BigIntUtils::mod_pow(a, d, n);

            if (x == 1 || x == n - 1) {
                continue;
            }

            bool continue_outer = false;
            for (int r = 1; r < s; ++r) {
                x = BigIntUtils::mod_mul(x, x, n);
                if (x == n - 1) {
                    continue_outer = true;
                    break;
                }
            }

            if (continue_outer) {
                continue;
            }

            return false;
        }

        return true;
    }

    std::string name() const override {
        return "Тест Миллера-Рабина";
    }
};

class SolovayStrassenTest : public PrimalityTest {
public:
    SolovayStrassenTest(int iter = 10) : PrimalityTest(iter) {}

    bool test(uint64_t n) override {
        if (n < 2) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;

        for (int i = 0; i < iterations; ++i) {
            uint64_t a = rng.generate_for_test(n);

            int jacobi = BigIntUtils::jacobi_symbol(static_cast<int64_t>(a), static_cast<int64_t>(n));
            if (jacobi == 0) {
                return false;
            }

            uint64_t mod_exp = BigIntUtils::mod_pow(a, (n - 1) / 2, n);

            int jacobi_mod = (jacobi == -1) ? static_cast<int>(n - 1) : jacobi;

            if (mod_exp != static_cast<uint64_t>(jacobi_mod % static_cast<int>(n))) {
                return false;
            }
        }

        return true;
    }

    std::string name() const override {
        return "Тест Соловея-Штрассена";
    }
};

class PrimalityTesterApp {
private:
    std::unique_ptr<PrimalityTest> current_test;
    int default_iterations = 10;

    void display_menu() {
        std::cout << "   ПРОГРАММА ПРОВЕРКИ ЧИСЕЛ НА ПРОСТОТУ\n";
        std::cout << std::endl;
        std::cout << "1. Тест Ферма\n";
        std::cout << "2. Тест Соловея-Штрассена\n";
        std::cout << "3. Тест Миллера-Рабина\n";
        std::cout << "4. Выход\n";
        std::cout << std::endl;
        std::cout << "Выберите тест (1-4): ";
    }

    uint64_t get_number_from_user() {
        uint64_t number;
        std::string input;

        while (true) {
            std::cout << "Введите число для проверки (от 2 до 2^64-1): ";
            std::cin >> input;

            try {
                // Пробуем преобразовать строку в число
                number = std::stoull(input);
                if (number < 2) {
                    std::cout << "Число должно быть больше 1!\n";
                    continue;
                }
                return number;
            } catch (const std::exception& e) {
                std::cout << "Ошибка: введите корректное число!\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    int get_iterations_from_user() {
        int iter;
        while (true) {
            std::cout << "Введите количество итераций (рекомендуется 10-50): ";
            std::cin >> iter;

            if (std::cin.fail() || iter < 1) {
                std::cout << "Ошибка: введите положительное число!\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                return iter;
            }
        }
    }

    void perform_test() {
        uint64_t number = get_number_from_user();
        int iterations = get_iterations_from_user();

        std::cout << "\nПроверка числа " << number << " с помощью "
                  << current_test->name() << " (итераций: " << iterations << ")\n";

        if (auto* fermat = dynamic_cast<FermatTest*>(current_test.get())) {
            *fermat = FermatTest(iterations);
        } else if (auto* ss = dynamic_cast<SolovayStrassenTest*>(current_test.get())) {
            *ss = SolovayStrassenTest(iterations);
        } else if (auto* mr = dynamic_cast<MillerRabinTest*>(current_test.get())) {
            *mr = MillerRabinTest(iterations);
        }

        auto start = std::chrono::high_resolution_clock::now();
        bool is_prime = current_test->test(number);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "Результат: ";
        if (is_prime) {
            std::cout << "Число " << number << " является простым (с вероятностью ошибки ~1/"
                      << (1ULL << iterations) << ")\n";
        } else {
            std::cout << "Число " << number << " является составным\n";
        }
        std::cout << "Время выполнения: " << duration.count() << " микросекунд\n";
    }

public:
    void run() {
        int choice;

        while (true) {
            display_menu();
            std::cin >> choice;

            if (std::cin.fail()) {
                std::cout << "Ошибка ввода!\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            switch (choice) {
                case 1:
                    current_test = std::make_unique<FermatTest>(default_iterations);
                    perform_test();
                    break;

                case 2:
                    current_test = std::make_unique<SolovayStrassenTest>(default_iterations);
                    perform_test();
                    break;

                case 3:
                    current_test = std::make_unique<MillerRabinTest>(default_iterations);
                    perform_test();
                    break;

                case 4:
                    std::cout << "Выход из программы.\n";
                    return;

                default:
                    std::cout << "Неверный выбор. Попробуйте снова.\n";
                    break;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "\nНажмите Enter для продолжения...";
            std::cin.get();
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");

    PrimalityTesterApp app;
    app.run();

    return 0;
}
