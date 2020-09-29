#include "BWTest.h"
#include "UAlbertaBotModule.h"

TEST(RushDefense, Steamhammer9PoolSpeed)
{
    BWTest test;
    test.map = Maps::GetOne("Python");
    test.randomSeed = 30841;
    test.opponentRace = BWAPI::Races::Zerg;
    test.opponentModule = []()
    {
        auto module = new UAlbertaBot::UAlbertaBotModule();
        Config::StardustTestStrategyName = "9PoolSpeed";
        return module;
    };
    test.frameLimit = 5000;
    test.expectWin = false;

    test.myInitialUnits = {
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Pylon, BWAPI::TilePosition(46, 119)),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Pylon, BWAPI::TilePosition(43, 109)),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Gateway, BWAPI::TilePosition(46, 121)),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Gateway, BWAPI::TilePosition(50, 121)),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Probe, BWAPI::Position(BWAPI::TilePosition(115, 37))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Probe, BWAPI::Position(BWAPI::TilePosition(40, 116))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Probe, BWAPI::Position(BWAPI::TilePosition(41, 116))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Probe, BWAPI::Position(BWAPI::TilePosition(40, 117))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Probe, BWAPI::Position(BWAPI::TilePosition(41, 117))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Probe, BWAPI::Position(BWAPI::TilePosition(40, 118))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Probe, BWAPI::Position(BWAPI::TilePosition(41, 118))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Probe, BWAPI::Position(BWAPI::TilePosition(40, 119))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Probe, BWAPI::Position(BWAPI::TilePosition(41, 119))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Probe, BWAPI::Position(BWAPI::TilePosition(40, 120))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Probe, BWAPI::Position(BWAPI::TilePosition(41, 120))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Probe, BWAPI::Position(BWAPI::TilePosition(40, 121))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Probe, BWAPI::Position(BWAPI::TilePosition(41, 121))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Protoss_Zealot, BWAPI::Position(BWAPI::TilePosition(60, 124))),
    };

    // Enemy zerglings
    test.opponentInitialUnits = {
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Spawning_Pool, BWAPI::Position(BWAPI::TilePosition(113, 40))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Drone, BWAPI::Position(BWAPI::TilePosition(121, 37))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Drone, BWAPI::Position(BWAPI::TilePosition(122, 37))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Drone, BWAPI::Position(BWAPI::TilePosition(121, 38))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Drone, BWAPI::Position(BWAPI::TilePosition(122, 38))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Drone, BWAPI::Position(BWAPI::TilePosition(121, 39))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Drone, BWAPI::Position(BWAPI::TilePosition(122, 39))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Drone, BWAPI::Position(BWAPI::TilePosition(121, 40))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Drone, BWAPI::Position(BWAPI::TilePosition(122, 40))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Drone, BWAPI::Position(BWAPI::TilePosition(121, 41))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Overlord, BWAPI::Position(BWAPI::TilePosition(43, 125))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Zergling, BWAPI::Position(BWAPI::TilePosition(120, 52))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Zergling, BWAPI::Position(BWAPI::TilePosition(119, 55))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Zergling, BWAPI::Position(BWAPI::TilePosition(71, 107))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Zergling, BWAPI::Position(BWAPI::TilePosition(72, 105))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Zergling, BWAPI::Position(BWAPI::TilePosition(72, 104))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Zergling, BWAPI::Position(BWAPI::TilePosition(74, 100))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Zergling, BWAPI::Position(BWAPI::TilePosition(100, 41))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Zergling, BWAPI::Position(BWAPI::TilePosition(101, 41))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Zergling, BWAPI::Position(BWAPI::TilePosition(101, 41))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Zergling, BWAPI::Position(BWAPI::TilePosition(123, 24))),
            UnitTypeAndPosition(BWAPI::UnitTypes::Zerg_Zergling, BWAPI::Position(BWAPI::TilePosition(124, 24))),
    };

    test.onEndMine = [](bool won)
    {
        // Verify that we still have some probes remaining
        bool hasAProbe = false;
        for (const auto &unit : BWAPI::Broodwar->self()->getUnits())
        {
            if (unit->getType().isWorker())
            {
                hasAProbe = true;
                break;
            }
        }

        EXPECT_TRUE(hasAProbe);
    };

    test.run();
}