// CompareConstRefStringVsStringView.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
    void ConstRefString(const std::string& txt)
    {
        ProcessChar(txt[0]);
    }

    void StringView(std::string_view txt)
    {
        ProcessChar(txt[0]);
    }

    void BoostStringView(boost::string_view txt)
    {
        ProcessChar(txt[0]);
    }

    char ch{ 127 };

private:
    inline void ProcessChar(char c)
    {
        ch %= c;
        ch += 15;
    }
};

static constexpr int repeat{ 100'000'000 };
std::string text{ "Hello world" };

void PerformConstStringRefBenchmark(int count)
{
    std::cout << "const std::string&\n";

    Process process;

    std::chrono::duration<double> minDuration = std::chrono::duration<double>::max();

    StopWatch sw;

    for (int i = 0; i < count; i++)
    {
        process.ConstRefString(text);
    }
    const auto duration = sw.Stop();
    if (duration < minDuration)
    {
        minDuration = duration;
    }
    std::cout << (int)process.ch << '\n';

    std::cout << minDuration.count() << '\n';
}

void PerformStringViewBenchmark(int count)
{
    std::cout << "std::string_view\n";

    Process process;

    std::chrono::duration<double> minDuration = std::chrono::duration<double>::max();

    StopWatch sw;

    for (int i = 0; i < count; i++)
    {
        process.StringView(text);
    }
    const auto duration = sw.Stop();
    if (duration < minDuration)
    {
        minDuration = duration;
    }
    std::cout << (int)process.ch << '\n';

    std::cout << minDuration.count() << '\n';
}

void PerformBoostStringViewBenchmark(int count)
{
    std::cout << "boost::string_view\n";

    Process process;

    std::chrono::duration<double> minDuration = std::chrono::duration<double>::max();

    StopWatch sw;

    for (int i = 0; i < count; i++)
    {
        process.BoostStringView(text);
    }
    const auto duration = sw.Stop();
    if (duration < minDuration)
    {
        minDuration = duration;
    }
    std::cout << (int)process.ch << '\n';

    std::cout << minDuration.count() << '\n';
}


int main()
{
    PerformConstStringRefBenchmark(repeat);
    PerformStringViewBenchmark(repeat);
    PerformBoostStringViewBenchmark(repeat);
}
