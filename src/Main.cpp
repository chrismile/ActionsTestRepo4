#include <unordered_map>
#include <iostream>

// See: https://stackoverflow.com/questions/4948780/magic-number-in-boosthash-combine
template <class T>
inline void hash_combine(std::size_t& s, const T& v) {
    std::hash<T> h;
    s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

size_t doSomething() {
    size_t s = 0;
    int v = 1000;
    hash_combine(s, v);
    return s;
}
