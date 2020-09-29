#include "DemoAIModule.h"
#include <iostream>

#include "Log.h"
#include "CherryVis.h"

using namespace BWAPI;
using namespace Filter;

void DemoAIModule::onStart()
{
    // Initialize our instrumentation
    Log::initialize();
    CherryVis::initialize();

    Log::Get() << "Started game on " << Broodwar->mapName() << " against " << Broodwar->enemy()->getName();

    // Demo of a CherryVis heatmap
    std::vector<long> buildability(Broodwar->mapWidth() * Broodwar->mapHeight());
    for (int x = 0; x < Broodwar->mapWidth(); x++)
    {
        for (int y = 0; y < Broodwar->mapHeight(); y++)
        {
            buildability[x + y * Broodwar->mapWidth()] = Broodwar->isBuildable(x, y);
        }
    }

    CherryVis::addHeatmap("Buildable", buildability, Broodwar->mapWidth(), Broodwar->mapHeight());
}

void DemoAIModule::onEnd(bool isWinner)
{
    // Writes all of the CherryVis data files
    CherryVis::gameEnd();
}

void DemoAIModule::onFrame()
{
    // Called once every game frame

    // Return if the game is a replay or is paused
    if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
        return;

    // Iterate through all the units that we own
    for (auto &u : Broodwar->self()->getUnits())
    {
        // Ignore the unit if it no longer exists
        // Make sure to include this block when handling any Unit pointer!
        if (!u->exists())
            continue;

        // In order for CherryVis to link BWAPI units to OpenBW units, it needs to be notified whenever a new unit is first seen
        // This uses the unit's client info as a flag for whether we have registered it or not to avoid additional infrastructure
        if (u->isCompleted() && !u->getClientInfo())
        {
            CherryVis::unitFirstSeen(u);
            u->setClientInfo(1);
        }

        // Output some information about what this unit is doing to its CherryVis log every frame
        std::ostringstream debug;

        // First line is command
        debug << "cmd=" << u->getLastCommand().getType() << ";f="
              << (Broodwar->getFrameCount() - u->getLastCommandFrame());
        if (u->getLastCommand().getTarget())
        {
            debug << ";tgt=" << u->getLastCommand().getTarget()->getType()
                  << "#" << u->getLastCommand().getTarget()->getID()
                  << "@" << WalkPosition(u->getLastCommand().getTarget()->getPosition())
                  << ";d=" << u->getLastCommand().getTarget()->getDistance(u);
        }
        else if (u->getLastCommand().getTargetPosition())
        {
            debug << ";tgt=" << WalkPosition(u->getLastCommand().getTargetPosition());
        }

        // Next line is order
        debug << "\nord=" << u->getOrder() << ";t=" << u->getOrderTimer();
        if (u->getOrderTarget())
        {
            debug << ";tgt=" << u->getOrderTarget()->getType()
                  << "#" << u->getOrderTarget()->getID()
                  << "@" << WalkPosition(u->getOrderTarget()->getPosition())
                  << ";d=" << u->getOrderTarget()->getDistance(u);
        }
        else if (u->getOrderTargetPosition())
        {
            debug << ";tgt=" << WalkPosition(u->getOrderTargetPosition());
        }

        // Last line is movement data
        debug << "\n";
        if (u->getType().topSpeed() > 0.001)
        {
            auto speed = sqrt(u->getVelocityX() * u->getVelocityX()
                              + u->getVelocityY() * u->getVelocityY());
            debug << "spd=" << ((int) (100.0 * speed / u->getType().topSpeed()))
                  << ";mvng=" << u->isMoving()
                  << ";stk=" << u->isStuck();
        }

        CherryVis::log(u->getID()) << debug.str();

        // Ignore the unit if it has one of the following status ailments
        if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
            continue;

        // Ignore the unit if it is in one of the following states
        if (u->isLoaded() || !u->isPowered() || u->isStuck())
            continue;

        // Ignore the unit if it is incomplete or busy constructing
        if (!u->isCompleted() || u->isConstructing())
            continue;


        // Finally make the unit do some stuff!


        // If the unit is a worker unit
        if (u->getType().isWorker())
        {
            // if our worker is idle
            if (u->isIdle())
            {
                // Order workers carrying a resource to return them to the center,
                // otherwise find a mineral patch to harvest.
                if (u->isCarryingGas() || u->isCarryingMinerals())
                {
                    u->returnCargo();
                }
                else if (!u->getPowerUp())  // The worker cannot harvest anything if it
                {                             // is carrying a powerup such as a flag
                    // Harvest from the nearest mineral patch or gas refinery
                    if (!u->gather(u->getClosestUnit(IsMineralField || IsRefinery)))
                    {
                        // If the call fails, then print the last error message
                        Broodwar << Broodwar->getLastError() << std::endl;
                    }

                } // closure: has no powerup
            } // closure: if idle

        }
        else if (u->getType().isResourceDepot()) // A resource depot is a Command Center, Nexus, or Hatchery
        {

            // Order the depot to construct more workers! But only when it is idle.
            if (u->isIdle() && !u->train(u->getType().getRace().getWorker()))
            {
                // If that fails, draw the error at the location so that you can visibly see what went wrong!
                // However, drawing the error once will only appear for a single frame
                // so create an event that keeps it on the screen for some frames
                Position pos = u->getPosition();
                Error lastErr = Broodwar->getLastError();
                Broodwar->registerEvent([pos, lastErr](Game *)
                                        { Broodwar->drawTextMap(pos, "%c%s", Text::White, lastErr.c_str()); },   // action
                                        nullptr,    // condition
                                        Broodwar->getLatencyFrames());  // frames to run

                // Retrieve the supply provider type in the case that we have run out of supplies
                UnitType supplyProviderType = u->getType().getRace().getSupplyProvider();
                static int lastChecked = 0;

                // If we are supply blocked and haven't tried constructing more recently
                if (lastErr == Errors::Insufficient_Supply &&
                    lastChecked + 400 < Broodwar->getFrameCount() &&
                    Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0)
                {
                    lastChecked = Broodwar->getFrameCount();

                    // Retrieve a unit that is capable of constructing the supply needed
                    Unit supplyBuilder = u->getClosestUnit(GetType == supplyProviderType.whatBuilds().first &&
                                                           (IsIdle || IsGatheringMinerals) &&
                                                           IsOwned);
                    // If a unit was found
                    if (supplyBuilder)
                    {
                        if (supplyProviderType.isBuilding())
                        {
                            TilePosition targetBuildLocation = Broodwar->getBuildLocation(supplyProviderType, supplyBuilder->getTilePosition());
                            if (targetBuildLocation)
                            {
                                // Register an event that draws the target build location
                                Broodwar->registerEvent([targetBuildLocation, supplyProviderType](Game *)
                                                        {
                                                            Broodwar->drawBoxMap(Position(targetBuildLocation),
                                                                                 Position(targetBuildLocation + supplyProviderType.tileSize()),
                                                                                 Colors::Blue);
                                                        },
                                                        nullptr,  // condition
                                                        supplyProviderType.buildTime() + 100);  // frames to run

                                // Order the builder to construct the supply structure
                                supplyBuilder->build(supplyProviderType, targetBuildLocation);
                            }
                        }
                        else
                        {
                            // Train the supply provider (Overlord) if the provider is not a structure
                            supplyBuilder->train(supplyProviderType);
                        }
                    } // closure: supplyBuilder is valid
                } // closure: insufficient supply
            } // closure: failed to train idle unit

        }

    } // closure: unit iterator

    CherryVis::frameEnd(Broodwar->getFrameCount());
}

