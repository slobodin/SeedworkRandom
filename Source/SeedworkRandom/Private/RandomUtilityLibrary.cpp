// Copyright (c) Mistfly Games. All Rights Reserved.

#include "RandomUtilityLibrary.h"
#include "Algo/Accumulate.h"
#include "Algo/ForEach.h"
#include "SeedworkRandom.h"

URandom* URandomUtilityLibrary::GetSharedRandom()
{
    auto result = URandom::Shared();
    check(IsValid(result));
    return result;
}

void URandomUtilityLibrary::NormalizeProbabilities(TArray<double>& probabilities)
{
    checkf(false, TEXT("check it works correctly first"));
    double totalProb = Algo::Accumulate(probabilities, 0.0);
    Algo::ForEach(probabilities, [totalProb](double& p) { p /= totalProb; });
}

int32 URandomUtilityLibrary::SharedIntegerInRange(int32 a, int32 b)
{
    return GetSharedRandom()->IntegerInRange(a, b);
}

double URandomUtilityLibrary::SharedDouble()
{
    return GetSharedRandom()->Double();
}
