#pragma once

#include <BWAPI.h>

namespace Log
{
    class LogWrapper
    {
    protected:
        std::ostringstream *os;
        int *refCount;
        std::ofstream *logFile;
        bool outputToConsole;

    public:

        LogWrapper(std::ofstream *logFile, bool outputToConsole);

        LogWrapper(const LogWrapper &other);

        ~LogWrapper();

        template<typename T> LogWrapper &operator<<(T const &value)
        {
            if (logFile)
            {
                (*os) << value;
            }
            return *this;
        }

        LogWrapper &operator=(const LogWrapper &) = delete;
    };

    void initialize();

    void SetOutputToConsole(bool outputToConsole);

    LogWrapper Get();

    std::string &LogFileName();
}