// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/GatchaRNG.h"
#include "Misc/DateTime.h"
#include "HAL/PlatformTime.h"
#include "Misc/Guid.h"

namespace
{
	static FRandomStream GRng;
	static bool bInited = false;

	static uint32 MakeAutoSeed()
	{
		uint64 s = (uint64)FDateTime::UtcNow().GetTicks();
		s ^= (uint64)FPlatformTime::Cycles64();
		const FGuid G = FGuid::NewGuid();
		// GUID 128비트 중 일부 섞기
		s ^= ((uint64)G.A << 32) ^ (uint64)G.B;
		return (uint32)(s ^ (s >> 32));
	}

	static FORCEINLINE void EnsureInit()
	{
		if (!bInited)
		{
			GachaRNG::Init(0);
		}
	}
}

void GachaRNG::Init(uint32 Seed /*=0*/)
{
	if (Seed == 0)
	{
		Seed = MakeAutoSeed();
	}
	GRng.Initialize(Seed);
	bInited = true;
}

uint32 GachaRNG::RandomU32()
{
	EnsureInit();
	return GRng.GetUnsignedInt();
}

float GachaRNG::FRand()
{
	EnsureInit();
	return GRng.FRand();
}

uint32 GachaRNG::UniformIndex(uint32 N)
{
	EnsureInit();
	if (N == 0) return 0;

	// 거부 샘플링으로 편향 제거 (2^32 공간 기준)
	const uint64 Space = 4294967296ull;              // 2^32
	const uint64 Limit = Space - (Space % N);        // Limit 이상 값은 버림
	uint32 r;
	do { r = GRng.GetUnsignedInt(); } while ((uint64)r >= Limit);
	return r % N;
}

int32 GachaRNG::DrawWeightedIndex(const TArray<int32>& Weights)
{
	EnsureInit();

	uint64 Total = 0;
	for (int32 w : Weights)
	{
		Total += (uint32)FMath::Max(0, w);
	}
	if (Total == 0) return INDEX_NONE;

	const uint32 Pick = UniformIndex((uint32)Total); // 0..Total-1
	uint64 Acc = 0;
	for (int32 i = 0; i < Weights.Num(); ++i)
	{
		Acc += (uint32)FMath::Max(0, Weights[i]);
		if (Pick < Acc)
		{
			return i;
		}
	}
	return INDEX_NONE;
}