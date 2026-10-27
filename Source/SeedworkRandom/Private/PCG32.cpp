// Copyright (c) Mistfly Games. All Rights Reserved.

#include "PCG32.h"
#include <bit>

uint32 FPCG32::Next()  // pcg_setseq_64_xsh_rr_32_random_r
{
    constexpr uint64 Multiplier = 6364136223846793005ULL;   // PCG_DEFAULT_MULTIPLIER_64

    // pcg_setseq_64_step_r
    const uint64 oldState = State;
    State = oldState * Multiplier + Increment;

    // pcg_output_xsh_rr_64_32
    const uint32 xorShifted = ((oldState >> 18u) ^ oldState) >> 27u;
    const uint32 rotation = oldState >> 59u;
    return std::rotr(xorShifted, rotation);
}
