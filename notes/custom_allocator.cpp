#include <iostream>
#include <vector>
#include <memory>

template<typename T>
struct LoggingAllocator {
    using value_type = T;

    LoggingAllocator() {}

    template <class U>
    constexpr LoggingAllocator(const LoggingAllocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        static int x;
        x++;
        std::cout << "Allocating " << n << " objects " << " x is " << x << "\n";
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t n) noexcept {
        static int y;
        y++;
        std::cout << "Deallocating " << n << " objects" << " y is " << y << " p values is " << p << "\n";
        ::operator delete(p);
    }
};

int main() {
    std::vector<int, LoggingAllocator<int>> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
}
