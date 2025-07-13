#include <iostream>
#include <unordered_map>
#include <chrono>
#include <random>
#include "robin_hood.h"

using FlatMap = robin_hood::unordered_flat_map<int, int>;

constexpr size_t numElements = 1'000'000;
constexpr size_t numLookups  = 100'000;

std::vector<int> generate_keys(size_t n) {
    std::vector<int> keys(n);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 10 * n);
    for (size_t i = 0; i < n; i++) {
        keys[i] = dist(rng);
    }
    return keys;
}

template <typename MapType>
MapType benchmark_insertion(const std::vector<int>& keys, const std::string& name) {
    MapType map;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < keys.size(); i++) {
        map[keys[i]] = static_cast<int>(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << name << " insertion time: " << dur << " ms\n";
    return map;  // Return the filled map for lookup benchmark
}

template <typename MapType>
void benchmark_lookup(const std::vector<int>& keys, const MapType& map, const std::string& name) {
    size_t found = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < numLookups; i++) {
        int key = keys[i % keys.size()];
        if (map.find(key) != map.end()) {
            found++;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << name << " lookup time: " << dur << " ms (found " << found << " keys)\n";
}

int main() {
    std::cout << "Generating keys...\n";
    auto keys = generate_keys(numElements);

    // std::unordered_map
    auto umap = benchmark_insertion<std::unordered_map<int, int>>(keys, "std::unordered_map");
    benchmark_lookup(keys, umap, "std::unordered_map");

    // FlatMap (robin_hood or absl-style)
    auto fmap = benchmark_insertion<FlatMap>(keys, "FlatMap");
    benchmark_lookup(keys, fmap, "FlatMap");

    return 0;
}
