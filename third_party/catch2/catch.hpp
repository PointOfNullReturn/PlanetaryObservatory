#pragma once

#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace Catch
{
struct TestCase
{
    std::string name;
    std::function<void()> func;
};

inline std::vector<TestCase>& registry()
{
    static std::vector<TestCase> tests;
    return tests;
}

class AutoReg
{
public:
    AutoReg(std::string name, std::function<void()> func)
    {
        registry().push_back(TestCase{std::move(name), std::move(func)});
    }
};

inline int runTests()
{
    int failures = 0;
    for (const auto& test : registry())
    {
        try
        {
            test.func();
            std::cout << "[ OK ] " << test.name << '\n';
        }
        catch (const std::exception& ex)
        {
            ++failures;
            std::cerr << "[FAIL] " << test.name << "\n  Reason: " << ex.what() << '\n';
        }
        catch (...)
        {
            ++failures;
            std::cerr << "[FAIL] " << test.name << "\n  Reason: unknown exception" << '\n';
        }
    }

    std::cout << registry().size() - failures << " test(s) passed, " << failures
              << " failed." << std::endl;
    return failures;
}

namespace detail
{
inline std::string uniqueName(const char* base, int line)
{
    std::ostringstream oss;
    oss << base << '@' << line;
    return oss.str();
}
} // namespace detail

inline void require(bool expr, const char* exprStr, const char* file, int line)
{
    if (!expr)
    {
        std::ostringstream oss;
        oss << "Requirement failed: " << exprStr << " at " << file << ':' << line;
        throw std::runtime_error(oss.str());
    }
}

} // namespace Catch

#define CATCH_UNIQUE_NAME(base, line) base##line

#define TEST_CASE(name_literal)                                                                      \
    static void CATCH_UNIQUE_NAME(po_catch_test_, __LINE__)();                                       \
    static ::Catch::AutoReg CATCH_UNIQUE_NAME(po_catch_reg_, __LINE__)(                              \
        name_literal,                                                                               \
        []()                                                                                        \
        {                                                                                           \
            CATCH_UNIQUE_NAME(po_catch_test_, __LINE__)();                                          \
        });                                                                                         \
    static void CATCH_UNIQUE_NAME(po_catch_test_, __LINE__)()

#define REQUIRE(expr) ::Catch::require((expr), #expr, __FILE__, __LINE__)

#ifdef CATCH_CONFIG_MAIN
int main()
{
    return ::Catch::runTests();
}
#endif
