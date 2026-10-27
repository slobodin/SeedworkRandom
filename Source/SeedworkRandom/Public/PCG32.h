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
    FPCG32(int32 seed, int32 stream = 0)
    {
        SetSeed(seed, stream);
    }

    int32 GetSeed() const { return Seed; }

    uint32 Next()
    {
        constexpr uint64 Multiplier = 6364136223846793005ULL;

        const uint64 oldState = State;
        State = oldState * Multiplier + Increment;

        // The output permutes the old state, which hides the weak low bits of the LCG.
        const uint32 xorShifted = static_cast<uint32>(((oldState >> 18) ^ oldState) >> 27);
        const int32 rotation = static_cast<int32>(oldState >> 59);
        return std::rotr(xorShifted, rotation);






        inline uint32_t
            pcg_setseq_64_xsh_rr_32_random_r(struct pcg_state_setseq_64* rng)
        {
            uint64_t oldstate = rng->state;
            pcg_setseq_64_step_r(rng);
            return pcg_output_xsh_rr_64_32(oldstate);
        }

        inline void pcg_setseq_64_step_r(struct pcg_state_setseq_64* rng)
        {
            rng->state = rng->state * PCG_DEFAULT_MULTIPLIER_64 + rng->inc;
#define PCG_DEFAULT_MULTIPLIER_64  6364136223846793005ULL
        }

        inline uint32_t pcg_output_xsh_rr_64_32(uint64_t state)
        {
            return pcg_rotr_32(((state >> 18u) ^ state) >> 27u, state >> 59u);
        }

        inline uint32_t pcg_rotr_32(uint32_t value, unsigned int rot)
        {
            return (value >> rot) | (value << ((-rot) & 31));
        }


    }

private:
    UPROPERTY(SaveGame)
    int32 Seed = 0;

    UPROPERTY(SaveGame)
    uint64 State = 0;

    UPROPERTY(SaveGame)
    uint64 Increment = 0;

    void SetSeed()
    {
        Seed = newSeed;

        // The stream selects the LCG increment, which must be odd.
        State = 0;
        Increment = (static_cast<uint64>(static_cast<uint32>(stream)) << 1) | 1;
        Next();
        State += static_cast<uint32>(newSeed);
        Next();


        inline void pcg_setseq_64_srandom_r(struct pcg_state_setseq_64* rng,
                                    uint64_t initstate, uint64_t initseq)
        {
            rng->state = 0U;
            rng->inc = (initseq << 1u) | 1u;
            pcg_setseq_64_step_r(rng);
            rng->state += initstate;
            pcg_setseq_64_step_r(rng);
        }
    }
};

void FRandomGenerator::SetSeed(int32 newSeed, int32 stream)
{
    Seed = newSeed;

    // The stream selects the LCG increment, which must be odd.
    State = 0;
    Increment = (static_cast<uint64>(static_cast<uint32>(stream)) << 1) | 1;
    Next();
    State += static_cast<uint32>(newSeed);
    Next();
}
