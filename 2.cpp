#include <iostream>
#include <list>
#include <iterator>
#include <utility>
#include <stdexcept>

// Печать списка для дебага
void print_list(const std::list<int>& lst) {
    for (const auto& x : lst)
        std::cout << x << " ";
    std::cout << "\n";
}

// Проверка: список содержит чётное количество элементов
bool is_even_sized(const std::list<int>& lst) {
    return lst.size() % 2 == 0;
}

// Получить итератор на середину списка
auto get_mid(std::list<int>& lst) {
    auto mid = lst.begin();
    std::advance(mid, lst.size() / 2);
    return mid;
}

// Функция вставки -1 после элементов первой половины списка
std::list<int> insert_neg_ones_after_first_half(std::list<int> lst) {
    if (lst.empty() || !is_even_sized(lst)) {
        throw std::invalid_argument("List must be non-empty and of even size.");
    }

    auto mid = get_mid(lst);

    // Цикл по обратному итератору первой половины
    for (auto r = std::make_reverse_iterator(mid); r != lst.rend(); ) {
        auto inserted = lst.insert(r.base(), -1); // Вставка
        r = decltype(r)(inserted);                // Обновляем итератор
        ++r;                                      // Сдвигаемся
    }

    return lst;
}

int main() {
    std::list<int> L = {10, 20, 30, 40};

    std::cout << "Original list:\n";
    print_list(L);

    try {
        std::list<int> modified = insert_neg_ones_after_first_half(L);

        std::cout << "Modified list:\n";
        print_list(modified);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
