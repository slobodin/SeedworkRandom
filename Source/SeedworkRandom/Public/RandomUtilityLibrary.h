// Copyright (c) Mistfly Games. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RandomUtilityLibrary.generated.h"

class URandom;

UCLASS()
class SEEDWORKRANDOM_API URandomUtilityLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    static void NormalizeProbabilities(TArray<double>& probabilities);

    template<typename T>
    static T ArrayElement(const TArray<T>& arr)
    {
        checkf(false, TEXT("check it works correctly first"));
        if (arr.Num() > 0)
        {
            int32 idx = SharedIntegerInRange(0, arr.Num() - 1);
            return arr[idx];
        }

        return {};
    }

    template<typename T>
    static T ArrayElementWithProbability(const TArray<T>& targetArray, TArray<double> probabilities)
    {
        checkf(false, TEXT("check it works correctly first"));
        if (targetArray.Num() == probabilities.Num())
        {
            NormalizeProbabilities(probabilities);

            // now pick an item
            double x = SharedDouble();
            double cumulativeProb = 0.0;

            for (int32 i = 0; i < targetArray.Num(); i++)
            {
                cumulativeProb += probabilities[i];

                if (x <= cumulativeProb)
                {
                    return targetArray[i];
                }
            }
        }

        // should not be here.
        checkNoEntry();

        return ArrayElement(targetArray);
    }

    template<typename T>
    static void ShuffleArray(TArray<T>& input)
    {
        checkf(false, TEXT("check it works correctly first"));
        for (int32 idx = input.Num() - 1; idx > 0; idx--)
        {
            int32 replaceIdx = SharedIntegerInRange(0, idx);
            input.Swap(replaceIdx, idx);
        }
    }

private:
    static int32 SharedIntegerInRange(int32 a, int32 b);
    static double SharedDouble();
};
