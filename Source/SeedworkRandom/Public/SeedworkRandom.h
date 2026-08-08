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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
    int32 Seed = 0;

    UFUNCTION(BlueprintCallable)
    void SetSeed(int32 newSeed);

    /** Random int in a closed interval[a..b](inclusive) */
    UFUNCTION(BlueprintCallable)
    int32 IntegerInRange(int32 a, int32 b);

    /** Random int in a closed interval[a..b](inclusive) */
    int32 IntegerInRange(const FInt32Interval& range);

    /** Random double in range [a..b) */
    UFUNCTION(BlueprintCallable)
    double DoubleInRange(double a, double b);

    /** Random double in range [a..b) */
    double DoubleInRange(const FVector2D& range);

    /** Random double in range [0..1) */
    UFUNCTION(BlueprintCallable)
    double Double();

    // Values near the mean are the most likely. Standard deviation affects the dispersion of generated values from the mean.
    UFUNCTION(BlueprintCallable)
    double NormalDistribution(double mean = 0.0, double stddev = 0.2);

    UFUNCTION(BlueprintCallable)
    double ExponentialDistribution(double lambda = 1.0);

    UFUNCTION(BlueprintCallable)
    bool BoolWithWeight(double weight);

    UFUNCTION(BlueprintCallable)
    bool Bool();

    UFUNCTION(BlueprintCallable)
    FVector Vector();

    // Copy of FMath::VRandCone but using mt engine to generate random numbers
    UFUNCTION(BlueprintCallable)
    FVector VectorInCone(const FVector& dir, double coneHalfAngleRad);
    
    UFUNCTION(BlueprintCallable)
    FRotator Rotator(bool bRoll = false);

    UFUNCTION(BlueprintCallable)
    FVector PointInBox(const FBox& box);

    FVector PointInBox(const FVector& center, const FVector& halfSize);

    static int32 GenerateAutoSeed();

    /** NOTE: This is not thread safe! */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "GetSharedRandom", ScriptName = "GetSharedRandom"))
    static URandom* K2_Shared();

    static URandom& Shared();

private:
    std::mt19937 MT;
    bool bInited = false;

    std::mt19937& GetRandomGenerator();
};

class FSeedworkRandomModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    virtual bool SupportsDynamicReloading() override
    {
        return false;
    }

private:
    TStrongObjectPtr<URandom> SharedRandom;
};
