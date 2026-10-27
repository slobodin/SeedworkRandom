// Copyright (c) Mistfly Games. All Rights Reserved.

#include "SeedworkRandom.h"
#include "Misc/Guid.h"
#include "Random.h"

namespace
{
    URandom* GSharedRandom = nullptr;
}

URandom* URandom::K2_Shared()
{
    checkSlow(GSharedRandom);
    return GSharedRandom;
}

URandom& URandom::Shared()
{
    checkSlow(GSharedRandom);
    return *GSharedRandom;
}

void FSeedworkRandomModule::StartupModule()
{
    auto GenerateAutoSeed = []() -> int32
    {
        // Use a combination of OS randomness and the current time to generate a seed.
        uint32 timeSeed = FPlatformTime::Cycles();
        uint32 guidHash = GetTypeHash(FGuid::NewGuid());
        return static_cast<int32>(timeSeed ^ guidHash);
    };

    auto rng = URandom::MakePCG32(GenerateAutoSeed());
    SharedRandom = TStrongObjectPtr(rng);
    GSharedRandom = SharedRandom.Get();
}

void FSeedworkRandomModule::ShutdownModule()
{
    GSharedRandom = nullptr;
    SharedRandom.Reset();
}

IMPLEMENT_MODULE(FSeedworkRandomModule, SeedworkRandom)
