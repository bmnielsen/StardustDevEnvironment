#include "BWTest.h"
#include "UAlbertaBotModule.h"

TEST(Steamhammer, RunTwenty)
{
    int count = 0;
    int lost = 0;
    while (count < 20)
    {
        BWTest test;
        test.opponentRace = BWAPI::Races::Zerg;
        test.maps = Maps::Get("aiide");
        test.opponentModule = []()
        {
            return new UAlbertaBot::UAlbertaBotModule();
        };
        test.onStartOpponent = []()
        {
            std::cout << "Steamhammer strategy: " << Config::Strategy::StrategyName << std::endl;

            std::cout.setstate(std::ios_base::failbit);
            std::cerr.setstate(std::ios_base::failbit);
        };
        test.onEndMine = [&](bool won)
        {
            std::ostringstream replayName;
            replayName << "Steamhammer_" << test.map->shortname();
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

TEST(Steamhammer, 4PoolHard)
{
    BWTest test;
    test.opponentRace = BWAPI::Races::Zerg;
    test.opponentModule = []()
    {
        auto module = new UAlbertaBot::UAlbertaBotModule();
        Config::StardustTestStrategyName = "4PoolHard";
        return module;
    };
    test.run();
}
