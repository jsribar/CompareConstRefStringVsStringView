// CompareConstRefStringVsStringView.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// https://godbolt.org/z/zG95bxvM1

#include <array>
#include <chrono>
#include <iostream>
#include <string_view>

#include <boost/utility/string_view.hpp>

class StopWatch
{
public:
    StopWatch()
    {
        m_startPoint = std::chrono::high_resolution_clock::now();
    }

    std::chrono::duration<double> Stop()
    {
        auto endPoint = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::duration<double>>(endPoint - m_startPoint);
    }

private:
    std::chrono::high_resolution_clock::time_point m_startPoint;
};

class Process
{
public:
    template <typename Str>
    void ProcessString(Str txt)
    {
        ProcessChar(txt[0]);
    }

    char ch{ 127 };

private:
    // Simulation of some processing on string content.
    inline void ProcessChar(char c)
    {
        ch %= c;
        ch += 15;
    }
};

static constexpr int repeat{ 10'000 };
const std::array<std::string, 6> text{ "Hello", "World", "Guten", "Morgen", "Dobro", "jutro" };
//const std::array<const char*, 6> text{ "Hello", "World", "Guten", "Morgen", "Dobro", "jutro" };
//const std::array<std::string_view, 6> text{ "Hello", "World", "Guten", "Morgen", "Dobro", "jutro" };

template <typename Str>
std::pair<double, int> PerformBenchmark(int count)
{
    // Result of dummy processing. Used also to check if all benchmarks provide the same result.
    int result = 127;

    Process process;

    std::chrono::duration<double> minDuration = std::chrono::duration<double>::max();

    for (int n = 0; n < count; ++n)
    {
        StopWatch sw;

        for (int i = 0; i < repeat; i++)
        {
            // Access strings interchangably to minimize or avoid cacheing or optimizations.
            process.ProcessString<Str>(text[i % text.size()]);
        }
        if (const auto duration = sw.Stop(); duration < minDuration)
        {
            minDuration = duration;
        }
        result %= (int)process.ch;
        result += 15;
    }
    // Return both duration and the result of dummy operation to avoid compiler to optimize out corresponding operations.
    return { minDuration.count(), result };
}


int main()
{
    auto BenchmarkConstStringRef = PerformBenchmark<const std::string&>;
    auto BenchmarkStringView = PerformBenchmark<std::string_view>;
    auto BenchmarkConstStringViewRef = PerformBenchmark<const std::string_view&>;
    auto BenchmarkBoostStringView = PerformBenchmark<boost::string_view>;

    const int count = 300;

    double constStringRefDuration = 0;
    double stringViewDuration = 0;
    double constStringViewRefDuration = 0;
    double boostStringViewDuration = 0;

    int resultConstStringRef = 127;
    int resultStringView = 127;
    int resultConstStringViewRef = 127;
    int resultBoostStringView = 127;

    for (int i = 0; i < 10; ++i)
    {
        // Do benchmark for each parameter type. Result of dummy processing is included to avoid optimizing it out.
        {
            const auto [duration, ch] = BenchmarkConstStringRef(count);
            constStringRefDuration += duration;
            resultConstStringRef %= ch;
            resultConstStringRef += 15;
        }
        {
            const auto [duration, ch] = BenchmarkStringView(count);
            stringViewDuration += duration;
            resultStringView %= ch;
            resultStringView += 15;
        }
        {
            const auto [duration, ch] = BenchmarkConstStringViewRef(count);
            constStringViewRefDuration += duration;
            resultConstStringViewRef %= ch;
            resultConstStringViewRef += 15;
        }
        {
            auto [duration, ch] = BenchmarkBoostStringView(count);
            boostStringViewDuration += duration;
            resultBoostStringView %= ch;
            resultBoostStringView += 15;
        }
        // Repeat benchmark for each parameter type in reverse order
        {
            auto [duration, ch] = BenchmarkBoostStringView(count);
            boostStringViewDuration += duration;
            resultBoostStringView %= ch;
            resultBoostStringView += 15;
        }
        {
            const auto [duration, ch] = BenchmarkConstStringViewRef(count);
            constStringViewRefDuration += duration;
            resultConstStringViewRef %= ch;
            resultConstStringViewRef += 15;
        }
        {
            const auto [duration, ch] = BenchmarkStringView(count);
            stringViewDuration += duration;
            resultStringView %= ch;
            resultStringView += 15;
        }
        {
            const auto [duration, ch] = BenchmarkConstStringRef(count);
            constStringRefDuration += duration;
            resultConstStringRef %= ch;
            resultConstStringRef += 15;
        }
    }

    std::cout << "const std::string&: " << constStringRefDuration << "\n";
    std::cout << resultConstStringRef << '\n';
    std::cout << "std::string_view: " << stringViewDuration << "\n";
    std::cout << resultStringView << '\n';
    std::cout << "const std::string_view&: " << constStringViewRefDuration << "\n";
    std::cout << resultConstStringViewRef << '\n';
    std::cout << "boost::string_view: " << boostStringViewDuration << "\n";
    std::cout << resultBoostStringView << '\n';
}
