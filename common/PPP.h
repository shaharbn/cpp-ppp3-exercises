#ifndef PPP_HEADER_GUARD
#define PPP_HEADER_GUARD

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <random>
#include <stdexcept>
#include <concepts>
#include <format>
#include <chrono>
#include <functional>
#include <source_location>
#include <span>

namespace PPP {

// פונקציות שגיאה לפי הספר
[[noreturn]] inline void error(const std::string& s)
{
    throw std::runtime_error{s};
}

[[noreturn]] inline void error(const std::string& s1, const std::string& s2)
{
    throw std::runtime_error{s1 + s2};
}

[[noreturn]] inline void simple_error(const std::string& s)
{
    std::cerr << "error: " << s << '\n';
    std::exit(EXIT_FAILURE);
}

// תמיכה ב-expect() לבדיקת Preconditions ו-Postconditions
template<typename Pred>
requires std::invocable<Pred>
inline void expect(Pred p, const std::string& message = "precondition violation",
                   const std::source_location& loc = std::source_location::current())
{
    if (!p()) {
        error(message + " in function " + loc.function_name() +
              " (" + loc.file_name() + ":" + std::to_string(loc.line()) + ")");
    }
}

inline void expect(bool b, const std::string& message = "precondition violation",
                   const std::source_location& loc = std::source_location::current())
{
    if (!b) {
        error(message + " (" + loc.file_name() + ":" + std::to_string(loc.line()) + ")");
    }
}

// המרות בטוחות ומניעת Narrowing
template<typename Target, typename Source>
Target narrow(Source v)
{
    Target t = static_cast<Target>(v);
    if (static_cast<Source>(t) != v)
        error("narrowing conversion error");
    return t;
}

template<typename Target = int, typename Source>
Target round_to(Source v)
{
    return narrow<Target>(std::round(v));
}

// מחולל מספרים אקראיים ופונקציות עזר
inline std::default_random_engine& get_random_engine()
{
    static std::default_random_engine engine{std::random_device{}()};
    return engine;
}

inline void seed(int s)
{
    get_random_engine().seed(s);
}

inline void seed()
{
    get_random_engine().seed(std::random_device{}());
}

inline int random_int(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(get_random_engine());
}

inline int random_int(int max)
{
    return random_int(0, max);
}

inline std::string random_letters(int n, int m)
{
    std::string s(random_int(n, m), 'x');
    for (char& ch : s)
        ch = narrow<char>(random_int('a', 'z'));
    return s;
}

// וקטור מוגן עם Range Checking ב-operator[]
template<typename T, typename Allocator = std::allocator<T>>
class Vector : public std::vector<T, Allocator> {
public:
    using std::vector<T, Allocator>::vector;

    using size_type = typename std::vector<T, Allocator>::size_type;
    using reference = typename std::vector<T, Allocator>::reference;
    using const_reference = typename std::vector<T, Allocator>::const_reference;

    reference operator[](size_type n) {
        if (n >= this->size())
            throw std::out_of_range{"Vector::operator[] index out of range"};
        return std::vector<T, Allocator>::operator[](n);
    }

    const_reference operator[](size_type n) const {
        if (n >= this->size())
            throw std::out_of_range{"Vector::operator[] index out of range"};
        return std::vector<T, Allocator>::operator[](n);
    }
};

} // namespace PPP

// חשיפת המשתנים והווקטור המוגן ל-Global Scope לשימוש ישיר לפי תחביר הספר
using namespace std;
using namespace PPP;

#define vector PPP::Vector

#endif // PPP_HEADER_GUARD