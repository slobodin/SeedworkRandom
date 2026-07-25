// Copyright (c) Mistfly Games. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "UObject/Object.h"
#include <random>
#include "SeedworkRandom.generated.h"

UCLASS(BlueprintType)
class SEEDWORKRANDOM_API URandom : public UObject
{
    GENERATED_BODY()

public:
    void Reset();

    /** Random int in a closed interval[a..b](inclusive) */
    int32 IntegerInRange(int32 a, int32 b);

    /** Random int in a closed interval[a..b](inclusive) */
    int32 IntegerInRange(const FInt32Interval& range);

    /** Random double in range [a..b) */
    double DoubleInRange(double a, double b);

    /** Random double in range [a..b) */
    double DoubleInRange(const FVector2D& range);

    /** Random double in range [0..1) */
    double Double();

    // Values near the mean are the most likely. Standard deviation affects the dispersion of generated values from the mean.
    double NormalDistribution(double mean = 0.0, double stddev = 0.2);

    double ExponentialDistribution(double lambda = 1.0);

    bool BoolWithWeight(double weight);

    bool Bool();

    FVector Vector();

    // Copy of FMath::VRandCone but using mt engine to generate random numbers
    FVector VectorInCone(const FVector& dir, double coneHalfAngleRad);

    FRotator Rotator(bool bRoll = false);

    FVector PointInBox(const FBox& box);
    FVector PointInBox(const FVector& center, const FVector& halfSize);

    /** NOTE: This is not thread safe! */
    static URandom* Shared();

private:
    std::mt19937 MT;
    bool bInited = false;

    std::mt19937* GetRandomGenerator();
};

class FSeedworkRandomModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    TStrongObjectPtr<URandom> SharedRandom;

    friend class URandom;
};
