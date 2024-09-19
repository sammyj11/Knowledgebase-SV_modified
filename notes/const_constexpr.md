## Resources:
1. The below notes are from the CppCon 2021 talk by Rainer Grimm: [Link](https://www.youtube.com/watch?v=tA6LbPyYdco)


### const
- Declare a variable const: means you cannot modify it afterwards.

- const objects:

    - must be initialised.
    - cannot be modified.
    - they cant be victim of data races. since they are read only.
    - can only invoke const member functions.

- `const member functions` of a class cannot change the state of the object.
That is, they can't change the value of member variables. 
Although they can change the value of objects which dont belong to this class.

- use `mutable` keyword, in case you want a member variable to get modified inside a `const` member function.

```cpp
int f = 100;
struct Widget {
    int a;
    mutable int c = 0;
    Widget(int init) : a(init) {}
    void test(int& p) const {
        p++; // valid since `p` doesnt belong to this class
        f++; // valid since `f` doesnt belong to this class
        c++; // valid since `c` is mutable
        // a++; // not valid
        std::cout << a << '\n';
    }
};
```

- `const char* const a`: means a is a const pointer to a const char. which means the value of neither the pointer nor the pointee can be altered.


### const_cast
- used to remove `const` or `volatile` from a variable. 

- modifying the value of  a `const` object by removing its constness is undefined behaviour.

```cpp
#include <iostream>


int main() {
    const int a = 10;
    int* b = const_cast<int*> (&a);
    *b = 11;
    std::cout << *b << '\n'; // prints 11
    std::cout << a << '\n'; // prints 10

}
```

### constexpr
- These expressions can be:
    - evaluated at the compile time (good optimisation).
    - they are thread safe.
- `const` variables are implicitly `constexpr` when initialised with some constant expression.
- they have potential to run at compile time (*not the guarantee*).

```cpp
#include <iostream>

// here this gcd function is evaluated at compile time only
constexpr int gcd(int a, int b) {
    return (b == 0) ? a : gcd(b, a % b);
}

int main() {
    // all the arguments must be known at compile time
    constexpr int result = gcd(48, 18);  // Compile-time GCD calculation
    std::cout << "GCD of 48 and 18 is: " << result << std::endl;
    return 0;
}
```
- C++20 supports the `constexpr` containers: std::vectors and std::string. 
    - Meaning, the memory is allocated and released at compile time (Transient Allocation).
    -   ```cpp
        #include <iostream>
        #include <vector>
        #include <algorithm>

        constexpr int maxElement() {
            std::vector<int> a {1, 22, 333, 44, 55};
            a.push_back(412);
            std::sort(a.begin(), a.end());
            return a.back();
        }

        int main() {
            // compile time (check godbolt assembly)
            constexpr int m = maxElement();
            std::cout << m << '\n';
        }
        ```
    
### consteval
- must run at compile time (*strong guarantee*)

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int runTime(int a) {
    return a + 1;
}

constexpr int runOrCompileTime(int a) {
    return a + 1;
}

consteval int compileTime(int a) {
    return a + 1;
}

int main() {
    // constexpr int ans1 = runTime(100); // ERROR
    constexpr int ans2 = runOrCompileTime(100);
    constexpr int ans3 = compileTime(100);

    int f = 100;
    int ans4 = runOrCompileTime(f); // Fine: Because it can be evaluated at runtime too!
    // int ans5 = compileTime(f); // ERROR: consteval must be evaluated at compile time, but `f' is not const
    // making `f' const solves the above problem
}
```

