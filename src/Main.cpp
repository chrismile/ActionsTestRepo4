#include <unordered_map>
#include <iostream>
#include <string>
#include <cstdint>

// See: https://stackoverflow.com/questions/4948780/magic-number-in-boosthash-combine
template <class T>
inline void hash_combine(std::size_t& s, const T& v) {
    std::hash<T> h;
    s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

struct CommandPoolType {
    uint32_t queueFamilyIndex = 0xFFFFFFFF; // 0xFFFFFFFF encodes standard (graphics queue).
    uint32_t threadIndex = 0;
    std::string sval = "ABC";
    unsigned long ulval = 100;
    

    bool operator==(const CommandPoolType& other) const {
        return queueFamilyIndex == other.queueFamilyIndex && other.threadIndex == threadIndex;
    }
};


namespace std {
template<> struct hash<CommandPoolType> {
    std::size_t operator()(CommandPoolType const& s) const noexcept {
        std::size_t result = 0;
        hash_combine(result, s.queueFamilyIndex);
        hash_combine(result, s.threadIndex);
        hash_combine(result, s.sval);
        hash_combine(result, s.sval);
        return result;
    }
};
}

std::unordered_map<CommandPoolType, int> doSomething() {
    size_t s = 0;
    int v = 1000;
    hash_combine(s, v);
    std::unordered_map<CommandPoolType, int> commandPools;
    commandPools.insert({ CommandPoolType{}, 0});
    return commandPools;
}
