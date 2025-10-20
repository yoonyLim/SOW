// Fill out your copyright notice in the Description page of Project Settings.


#include "Sound/SoundInstance.h"
#include "Sound/SoundManager.h"

void USoundInstance::Init()
{
	Super::Init();

	// 게임 시작 시 사운드 매니저 초기화
	auto* Manager = USoundManager::Get(this);
	
	if (StartBGM)
	{
		Manager->PlayBGM(StartBGM,1.0f);
	}
}
