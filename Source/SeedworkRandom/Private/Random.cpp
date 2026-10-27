// Copyright (c) Mistfly Games. All Rights Reserved.

#include "Random.h"
#include "Algo/Accumulate.h"
#include "Algo/ForEach.h"
#include <cmath>

// NOTE: The distributions are written here instead of std ones, because the std algorithms are implementation-defined,
// and the same seed would give different numbers with a different standard library.

int32 URandom::IntegerInRange(int32 a, int32 b)
{
    if (!ensure(a <= b))
    {
        Swap(a, b);
    }

    // [INT32_MIN, INT32_MAX] holds 2^32 values, so the range size needs 64 bits.
    const uint64 rangeSize = static_cast<uint64>(static_cast<int64>(b) - a) + 1;
    if (rangeSize > TNumericLimits<uint32>::Max())
    {
        return static_cast<int32>(Rng.Next());
    }

    // Lemire's method: the high half of draw * bound is in [0, bound). The low half is below 2^32 mod bound
    // exactly for the draws that make the result biased, so those draws are rejected.
    const uint32 bound = static_cast<uint32>(rangeSize);
    uint64 product = static_cast<uint64>(Rng.Next()) * bound;
    if (static_cast<uint32>(product) < bound)
    {
        const uint32 threshold = (0u - bound) % bound;
        while (static_cast<uint32>(product) < threshold)
        {
            product = static_cast<uint64>(Rng.Next()) * bound;
        }
    }

    return static_cast<int32>(a + static_cast<int64>(product >> 32));
}

double URandom::DoubleInRange(double a, double b)
{
    if (!ensure(a <= b))
    {
        Swap(a, b);
    }

    // The sum can round up to b, which the range excludes.
    const double result = a + (b - a) * Double();
    return result < b ? result : std::nextafter(b, a);
}

double URandom::Double()
{
    const uint64 high = Rng.Next();
    const uint64 low = Rng.Next();

    // 53 bits fill the double mantissa exactly, so the result never rounds up to 1.
    return static_cast<double>(((high << 32) | low) >> 11) * 0x1.0p-53;
}

double URandom::NormalDistribution(double mean, double stddev)
{
    // Marsaglia polar method. It makes two values from each accepted pair, but the second one is dropped,
    // so that the generator has no state other than the engine.
    double u;
    double v;
    double lengthSquared;
    do
    {
        u = Double() * 2.0 - 1.0;
        v = Double() * 2.0 - 1.0;
        lengthSquared = u * u + v * v;
    } while (lengthSquared >= 1.0 || lengthSquared == 0.0);

    return mean + stddev * u * FMath::Sqrt(-2.0 * FMath::Loge(lengthSquared) / lengthSquared);
}

double URandom::ExponentialDistribution(double lambda)
{
    // Inverse transform. 1 - Double() is in (0..1], so the log is finite.
    return -FMath::Loge(1.0 - Double()) / lambda;
}

bool URandom::BoolWithWeight(double weight)
{
    ensure(weight >= 0.0 && weight <= 1.0);
    return Double() < weight;
}

bool URandom::Bool()
{
    return (Rng.Next() >> 31) != 0;
}

FVector URandom::Vector()
{
    // Points outside the unit sphere are rejected, so that the directions are uniform and not denser toward the cube corners.
    FVector result;
    double lengthSquared;
    do
    {
        result.X = Double() * 2.0 - 1.0;
        result.Y = Double() * 2.0 - 1.0;
        result.Z = Double() * 2.0 - 1.0;
        lengthSquared = result.SizeSquared();
    } while (lengthSquared > 1.0 || lengthSquared < KINDA_SMALL_NUMBER);

    return result * (1.0 / FMath::Sqrt(lengthSquared));
}

FVector URandom::VectorInCone(const FVector& dir, double coneHalfAngleRad)
{
    const FVector axis = dir.GetSafeNormal();
    if (coneHalfAngleRad <= 0.0 || axis.IsZero())
    {
        return axis;
    }

    // The area of a spherical cap grows linearly with the cosine of its angle, so a uniform cosine
    // gives directions that are uniform over the cap.
    const double cosAngle = FMath::Lerp(1.0, FMath::Cos(coneHalfAngleRad), Double());
    const double sinAngle = FMath::Sqrt(1.0 - cosAngle * cosAngle);
    const double azimuth = UE_DOUBLE_TWO_PI * Double();

    FVector side;
    FVector up;
    axis.FindBestAxisVectors(side, up);

    return axis * cosAngle + (side * FMath::Cos(azimuth) + up * FMath::Sin(azimuth)) * sinAngle;
}

FRotator URandom::Rotator(bool bRoll)
{
    FRotator result;
    result.Yaw = Double() * 360.0;
    result.Pitch = Double() * 360.0;
    result.Roll = bRoll ? Double() * 360.0 : 0.0;
    return result;
}

FVector URandom::PointInBox(const FBox& box)
{
    const double x = DoubleInRange(box.Min.X, box.Max.X);
    const double y = DoubleInRange(box.Min.Y, box.Max.Y);
    const double z = DoubleInRange(box.Min.Z, box.Max.Z);
    return FVector(x, y, z);
}

FVector URandom::PointInBox(const FVector& center, const FVector& halfSize)
{
    return PointInBox(FBox(center - halfSize, center + halfSize));
}

void URandom::NormalizeProbabilities(TArray<double>& probabilities)
{
    double totalProb = Algo::Accumulate(probabilities, 0.0);

    if (totalProb <= 0.0)
    {
        return;
    }

    Algo::ForEach(probabilities, [totalProb](double& p) { p /= totalProb; });
}
