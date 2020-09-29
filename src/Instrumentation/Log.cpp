#include "Log.h"

#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace Log
{
    namespace
    {
        bool isOutputtingToConsole = false;
        std::chrono::system_clock::time_point startTime;
        std::string logFileName;
        std::ofstream *log;
    }

    LogWrapper::LogWrapper(std::ofstream *logFile, bool outputToConsole)
            : os(new std::ostringstream)
            , refCount(new int(1))
            , logFile(logFile)
            , outputToConsole(outputToConsole)
    {
        if (!logFile) return;

        int seconds = BWAPI::Broodwar->getFrameCount() / 24;
        int minutes = seconds / 60;
        seconds = seconds % 60;
        (*os) << BWAPI::Broodwar->getFrameCount() << "(" << minutes << ":" << (seconds < 10 ? "0" : "") << seconds << "): ";
    }

    LogWrapper::LogWrapper(const LogWrapper &other)
            : os(other.os)
            , refCount(other.refCount)
            , logFile(other.logFile)
            , outputToConsole(other.outputToConsole)
    {
        ++*refCount;
    }

    LogWrapper::~LogWrapper()
    {
        --*refCount;

        if (*refCount == 0)
        {
            if (outputToConsole)
            {
                std::cout << os->str() << std::endl;
            }

            if (logFile)
            {
                (*os) << "\n";
                (*logFile) << os->str();
                logFile->flush();
            }

            delete os;
            delete refCount;
        }
    }

    void initialize()
    {
        startTime = std::chrono::system_clock::now();

        try
        {
            if (log)
            {
                log->close();
                log = nullptr;
            }
        }
        catch (std::exception &ex)
        {
            // Ignore
        }
    }

    void SetOutputToConsole(bool outputToConsole)
    {
        isOutputtingToConsole = outputToConsole;
    }

    LogWrapper Get()
    {
        if (!log)
        {
            std::ostringstream filename;
            filename << "bwapi-data/write/DemoAI_log";
            auto tt = std::chrono::system_clock::to_time_t(startTime);
            auto tm = std::localtime(&tt);
            filename << "_" << std::put_time(tm, "%Y%m%d_%H%M%S") << ".txt";
            logFileName = filename.str();

            log = new std::ofstream();
            log->open(logFileName, std::ofstream::trunc);
        }

        return LogWrapper(log, isOutputtingToConsole);
    }

    std::string &LogFileName()
    {
        return logFileName;
    }
}