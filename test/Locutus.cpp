#include "BWTest.h"
#include "LocutusBotModule.h"

TEST(Locutus, RunOne)
{
    BWTest test;
    test.maps = Maps::Get("aiide");
    test.opponentRace = BWAPI::Races::Protoss;
    test.opponentModule = []()
    {
        return new Locutus::LocutusBotModule();
    };
    test.onStartOpponent = []()
    {
        std::cout << "Locutus strategy: " << Locutus::LocutusBotModule::getStrategyName() << std::endl;

        std::cout.setstate(std::ios_base::failbit);
    };
    test.onEndMine = [&](bool won)
    {
        std::ostringstream replayName;
        replayName << "Locutus_" << test.map->shortname();
        if (!won)
        {
            replayName << "_LOSS";
        }
        replayName << "_" << test.randomSeed;
        test.replayName = replayName.str();
    };
    test.run();
}

TEST(Locutus, RunTwenty)
{
    int count = 0;
    int lost = 0;
    while (count < 20)
    {
        BWTest test;
        test.maps = Maps::Get("aiide");
        test.opponentRace = BWAPI::Races::Protoss;
        test.opponentModule = []()
        {
            return new Locutus::LocutusBotModule();
        };
        test.onStartOpponent = []()
        {
            std::cout << "Locutus strategy: " << Locutus::LocutusBotModule::getStrategyName() << std::endl;

            std::cout.setstate(std::ios_base::failbit);
        };
        test.onEndMine = [&](bool won)
        {
            std::ostringstream replayName;
            replayName << "Locutus_" << test.map->shortname();
            if (!won)
            {
                replayName << "_LOSS";
                lost++;
            }
            replayName << "_" << test.randomSeed;
            test.replayName = replayName.str();

            count++;
            std::cout << "---------------------------------------------" << std::endl;
            std::cout << "STATUS AFTER " << count << " GAME" << (count == 1 ? "" : "S") << ": "
                      << (count - lost) << " won; " << lost << " lost" << std::endl;
            std::cout << "---------------------------------------------" << std::endl;
        };
        test.expectWin = false;
        test.run();
    }
}

TEST(Locutus, 4GateGoon)
{
    BWTest test;
    test.opponentRace = BWAPI::Races::Protoss;
    test.opponentModule = []()
    {
        Locutus::LocutusBotModule::setStrategy("4GateGoon");
        return new Locutus::LocutusBotModule();
    };

    test.run();
}

TEST(Locutus, GasSteal4GateGoon)
{
    BWTest test;
    test.opponentRace = BWAPI::Races::Protoss;
    test.opponentModule = []()
    {
        Locutus::LocutusBotModule::setStrategy("4GateGoon");
        Locutus::LocutusBotModule::forceGasSteal();
        return new Locutus::LocutusBotModule();
    };

    test.run();
}
