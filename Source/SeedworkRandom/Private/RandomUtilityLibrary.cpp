// Copyright (c) Mistfly Games. All Rights Reserved.

#include "RandomUtilityLibrary.h"
#include "Algo/Accumulate.h"
#include "Algo/ForEach.h"
#include "SeedworkRandom.h"

void URandomUtilityLibrary::NormalizeProbabilities(TArray<double>& probabilities)
{
    checkf(false, TEXT("check it works correctly first"));
    double totalProb = Algo::Accumulate(probabilities, 0.0);

    if (totalProb <= 0.0)
    {
        return;
    }

    Algo::ForEach(probabilities, [totalProb](double& p) { p /= totalProb; });
}

int32 URandomUtilityLibrary::SharedIntegerInRange(int32 a, int32 b)
{
    return URandom::Shared()->IntegerInRange(a, b);
}

double URandomUtilityLibrary::SharedDouble()
{
    return URandom::Shared()->Double();
}
