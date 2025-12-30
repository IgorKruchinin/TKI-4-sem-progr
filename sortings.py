import random
import copy
import time

class SortingCounter:
    def __init__(self):
        self.comparisons = 0
        self.swaps = 0
        self.assignments = 0

    def reset(self):
        self.comparisons = 0
        self.swaps = 0
        self.assignments = 0

    def total_operations(self):
        return self.comparisons + self.swaps + self.assignments

    def __str__(self):
        return f"Сравнений: {self.comparisons}, Перестановок: {self.swaps}, Присваиваний: {self.assignments}, Всего: {self.total_operations()}"

def bubble_sort(arr, counter):
    n = len(arr)
    arr_copy = arr.copy()

    for i in range(n):
        for j in range(0, n - i - 1):
            counter.comparisons += 1
            if arr_copy[j] > arr_copy[j + 1]:
                arr_copy[j], arr_copy[j + 1] = arr_copy[j + 1], arr_copy[j]
                counter.swaps += 1
                counter.assignments += 2

    return arr_copy

def insertion_sort(arr, counter):
    arr_copy = arr.copy()

    for i in range(1, len(arr_copy)):
        key = arr_copy[i]
        counter.assignments += 1
        j = i - 1

        counter.comparisons += 1
        while j >= 0 and key < arr_copy[j]:
            counter.comparisons += 1
            arr_copy[j + 1] = arr_copy[j]
            counter.assignments += 1
            j -= 1

        arr_copy[j + 1] = key
        counter.assignments += 1

    return arr_copy

def quick_sort(arr, counter, low=None, high=None):
    if low is None:
        low = 0
    if high is None:
        high = len(arr) - 1

    if low < high:
        pi = partition(arr, counter, low, high)

        quick_sort(arr, counter, low, pi - 1)
        quick_sort(arr, counter, pi + 1, high)

    return arr

def partition(arr, counter, low, high):
    pivot = arr[high]
    counter.assignments += 1
    i = low - 1

    for j in range(low, high):
        counter.comparisons += 1
        if arr[j] <= pivot:
            i += 1
            if i != j:
                arr[i], arr[j] = arr[j], arr[i]
                counter.swaps += 1
                counter.assignments += 2

    if i + 1 != high:
        arr[i + 1], arr[high] = arr[high], arr[i + 1]
        counter.swaps += 1
        counter.assignments += 2

    return i + 1

def generate_random_array(n, min_val=0, max_val=1000):
    return [random.randint(min_val, max_val) for _ in range(n)]

def test_sorting_algorithms(array_size=100):
    print(f"Размер массива: {array_size}")

    original_array = generate_random_array(array_size)
    print(f"Исходный массив (первые 10 элементов): {original_array[:10]}...")
    print()

    algorithms = [
        ("Пузырьковая сортировка (2 цикла)", bubble_sort),
        ("Сортировка вставками", insertion_sort),
        ("Быстрая сортировка (Хоар)", quick_sort)
    ]

    results = []

    for name, algorithm in algorithms:
        print(f"{name}:")

        arr_copy = original_array.copy()
        counter = SortingCounter()

        start_time = time.time()

        if name == "Быстрая сортировка (Хоар)":
            sorted_array = algorithm(arr_copy, counter)
        else:
            sorted_array = algorithm(original_array, counter)

        end_time = time.time()
        execution_time = (end_time - start_time) * 1000  # в миллисекундах

        is_sorted = all(sorted_array[i] <= sorted_array[i + 1] for i in range(len(sorted_array) - 1))

        print(f"  Время выполнения: {execution_time:.2f} мс")
        print(f"  Корректно отсортирован: {'Да' if is_sorted else 'Нет'}")
        print(f"  Операции: {counter}")
        print()

        results.append({
            'name': name,
            'time': execution_time,
            'operations': counter.total_operations(),
            'comparisons': counter.comparisons,
            'swaps': counter.swaps,
            'assignments': counter.assignments
        })

    print("СРАВНЕНИЕ РЕЗУЛЬТАТОВ:")

    results.sort(key=lambda x: x['operations'])

    print("По количеству операций (меньше = лучше):")
    for i, result in enumerate(results, 1):
        print(f"{i}. {result['name']}: {result['operations']} операций")

    print()

    results.sort(key=lambda x: x['time'])

    print("По времени выполнения (меньше = лучше):")
    for i, result in enumerate(results, 1):
        print(f"{i}. {result['name']}: {result['time']:.2f} мс")

    print()

    print("ДЕТАЛЬНОЕ СРАВНЕНИЕ ОПЕРАЦИЙ:")
    print(f"{'Алгоритм':<30} {'Сравнений':<12} {'Перестановок':<12} {'Присваиваний':<12} {'Всего':<12}")

    for result in results:
        print(f"{result['name']:<30} {result['comparisons']:<12} {result['swaps']:<12} {result['assignments']:<12} {result['operations']:<12}")

def main():
    print("ПРОГРАММА СРАВНЕНИЯ АЛГОРИТМОВ СОРТИРОВКИ")

    sizes = [50, 100, 200]

    for size in sizes:
        test_sorting_algorithms(size)
        print("\n" + "=" * 60 + "\n")

    print("ДОПОЛНИТЕЛЬНЫЕ ТЕСТЫ:")

    print("\nМаленький массив (10 элементов):")
    test_sorting_algorithms(10)

    large_size = 500
    print(f"\nБольшой массив ({large_size} элементов):")

    original_array = generate_random_array(large_size)
    counter = SortingCounter()

    start_time = time.time()
    sorted_array = quick_sort(original_array.copy(), counter)
    end_time = time.time()

    print(f"Быстрая сортировка ({large_size} элементов):")
    print(f"  Время: {(end_time - start_time) * 1000:.2f} мс")
    print(f"  Операции: {counter}")

if __name__ == "__main__":
    main()
