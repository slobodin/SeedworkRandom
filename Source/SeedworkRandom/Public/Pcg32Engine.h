// Copyright (c) Mistfly Games. All Rights Reserved.

#pragma once

#include "Pcg32Engine.generated.h"

/**
 * PCG32 (XSH RR 64/32), see https://www.pcg-random.org.
 */
USTRUCT()
struct SEEDWORKRANDOM_API FPcg32Engine
{
    GENERATED_BODY()

public:
    FPcg32Engine()
    {
        SetSeed(0, 0);
    }

    explicit FPcg32Engine(int32 seed, int32 stream = 0)
    {
        SetSeed(seed, stream);
    }

    int32 GetSeed() const { return Seed; }

    uint32 Next();

private:
    UPROPERTY(SaveGame)
    int32 Seed = 0;

    UPROPERTY(SaveGame)
    uint64 State = 0;

    UPROPERTY(SaveGame)
    uint64 Increment = 0;

    void SetSeed(uint64 newSeed, uint64 stream)
    {
        Seed = newSeed;

        // pcg_setseq_64_srandom_r
        State = 0;
        Increment = (stream << 1u) | 1u;
        Next();
        State += newSeed;
        Next();
    }
};
