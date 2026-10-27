// Copyright (c) Mistfly Games. All Rights Reserved.

#pragma once

#include "PCG32.generated.h"

/**
 * PCG32 (XSH RR 64/32), see https://www.pcg-random.org.
 */
USTRUCT()
struct SEEDWORKRANDOM_API FPCG32
{
    GENERATED_BODY()

public:
    FPCG32()
    {
        SetSeed(0, 0);
    }

    explicit FPCG32(int32 seed, int32 stream = 0)
    {
        SetSeed(seed, stream);
    }

    int32 GetSeed() const { return Seed; }

    uint32 Next()   // pcg_setseq_64_xsh_rr_32_random_r
    {
        constexpr uint64 Multiplier = 6364136223846793005ULL;   // PCG_DEFAULT_MULTIPLIER_64

        // pcg_setseq_64_step_r
        const uint64 oldState = State;
        State = oldState * Multiplier + Increment;

        // pcg_output_xsh_rr_64_32
        const uint32 xorShifted = ((oldState >> 18u) ^ oldState) >> 27u;
        const uint32 rotation = oldState >> 59u;
        return (xorShifted >> rotation) | (xorShifted << ((-rotation) & 31));
    }

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
