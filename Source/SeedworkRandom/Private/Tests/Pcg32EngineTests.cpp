// Copyright (c) Mistfly Games. All Rights Reserved.

#include "Pcg32Engine.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSeedworkPcg32EngineTest, "SeedworkRandom.Pcg32Engine",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FSeedworkPcg32EngineTest::RunTest(const FString&)
{
    struct FFixture
    {
        uint64 Seed;
        uint64 Stream;
        uint32 Draws[6];
    };

    // The first case is the PCG reference sequence. The other cases exercise bits above bit 31.
    constexpr FFixture fixtures[] =
    {
        {42ull, 54ull, {0xa15c02b7u, 0x7b47f409u, 0xba1d3330u, 0x83d2f293u, 0xbfa4784bu, 0xcbed606eu}},
        {0x100000001ull, 0ull, {0x4e563271u, 0x7aa0d9f2u, 0xfba85d0du, 0x756619f9u, 0x1c306ab4u, 0x75d3a4ceu}},
        {0xfedcba9876543210ull, 0x123456789abcdef0ull,
            {0x1e824c09u, 0x5ef51f62u, 0x835cb43eu, 0x26e24320u, 0xdd8e760eu, 0xe3226073u}},
    };

    int32 fixtureIndex = 0;
    for (const FFixture& fixture : fixtures)
    {
        FPcg32Engine engine(fixture.Seed, fixture.Stream);
        TestEqual(FString::Printf(TEXT("Seed readback for fixture %d"), fixtureIndex), engine.GetSeed(), fixture.Seed);

        int32 drawIndex = 0;
        for (uint32 expectedDraw : fixture.Draws)
        {
            TestEqual(FString::Printf(TEXT("Fixture %d draw %d"), fixtureIndex, drawIndex),
                engine.Next(), expectedDraw);
            ++drawIndex;
        }
        ++fixtureIndex;
    }

    return true;
}

#endif
