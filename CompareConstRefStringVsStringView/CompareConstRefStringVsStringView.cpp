#include <array>
#include <chrono>
#include <iostream>
#include <string_view>
#include <type_traits>

#include <boost/utility/string_view.hpp>

class StopWatch
{
public:
    StopWatch()
    {
        m_startPoint = std::chrono::high_resolution_clock::now();
    }

    std::chrono::duration<double> Stop() const
    {
        auto endPoint = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::duration<double>>(endPoint - m_startPoint);
    }

private:
    std::chrono::high_resolution_clock::time_point m_startPoint;
};

static constexpr int repeat{ 3'000 };
static constexpr int loops{ 30 };

const std::array<std::string, 6> string_array{ "Hello", "World", "Guten", "Morgen", "Dobro", "jutro" };
const std::array<const char*, 6> const_char_array{ "Hello", "World", "Guten", "Morgen", "Dobro", "jutro" };
const std::array<boost::string_view, 6> string_view_array{ "Hello", "World", "Guten", "Morgen", "Dobro", "jutro" };

template <typename Array>
class Process
{
public:
    explicit Process(const Array& array) : array(array)
    {
    }

    template <typename Str>
    void ProcessString(Str txt)
    {
        ProcessChar(txt[0]);
    }

    template <typename Str>
    Str ReturnString(int i)
    {
        return array[i % array.size()];
    }

    char ch{ 127 };

private:
    // Simulation of some processing on string content.
    inline void ProcessChar(char c)
    {
        ch %= c;
        ch += 15;
    }

    const Array& array;
};

template <typename Str, typename Array>
std::pair<double, int> ArgumentBechmark(int count, const Array& array)
{
    // Result of dummy processing. Used also to check if all benchmarks provide the same result.
    int result = 127;

    Process process(array);

    std::chrono::duration<double> minDuration = std::chrono::duration<double>::max();

    for (int n = 0; n < count; ++n)
    {
        StopWatch sw;

        for (int i = 0; i < repeat; i++)
        {
            // Access strings interchangably to minimize or avoid caching or optimizations.
            if constexpr(std::is_same_v<Str, std::string> && std::is_same_v<Array, const std::array<boost::string_view, 6>>)
            {
                process.template ProcessString<Str>(Str(array[i % array.size()]));
            }
            else
            {
                process.template ProcessString<Str>(array[i % array.size()]);
            }
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

template <typename Str, typename Array>
std::pair<double, int> ReturnValueBechmarkStringArray(int count, const Array& array)
{
    // Result of dummy processing. Used also to check if all benchmarks provide the same result.
    int result = 127;

    Process process(array);

    std::chrono::duration<double> minDuration = std::chrono::duration<double>::max();

    for (int n = 0; n < count; ++n)
    {
        StopWatch sw;

        for (int i = 0; i < repeat; i++)
        {
            // Access strings interchangably to minimize or avoid cacheing or optimizations.
            Str str = process.template ReturnString<Str>(i);
            result %= str[0];
            result += 15;
        }
        if (const auto duration = sw.Stop(); duration < minDuration)
        {
            minDuration = duration;
        }
    }
    // Return both duration and the result of dummy operation to avoid compiler to optimize out corresponding operations.
    return { minDuration.count(), result };
}

void DoArgumentsBenchmark(int count)
{
    std::cout << "\n*** Different argument types:\n\n";

    auto BenchmarkConstStringRefOnStringArray = ArgumentBechmark<const std::string&, decltype(string_array)>;
    auto BenchmarkStringViewOnStringArray = ArgumentBechmark<std::string_view, decltype(string_array)>;
    auto BenchmarkConstStringViewRefOnStringArray = ArgumentBechmark<const std::string_view&, decltype(string_array)>;
    auto BenchmarkBoostStringViewOnStringArray = ArgumentBechmark<boost::string_view, decltype(string_array)>;
    
    auto BenchmarkStdStringViewOnStringViewArray = ArgumentBechmark<std::string, decltype(string_view_array)>;
    
    auto BenchmarkConstStdStringRefOnConstCharArray = ArgumentBechmark<const std::string&, decltype(const_char_array)>;
    auto BenchmarkStdStringOnConstCharArray = ArgumentBechmark<std::string, decltype(const_char_array)>;
    auto BenchmarkBoostStringViewOnConstCharArray = ArgumentBechmark<boost::string_view, decltype(const_char_array)>;

    double constStringRefOnStringArrayDuration = 0;
    double stringViewOnStringArrayDuration = 0;
    double constStringViewRefOnStringArrayDuration = 0;
    double boostStringViewOnStringArrayDuration = 0;

    double stringOnStringViewArrayDuration = 0;

    double constStringRefOnConstCharArrayDuration = 0;
    double stringOnConstCharArrayDuration = 0;
    double boostStringViewOnConstCharArrayDuration = 0;

    int resultConstStringRefOnStringArray = 127;
    int resultStringViewOnStringArray = 127;
    int resultConstStringViewRefOnStringArray = 127;
    int resultBoostStringViewOnStringArray = 127;

    int resultStringOnStringViewArray = 127;
    
    int resultConstStringRefOnConstCharArray = 127;
    int resultStringOnConstCharArray = 127;
    int resultBoostStringViewOnConstCharArray = 127;

    for (int i = 0; i < loops; ++i)
    {
        // Do benchmark for each parameter type. Result of dummy processing is included to avoid optimizing it out.
        {
            const auto [duration, ch] = BenchmarkConstStringRefOnStringArray(count, string_array);
            constStringRefOnStringArrayDuration += duration;
            resultConstStringRefOnStringArray %= ch;
            resultConstStringRefOnStringArray += 15;
        }
        {
            const auto [duration, ch] = BenchmarkStringViewOnStringArray(count, string_array);
            stringViewOnStringArrayDuration += duration;
            resultStringViewOnStringArray %= ch;
            resultStringViewOnStringArray += 15;
        }
        {
            const auto [duration, ch] = BenchmarkConstStringViewRefOnStringArray(count, string_array);
            constStringViewRefOnStringArrayDuration += duration;
            resultConstStringViewRefOnStringArray %= ch;
            resultConstStringViewRefOnStringArray += 15;
        }
        {
            auto [duration, ch] = BenchmarkBoostStringViewOnStringArray(count, string_array);
            boostStringViewOnStringArrayDuration += duration;
            resultBoostStringViewOnStringArray %= ch;
            resultBoostStringViewOnStringArray += 15;
        }
        {
            auto [duration, ch] = BenchmarkStdStringViewOnStringViewArray(count, string_view_array);
            stringOnStringViewArrayDuration += duration;
            resultStringOnStringViewArray %= ch;
            resultStringOnStringViewArray += 15;
        }
        {
            auto [duration, ch] = BenchmarkConstStdStringRefOnConstCharArray(count, const_char_array);
            constStringRefOnConstCharArrayDuration += duration;
            resultConstStringRefOnConstCharArray %= ch;
            resultConstStringRefOnConstCharArray += 15;
        }
        {
            auto [duration, ch] = BenchmarkStdStringOnConstCharArray(count, const_char_array);
            stringOnConstCharArrayDuration += duration;
            resultStringOnConstCharArray %= ch;
            resultStringOnConstCharArray += 15;
        }
        {
            auto [duration, ch] = BenchmarkBoostStringViewOnConstCharArray(count, const_char_array);
            boostStringViewOnConstCharArrayDuration += duration;
            resultBoostStringViewOnConstCharArray %= ch;
            resultBoostStringViewOnConstCharArray += 15;
        }
        // Repeat benchmark for each parameter type in reverse order
        {
            auto [duration, ch] = BenchmarkBoostStringViewOnConstCharArray(count, const_char_array);
            boostStringViewOnConstCharArrayDuration += duration;
            resultBoostStringViewOnConstCharArray %= ch;
            resultBoostStringViewOnConstCharArray += 15;
        }
        {
            auto [duration, ch] = BenchmarkStdStringOnConstCharArray(count, const_char_array);
            stringOnConstCharArrayDuration += duration;
            resultStringOnConstCharArray %= ch;
            resultStringOnConstCharArray += 15;
        }
        {
            auto [duration, ch] = BenchmarkConstStdStringRefOnConstCharArray(count, const_char_array);
            constStringRefOnConstCharArrayDuration += duration;
            resultConstStringRefOnConstCharArray %= ch;
            resultConstStringRefOnConstCharArray += 15;
        }
        {
            auto [duration, ch] = BenchmarkStdStringViewOnStringViewArray(count, string_view_array);
            stringOnStringViewArrayDuration += duration;
            resultStringOnStringViewArray %= ch;
            resultStringOnStringViewArray += 15;
        }
        {
            auto [duration, ch] = BenchmarkBoostStringViewOnStringArray(count, string_array);
            boostStringViewOnStringArrayDuration += duration;
            resultBoostStringViewOnStringArray %= ch;
            resultBoostStringViewOnStringArray += 15;
        }
        {
            const auto [duration, ch] = BenchmarkConstStringViewRefOnStringArray(count, string_array);
            constStringViewRefOnStringArrayDuration += duration;
            resultConstStringViewRefOnStringArray %= ch;
            resultConstStringViewRefOnStringArray += 15;
        }
        {
            const auto [duration, ch] = BenchmarkStringViewOnStringArray(count, string_array);
            stringViewOnStringArrayDuration += duration;
            resultStringViewOnStringArray %= ch;
            resultStringViewOnStringArray += 15;
        }
        {
            const auto [duration, ch] = BenchmarkConstStringRefOnStringArray(count, string_array);
            constStringRefOnStringArrayDuration += duration;
            resultConstStringRefOnStringArray %= ch;
            resultConstStringRefOnStringArray += 15;
        }
    }

    std::cout << "const std::string& on std::string array: " << constStringRefOnStringArrayDuration << "\n";
    std::cout << resultConstStringRefOnStringArray << '\n';
    std::cout << "std::string_view on std::string array: " << stringViewOnStringArrayDuration << "\n";
    std::cout << resultStringViewOnStringArray << '\n';
    std::cout << "const std::string_view& on std::string array: " << constStringViewRefOnStringArrayDuration << "\n";
    std::cout << resultConstStringViewRefOnStringArray << '\n';
    std::cout << "boost::string_view on std::string array: " << boostStringViewOnStringArrayDuration << "\n";
    std::cout << resultBoostStringViewOnStringArray << '\n';
    std::cout << "std::string on boost::string_view array: " << stringOnStringViewArrayDuration << "\n";
    std::cout << resultStringOnStringViewArray << '\n';
    std::cout << "const std::string& on const char* array: " << constStringRefOnConstCharArrayDuration << "\n";
    std::cout << resultConstStringRefOnConstCharArray << '\n';
    std::cout << "std::string on const char* array: " << stringOnConstCharArrayDuration << "\n";
    std::cout << resultStringOnConstCharArray << '\n';
    std::cout << "boost::string_view on const char* array: " << boostStringViewOnConstCharArrayDuration << "\n";
    std::cout << resultBoostStringViewOnConstCharArray << '\n';
}

void DoReturnValueBenchmark(int count)
{
    std::cout << "\n*** Different return value types:\n\n";

    auto BenchmarkConstStringRefStringArray = ReturnValueBechmarkStringArray<const std::string&, decltype(string_array)>;
    auto BenchmarkStringViewStringArray = ReturnValueBechmarkStringArray<std::string_view, decltype(string_array)>;
    auto BenchmarkBoostStringViewStringArray = ReturnValueBechmarkStringArray<boost::string_view, decltype(string_array)>;
    auto BenchmarkBoostStringViewStringViewArray = ReturnValueBechmarkStringArray<boost::string_view, decltype(string_view_array)>;
    auto BenchmarkConstBoostStringViewRefStringViewArray = ReturnValueBechmarkStringArray<const boost::string_view&, decltype(string_view_array)>;

    double constStringRefDuration = 0;
    double stringViewDuration = 0;
    double boostStringViewDuration = 0;

    double boostStringViewDuration2 = 0;
    double constBoostStringViewRefDuration = 0;

    int resultConstStringRef = 127;
    int resultStringView = 127;
    int resultBoostStringView = 127;

    int resultBoostStringView2 = 127;
    int resultConstBoostStringViewRef = 127;

    for (int i = 0; i < loops; ++i)
    {
        // Do benchmark for each parameter type. Result of dummy processing is included to avoid optimizing it out.
        {
            const auto [duration, ch] = BenchmarkConstStringRefStringArray(count, string_array);
            constStringRefDuration += duration;
            resultConstStringRef %= ch;
            resultConstStringRef += 15;
        }
        {
            const auto [duration, ch] = BenchmarkStringViewStringArray(count, string_array);
            stringViewDuration += duration;
            resultStringView %= ch;
            resultStringView += 15;
        }
        {
            auto [duration, ch] = BenchmarkBoostStringViewStringArray(count, string_array);
            boostStringViewDuration += duration;
            resultBoostStringView %= ch;
            resultBoostStringView += 15;
        }
        {
            auto [duration, ch] = BenchmarkBoostStringViewStringViewArray(count, string_view_array);
            boostStringViewDuration2 += duration;
            resultBoostStringView2 %= ch;
            resultBoostStringView2 += 15;
        }
        {
            auto [duration, ch] = BenchmarkConstBoostStringViewRefStringViewArray(count, string_view_array);
            constBoostStringViewRefDuration += duration;
            resultConstBoostStringViewRef %= ch;
            resultConstBoostStringViewRef += 15;
        }
        // Repeat benchmark for each parameter type in reverse order
        {
            auto [duration, ch] = BenchmarkConstBoostStringViewRefStringViewArray(count, string_view_array);
            constBoostStringViewRefDuration += duration;
            resultConstBoostStringViewRef %= ch;
            resultConstBoostStringViewRef += 15;
        }
        {
            auto [duration, ch] = BenchmarkBoostStringViewStringViewArray(count, string_view_array);
            boostStringViewDuration2 += duration;
            resultBoostStringView2 %= ch;
            resultBoostStringView2 += 15;
        }
        {
            auto [duration, ch] = BenchmarkBoostStringViewStringArray(count, string_array);
            boostStringViewDuration += duration;
            resultBoostStringView %= ch;
            resultBoostStringView += 15;
        }
        {
            const auto [duration, ch] = BenchmarkStringViewStringArray(count, string_array);
            stringViewDuration += duration;
            resultStringView %= ch;
            resultStringView += 15;
        }
        {
            const auto [duration, ch] = BenchmarkConstStringRefStringArray(count, string_array);
            constStringRefDuration += duration;
            resultConstStringRef %= ch;
            resultConstStringRef += 15;
        }
    }

    std::cout << "const std::string&: " << constStringRefDuration << "\n";
    std::cout << resultConstStringRef << '\n';
    std::cout << "std::string_view: " << stringViewDuration << "\n";
    std::cout << resultStringView << '\n';
    std::cout << "boost::string_view: " << boostStringViewDuration << "\n";
    std::cout << resultBoostStringView << '\n';
    std::cout << "boost::string_view2: " << boostStringViewDuration2 << "\n";
    std::cout << resultBoostStringView2 << '\n';
    std::cout << "const boost::string_view&: " << constBoostStringViewRefDuration << "\n";
    std::cout << resultConstBoostStringViewRef << '\n';
}

int main()
{
    const int count = 300;

    DoArgumentsBenchmark(count);

    DoReturnValueBenchmark(count);
}