void DemoAIModule::onSendText(std::string text)
{

    // Send the text to the game if it is not being processed.
    Broodwar->sendText("%s", text.c_str());


    // Make sure to use %s and pass the text as a parameter,
    // otherwise you may run into problems when you use the %(percent) character!

}

void DemoAIModule::onReceiveText(BWAPI::Player player, std::string text)
{
    // Parse the received text
    Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void DemoAIModule::onPlayerLeft(BWAPI::Player player)
{
    // Interact verbally with the other players in the game by
    // announcing that the other player has left.
    Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void DemoAIModule::onNukeDetect(BWAPI::Position target)
{

    // Check if the target is a valid position
    if (target)
    {
        // if so, print the location of the nuclear strike target
        Broodwar << "Nuclear Launch Detected at " << target << std::endl;
    }
    else
    {
        // Otherwise, ask other players where the nuke is!
        Broodwar->sendText("Where's the nuke?");
    }

    // You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void DemoAIModule::onUnitDiscover(BWAPI::Unit unit)
{
}

void DemoAIModule::onUnitEvade(BWAPI::Unit unit)
{
}

void DemoAIModule::onUnitShow(BWAPI::Unit unit)
{
}

void DemoAIModule::onUnitHide(BWAPI::Unit unit)
{
}

void DemoAIModule::onUnitCreate(BWAPI::Unit unit)
{
    if (unit->getPlayer() == BWAPI::Broodwar->self())
    {
        Log::Get() << "Unit created: " << unit->getType();
    }
}

void DemoAIModule::onUnitDestroy(BWAPI::Unit unit)
{
    if (unit->getPlayer() == BWAPI::Broodwar->self())
    {
        Log::Get() << "Unit lost: " << unit->getType();
    }
}

void DemoAIModule::onUnitMorph(BWAPI::Unit unit)
{
    if (Broodwar->isReplay())
    {
        // if we are in a replay, then we will print out the build order of the structures
        if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
        {
            int seconds = Broodwar->getFrameCount() / 24;
            int minutes = seconds / 60;
            seconds %= 60;
            Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
        }
    }
}

void DemoAIModule::onUnitRenegade(BWAPI::Unit unit)
{
}

void DemoAIModule::onSaveGame(std::string gameName)
{
    Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void DemoAIModule::onUnitComplete(BWAPI::Unit unit)
{
}