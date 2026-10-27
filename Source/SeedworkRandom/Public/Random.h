// Copyright (c) Mistfly Games. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "Math/Interval.h"
#include "Pcg32Engine.h"
#include "Random.generated.h"

/**
 * Wrapper over PCG-XSH-RR 64/32 random generator. See Pcg32Engine.h
 */
UCLASS(BlueprintType)
class SEEDWORKRANDOM_API URandom : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    int32 GetSeed() const { return Rng.GetSeed(); }

    /** Random int in a closed interval [a..b] (uniform) */
    UFUNCTION(BlueprintCallable)
    int32 IntegerInRange(int32 a, int32 b);

    /** Random int in a closed interval [a..b] (uniform) */
    int32 IntegerInRange(const FInt32Interval& range)
    {
        return IntegerInRange(range.Min, range.Max);
    }

    /** Random double in range [a..b) (uniform) */
    UFUNCTION(BlueprintCallable)
    double DoubleInRange(double a, double b);

    /** Random double in range [0..1) (uniform) */
    UFUNCTION(BlueprintCallable)
    double Double();

    /** Values near the mean are the most likely. Standard deviation affects the dispersion of generated values from the mean. */
    UFUNCTION(BlueprintCallable)
    double NormalDistribution(double mean = 0.0, double stddev = 0.2);

    UFUNCTION(BlueprintCallable)
    double ExponentialDistribution(double lambda = 1.0);

    /** Returns true with the probability weight, which is in [0..1]. */
    UFUNCTION(BlueprintCallable)
    bool BoolWithWeight(double weight);

    UFUNCTION(BlueprintCallable)
    bool Bool();

    /** Random unit vector. */
    UFUNCTION(BlueprintCallable)
    FVector Vector();

    /** Random unit vector, uniform over the cone around dir. */
    UFUNCTION(BlueprintCallable)
    FVector VectorInCone(const FVector& dir, double coneHalfAngleRad);

    UFUNCTION(BlueprintCallable)
    FRotator Rotator(bool bRoll = false);

    UFUNCTION(BlueprintCallable)
    FVector PointInBox(const FBox& box);

    FVector PointInBox(const FVector& center, const FVector& halfSize);

    template<typename T>
    T ArrayElement(const TArray<T>& arr)
    {
        if (arr.Num() > 0)
        {
            int32 idx = IntegerInRange(0, arr.Num() - 1);
            return arr[idx];
        }

        return {};
    }

    /**
     * Picks an element with a chance proportional to its probability. The probabilities don't need to sum to 1.
     * Elements with a probability of 0 or less are never picked. If no probability is positive, returns a default value,
     * the same as ArrayElement for an empty array.
     */
    template<typename T>
    T ArrayElementWithProbability(const TArray<T>& targetArray, const TArray<double>& probabilities)
    {
        if (!ensure(targetArray.Num() == probabilities.Num()))
        {
            return {};
        }

        double total = 0.0;
        for (double probability : probabilities)
        {
            if (probability > 0.0)
            {
                total += probability;
            }
        }

        if (total <= 0.0)
        {
            return {};
        }

        // Rounding can leave the remainder slightly above 0 after the last subtraction,
        // so the last element with a positive probability is the fallback.
        double remainder = Double() * total;
        int32 pickedIndex = INDEX_NONE;
        for (int32 i = 0; i < probabilities.Num(); i++)
        {
            if (probabilities[i] > 0.0)
            {
                pickedIndex = i;
                remainder -= probabilities[i];
                if (remainder < 0.0)
                {
                    break;
                }
            }
        }

        return targetArray[pickedIndex];
    }

    template<typename T>
    void ShuffleArray(TArray<T>& input)
    {
        for (int32 idx = input.Num() - 1; idx > 0; idx--)
        {
            int32 replaceIdx = IntegerInRange(0, idx);
            input.Swap(replaceIdx, idx);
        }
    }

    static void NormalizeProbabilities(TArray<double>& probabilities);

    /** NOTE: This is not thread safe! */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "GetSharedRandom", ScriptName = "GetSharedRandom"))
    static URandom* K2_Shared();

    static URandom& Shared();

    static URandom* Make(int32 seed, int32 stream = 0)
    {
        URandom* random = NewObject<URandom>();
        random->Rng = FPcg32Engine(seed, stream);
        return random;
    }

private:
    UPROPERTY(SaveGame)
    FPcg32Engine Rng;
};
