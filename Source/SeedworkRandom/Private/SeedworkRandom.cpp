// Copyright (c) Mistfly Games. All Rights Reserved.

#include "SeedworkRandom.h"
#include <chrono>

void URandom::SetSeed(int32 newSeed)
{
    Seed = newSeed;
    bInited = false;
}

int32 URandom::IntegerInRange(int32 a, int32 b)
{
    if (a == b)
    {
        return a;
    }

    ensure(a < b);

    std::uniform_int_distribution<int32> distr{ a, b };

    return distr(GetRandomGenerator());
}

int32 URandom::IntegerInRange(const FInt32Interval& range)
{
    return IntegerInRange(range.Min, range.Max);
}

double URandom::DoubleInRange(double a, double b)
{
    if (FMath::IsNearlyEqual(a, b, KINDA_SMALL_NUMBER))
    {
        return a;
    }

    ensure(a < b);

    std::uniform_real_distribution<double> distr{ a, b };

    return distr(GetRandomGenerator());
}

double URandom::DoubleInRange(const FVector2D& range)
{
    return DoubleInRange(range.X, range.Y);
}

double URandom::Double()
{
    return DoubleInRange(0.0, 1.0);
}

double URandom::NormalDistribution(double mean, double stddev)
{
    std::normal_distribution<double> distr{ mean, stddev };

    return distr(GetRandomGenerator());
}

double URandom::ExponentialDistribution(double lambda)
{
    std::exponential_distribution<double> distr{ lambda };

    return distr(GetRandomGenerator());
}

bool URandom::BoolWithWeight(double weight)
{
    ensure(weight >= 0.0 && weight <= 1.0);

    // If the Weight equals to 0.0 then always return false
    if (weight <= 0.0)
    {
        return false;
    }
    else
    {
        // If the Weight is higher or equal to the random number then return true
        return weight >= DoubleInRange(0.0, 1.0);
    }
}

bool URandom::Bool()
{
    return IntegerInRange(0, 1) == 1;
}

FVector URandom::Vector()
{
    FVector Result;
    FVector::FReal L;

    do
    {
        // Check random vectors in the unit sphere so result is statistically uniform.
        Result.X = Double() * 2.0 - 1.0;
        Result.Y = Double() * 2.0 - 1.0;
        Result.Z = Double() * 2.0 - 1.0;
        L = Result.SizeSquared();
    } while (L > 1.0 || L < KINDA_SMALL_NUMBER);

    return Result * (1.0 / FMath::Sqrt(L));
}

FVector URandom::VectorInCone(const FVector& dir, double coneHalfAngleRad)
{
    if (coneHalfAngleRad > 0.0)
    {
        double const RandU = Double();
        double const RandV = Double();

        // Get spherical coords that have an even distribution over the unit sphere
        // Method described at http://mathworld.wolfram.com/SpherePointPicking.html
        double Theta = 2.0 * PI * RandU;
        double Phi = FMath::Acos((2.0 * RandV) - 1.0);

        // restrict phi to [0, ConeHalfAngleRad]
        // this gives an even distribution of points on the surface of the cone
        // centered at the origin, pointing upward (z), with the desired angle
        Phi = FMath::Fmod(Phi, coneHalfAngleRad);

        // get axes we need to rotate around
        FMatrix const DirMat = FRotationMatrix(dir.Rotation());
        // note the axis translation, since we want the variation to be around X
        FVector const DirZ = DirMat.GetScaledAxis(EAxis::X);
        FVector const DirY = DirMat.GetScaledAxis(EAxis::Y);

        FVector Result = dir.RotateAngleAxis(Phi * 180.0 / PI, DirY);
        Result = Result.RotateAngleAxis(Theta * 180.0 / PI, DirZ);

        // ensure it's a unit vector (might not have been passed in that way)
        Result = Result.GetSafeNormal();

        return Result;
    }
    else
    {
        return dir.GetSafeNormal();
    }
}

FRotator URandom::Rotator(bool bRoll)
{
    FRotator RRot;
    RRot.Yaw = Double() * 360.0;
    RRot.Pitch = Double() * 360.0;
    if (bRoll)
    {
        RRot.Roll = Double() * 360.0;
    }
    else
    {
        RRot.Roll = 0;
    }
    return RRot;
}

FVector URandom::PointInBox(const FBox& box)
{
    return FVector(DoubleInRange(box.Min.X, box.Max.X),
        DoubleInRange(box.Min.Y, box.Max.Y),
        DoubleInRange(box.Min.Z, box.Max.Z));
}

FVector URandom::PointInBox(const FVector& center, const FVector& halfSize)
{
    const FVector BoxMin = center - halfSize;
    const FVector BoxMax = center + halfSize;

    return PointInBox(FBox(BoxMin, BoxMax));
}

int32 URandom::GenerateAutoSeed()
{
    std::random_device rd;
    const uint32 autoSeed =
        rd() ^ static_cast<uint32>(std::chrono::steady_clock::now().time_since_epoch().count());
    return static_cast<int32>(autoSeed);
}

URandom* URandom::Shared()
{
    static FName Name = "SeedworkRandom";
    FSeedworkRandomModule& randomModule = FModuleManager::GetModuleChecked<FSeedworkRandomModule>(Name);

    check(randomModule.SharedRandom.IsValid());

    return randomModule.SharedRandom.Get();
}

std::mt19937& URandom::GetRandomGenerator()
{
    if (LIKELY(bInited))
    {
        return MT;
    }

    std::seed_seq ss{ static_cast<uint32>(Seed) };
    MT = std::mt19937{ ss };

    // Warm up the generator to move past mt19937's weak initial state.
    for (int32 i = 0; i < 128; i++)
    {
        static_cast<void>(MT());
    }

    bInited = true;
    return MT;
}

void FSeedworkRandomModule::StartupModule()
{
    SharedRandom = TStrongObjectPtr(NewObject<URandom>());
    SharedRandom->SetSeed(URandom::GenerateAutoSeed());
}

void FSeedworkRandomModule::ShutdownModule()
{
    SharedRandom.Reset();
}

IMPLEMENT_MODULE(FSeedworkRandomModule, SeedworkRandom)
