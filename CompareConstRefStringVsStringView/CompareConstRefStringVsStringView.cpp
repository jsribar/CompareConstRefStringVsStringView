// CompareConstRefStringVsStringView.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <iostream>
#include <string_view>

#include <boost/utility/string_view.hpp>

class StopWatch
{
public:
    StopWatch(std::ostream& os = std::cout) : m_os(os)
    {
        m_startPoint = std::chrono::high_resolution_clock::now();
    }

    std::chrono::duration<double> Stop()
    {
        auto endPoint = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::duration<double>>(endPoint - m_startPoint);
    }

    ~StopWatch()
    {
        auto endPoint = Stop();
        m_os << "Elapsed:" << endPoint.count() << std::endl;
    }

private:
    std::chrono::high_resolution_clock::time_point m_startPoint;
    std::ostream& m_os;
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

static constexpr int repeat{ 500 };

int main()
{
    std::string text{ "Hello world" };

    for (int n = 0; n < 10; ++n)
    {
        {
            std::cout << "const std::string&\n";

            Process process;

            StopWatch sw;

            for (int i = 0; i <= repeat; i++)
            {
                for (int j = 0; j <= repeat; j++)
                {
                    process.ConstRefString(text);
                }
            }
            sw.Stop();
            std::cout << (int)process.ch << '\n';
        }

        {
            std::cout << "\nstd::string_view\n";

            Process process;

            StopWatch sw;

            for (int i = 0; i <= repeat; i++)
            {
                for (int j = 0; j <= repeat; j++)
                {
                    process.StringView(text);
                }
            }
            sw.Stop();
            std::cout << (int)process.ch << '\n';
        }

        {
            std::cout << "\nboost::string_view\n";

            Process process;

            StopWatch sw;

            for (int i = 0; i <= repeat; i++)
            {
                for (int j = 0; j <= repeat; j++)
                {
                    process.BoostStringView(text);
                }
            }
            sw.Stop();
            std::cout << (int)process.ch << '\n';
        }

        std::cout << "\n***********************\n";
    }
}
