// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
namespace GachaRNG
{
	void Init(uint32 Seed = 0);

	uint32 RandomU32();

	float  FRand();

	uint32 UniformIndex(uint32 N);

	int32  DrawWeightedIndex(const TArray<int32>& Weights);
}
