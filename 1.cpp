#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <filesystem>
#include <optional>

namespace fs = std::filesystem;

using Words = std::vector<std::string>;

// Чтение слов из файла
auto read_words(const std::string& filename) -> std::optional<Words> {
    if (!fs::exists(filename)) return std::nullopt;
    std::ifstream fin(filename);
    return Words{
        std::istream_iterator<std::string>(fin),
        std::istream_iterator<std::string>()
    };
}

// Объединение двух наборов слов
auto combine_words(const Words& src, const Words& dst) -> Words {
    size_t count = std::min(src.size(), dst.size());
    Words result(count);
    std::transform(
        src.begin(), src.begin() + count,
                   dst.begin(),
                   result.begin(),
                   [](const auto& a, const auto& b) { return a + "-" + b; }
    );
    return result;
}

// Запись в файл
bool write_words(const std::string& filename, const Words& words) {
    std::ofstream fout(filename);
    if (!fout) return false;
    std::copy(words.begin(), words.end(), std::ostream_iterator<std::string>(fout, "\n"));
    return true;
}

int main() {
    std::string name1 = "name1.txt";
    std::string name2 = "name2.txt";
    Words name2_words = {"one", "two", "three"};

    // Функциональный pipeline с проверкой
    auto maybe_words_from_file = read_words(name1);

    if (!maybe_words_from_file.has_value()) {
        std::cerr << "Error: Input file not found or empty.\n";
        return 1;
    }

    auto combined_words = combine_words(name2_words, maybe_words_from_file.value());

    if (!write_words(name2, combined_words)) {
        std::cerr << "Error: Cannot write to output file.\n";
        return 1;
    }

    std::cout << "Processing complete. Output written to " << name2 << '\n';
    return 0;
}
