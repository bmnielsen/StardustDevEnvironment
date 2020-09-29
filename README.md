# StardustDevEnvironment

Development environment for creating AIs for StarCraft: Brood War

## Introduction

This is the development environment used for developing [Stardust](https://github.com/bmnielsen/Stardust), stripped down to its bare components so it is suitable for use by other bots.

It uses the following main components:

- [BWAPI](https://github.com/bwapi/bwapi) as the interface between the bot and the game.
- [OpenBW](http://www.openbw.com/) as the underlying game engine for local development.
- [Steamhammer](http://satirist.org/ai/starcraft/steamhammer/) and [Locutus](https://github.com/bmnielsen/Locutus) as test opponents.
- CMake
- Googletest

## Build

The following platforms have been tested by the author:

- CLion on MacOS Mojave for OpenBW-based local development
- Visual Studio 2019 for building Windows binaries

The CMake configuration reflects this, as on the MSVC platform it is configured to build a binary against standard BWAPILIB instead of OpenBW. It should however be possible to build the OpenBW-based configuration on Windows by modifying the CMake configuration.

For Linux, you will probably need to change some paths to LLVM, but otherwise I expect no major changes are needed.

## Run

The environment comes with the following components:

- A demo AI module (based on BWAPI's ExampleAIModule)
- Instrumentation code from Stardust for logging and outputting data understandable by [CherryVis](https://torchcraft.github.io/TorchCraftAI/blog/2019/02/20/releasing-cherryvis.html)
- A simple test framework for running scenarios and games
- Maps from the SSCAIT, AIIDE and COG tournaments
- Steamhammer and Locutus as test opponents

In order to run the tests, you need to put the three Brood War MPQ files (STARDAT.MPQ, BROODAT.MPQ and patch_rt.mpq) into the same directory as the test binary.

Some example tests are provided: Steamhammer.cpp and Locutus.cpp contain some tests for running full games against those opponents, and RushDefense.cpp has an example test that sets up a specific scenario.

## CherryVis

The instrumentation is based around using [CherryVis](https://torchcraft.github.io/TorchCraftAI/blog/2019/02/20/releasing-cherryvis.html) to view replays.

The demo AI module has examples of using a heatmap to mark buildable tiles and outputting unit-specific logs. For more ideas of what can be done, check out CherryVis.cpp or the CherryVis project itself.

To run CherryVis, check out [cherryvis-docker](https://github.com/bmnielsen/cherryvis-docker).

## Changes to OpenBW

The OpenBW code has had a couple of notable modifications applied to help integrate it into the development environment:

- The BWAPI 4.4 latcom changes have been applied to OpenBW's BWAPI fork
- The CherryVis OpenBW patch has been applied to support unit creation by triggers
