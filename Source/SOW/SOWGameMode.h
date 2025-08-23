// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SOWGameMode.generated.h"
#include "SOWCharacterEnemyBase.h"
#inlcude "Delegates/DelegateCombinations.h" // Core

class ASOWEnemySpawner;
class UUserWidget;
struct F_Wave;
struct F_MonsterSpawnData;

// 델리게이트 선언
// 이 노드는 'Interlude Duration' (float)과 'Wave Number' (int) 매개변수를 받으므로,
// 이에 해당하는 델리게이트를 선언합니다.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInterludeEnd, FOnInterludeStart, float, InterludeDuration, int32, WaveNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInterludeEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnWaveStart, int32, WaveNumber, int32, EnemiesKilled, int32, TotalEnemies, int32, GoldCollected);

UCLASS(minimalapi)
class ASOWGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASOWGameMode();

protected:
	virtual void BeginPlay() override;

	// === Wave Timer ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Timer")
	int32 WaveSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Timer")
	int32 StoredInMinutes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Timer")
	float WaveDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Timer")
	float CurrentWaveTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Timer")
	bool bIsDuringWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Timer")
	float PreInterludeDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Timer")
	float CurrentInterludeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Timer")
	bool bIsDuringInterlude;

	// === Wave Next Spawn Data ===

	/** 전체 적 스포너 목록 (씬에 존재하는 모든 BP Enemy Spawner) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Next Spawn Data")
	TArray<ASOWEnemySpawner *> EnemySpawners;

	/** 현재 웨이브에서 사용할 스포너 목록 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Next Spawn Data")
	TArray<ASOWEnemySpawner *> EnemySpawnersToUse;

	/** 다음 스폰 예정 시간 (게임 시간 기준) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Next Spawn Data")
	float NextSpawnTime;

	/** 다음으로 스폰할 몬스터 데이터의 인덱스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Next Spawn Data")
	int32 NextMonsterSpawnDataIndex;

	/** 다음 스폰이 존재하는지 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Next Spawn Data")
	bool bDoesNextSpawnExist;

	// === Wave Current Info ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Info")
	int32 GoldCollected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Info")
	int32 EnemiesRemaining;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Info")
	int32 EnemiesKilled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Info")
	int32 TotalEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Info")
	int32 WaveNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Info")
	F_Wave WaveData

		// 데이터 테이블 참조
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wave Data")
			UDataTable *WaveDataTable;

	// 적 스폰 구조체
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Info")
	F_MonsterSpawnData MonsterSpawnData;

	// 어떤 적 타입(클래스)을 스폰할지 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASOWCharactersEnemyBase> MonsterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MonsterCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> SpawnPointIndices;

	// HUD 클래스 설정용 (이미 선언됨)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave HUD")
	TSubclassOf<UUserWidget> WaveHUD;

	// 실제 생성된 HUD 인스턴스
	UPROPERTY(BlueprintReadOnly, Category = "Wave HUD")
	UUserWidget *WaveHUDWidget;

	// === Components ===
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USceneComponent *DefaultSceneRoot;

	// === 시간 ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 WaveMinutes;

	// Enemyspawner class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Next Spawn Data")
	TArray<ASOWEnemySpawner *> EnemySpawners;

	// 다른 변수 선언
	UPROPERTY()
	int32 CurrentSpawnerToUseIndex;

	// 인터루드 타이머 관리를 위한 FTimerHandle
	UPROPERTY()
	FTimerHandle InterludeTimerHandle;

	// 타이머 설정 함수
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void SetWaveTimer(float InDuration);

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void SetInterludeTimer(float InDuration);

	// 적 수 계산 함수
	UFUNCTION(BlueprintCallable, Category = "Wave")
	int32 GetNumberOfEnemy(const TArray<F_MonsterSpawnData> &SpawnData) const;

	// 스포너 함수
	UFUNCTION(BlueprintCallable, Category = "Wave Spawn")
	void GetEnemySpawners();

	// 인터루드를 시작하는 이벤트
	UFUNCTION(BlueprintCallable, Category = "Interlude")
	void StartInterlude();

	// PrepareWave 함수 선언
	UFUNCTION(BlueprintCallable, Category = "Wave Spawn")
	void PrepareWave();

	// PrepareEnemySpawns 함수 선언
	UFUNCTION(BlueprintCallable, Category = "Wave Spawn")
	void PrepareEnemySpawns();

	// PrepareEnemySpanws에서 호출할 함수들 선언
	UFUNCTION(BlueprintCallable, Category = "Wave Spawn")
	void SetNextSpawnTime();

	UFUNCTION(BlueprintCallable, Category = "Wave Spawn")
	void SetEnemySpawnersToUse();

	UFUNCTION(BlueprintCallable, Category = "Wave Spawn")
	void QueueEnemiesToEnemySpawners();

	// 인터루드 시간을 업데이트하는 함수
	UFUNCTION(BlueprintCallable, Category = "Time")
	void UpdateInterlude(float DeltaSeconds);

	// UpdateInterludeTimer 함수 선언
	UFUNCTION(BlueprintCallable, Category = "Wave HUD")
	void UpdateInterludeTimer();

	// 블루프린트에서 구현될 이벤트 선언
	UFUNCTION(BlueprintImplementableEvent, Category = "Wave Spawn")
	void OnInterludeTimerUpdated(float Current Interlude Time);

	// 인터루드 종료 함수
	UFUNCTION(BlueprintCallable, Category = "Wave Spawn")
	void EndInterlude();

	// 웨이브
	// 웨이브 시작 함수
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void StartWave(int32 WaveNumber, int32 EnemiesKilled, int32 TotalEnemies, int32 GoldCollected);

	// 블루프린트에서 구현될 이벤트 선언
	UFUNCTION(BlueprintImplementableEvent, Category = "Wave Spawn")
	void CallOnWaveStart(int32 WaveNumber, int32 EnemiesKilled, int32 TotalEnemies, int32 GoldCollected);

	// 웨이브 업데이트 함수
	// UpdateWave 함수 선언 (Blueprint Callable)
	UFUNCTION(BlueprintCallable, Category = "Wave Spawn")
	void UpdateWave(float DeltaSeconds);

	// UpdateWaveTimer 함수 선언 (Blueprint Callable)
	UFUNCTION(BlueprintCallable, Category = "Wave Spawn")
	void UpdateWaveTimer();

	// 웨이브 종료 함수
	UFUNCTION(BlueprintCallable, Category = "Wave Spawn")
	void EndWave();

	// 적 스폰 함수
	UFUNCTION(BlueprintCallable, Category = "Wave Spawn")
	void SpawnEnemies();

	// 다음 스폰 존재 여부 확인 함수
	UFUNCTION(BlueprintCallable, Category = "Wave Spawn")
	bool CheckNextSpawnExists();

	// 블루프린트에서 구현될 이벤트 선언
	UFUNCTION(BlueprintImplementableEvent, Category = "Wave Spawn")
	void WaveTimerUpdated(int32 Minutes, int32 Seconds);

	// 웨이브 종료 이벤트
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void EndWave();

	// 다음 웨이브 존재 여부 확인 함수
	UFUNCTION(BlueprintCallable, Category = "Wave Spawn")
	bool CheckNextInterludeExists();

	// 이벤트 디스패치 선언
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnInterludeStart OnInterludeStart;

	// 블루프린트에서 구현될 이벤트 선언
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnInterludeEnd OnInterludeEnd;

	// 델리게이트 변수 선언
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWaveStart OnWaveStart;

	// 델리게이트 변수 선언
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWaveEnd OnWaveEnd;
};
