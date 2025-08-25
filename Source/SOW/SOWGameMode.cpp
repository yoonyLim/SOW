// Copyright Epic Games, Inc. All Rights Reserved.

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "SOWGameMode.h"
#include "Characters/SOWCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "SOWEnemySpawner.h"
#include "TimerManager.h"

ASOWGameMode::ASOWGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// 초기 변수값 설정 (선택)
	GoldCollected = 0;
	EnemiesKilled = 0;
	WaveSeconds = 0;
	StoredInMinutes = 0;
	WaveMinutes = 0;
	WaveDuration = 0.0f;
	CurrentWaveTime = 0.0f;
	PreInterludeDuration = 0.0f;
	CurrentInterludeTime = 0.0f;
	bIsDuringWave = false;
	bIsDuringInterlude = false;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/01Blueprints/Player/BP_Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
// BeginPlay
void ASOWGameMode::BeginPlay()
{
	Super::BeginPlay();

	// WaveHUD 생성
	CreateWaveHUD();

	// 2. 기본 변수 초기화
	GoldCollected = 0;
	EnemiesKilled = 0;
	NextMonsterSpawnDataIndex = 0; // 또는 적절한 초기값

	// 3. 상태 플래그 초기화
	IsDuringInterlude = false;
	IsDuringWave = false;

	// 4. 적 스포너 정보 불러오기
	GetEnemySpawners();

	// 5. 인터루드 시작
	StartInterlude();
}

void ASOWGameMode::CreateWaveHUD()
{
	if (WaveHUD)
	{
		APlayerController *PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			WaveHUDWidget = CreateWidget<UUserWidget>(PlayerController, WaveHUD);
			if (WaveHUDWidget)
			{
				WaveHUDWidget->AddToViewport();
			}
		}
	}
}

void ASOWGameMode::GetEnemySpawners()
{
	TArray<AActor *> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASOWEnemySpawner::StaticClass(), FoundActors);

	EnemySpawners.Empty();

	for (AActor *Actor : FoundActors)
	{
		ASOWEnemySpawner *Spawner = Cast<ASOWEnemySpawner>(Actor);
		if (Spawner)
		{
			EnemySpawners.Add(Spawner);
		}
	}
}

// Interlude
void ASOWGameMode::StartInterlude(float PreInterludeDuration, int32 WaveNumber)
{
	// 1. 웨이브 데이터 준비
	PrepareWave();

	// 2. 적 스폰 데이터 준비
	PrepareEnemySpawns();

	// 3. 인터루드 시간 계산
	float InterludeDuration = PreInterludeDuration;
	int32 WaveNumber = WaveNumber;

	// 4. 인터루드 시작 이벤트 호출 (블루프린트용 이벤트)
	OnInterludeStart.Broadcast(PreInterludeDuration, WaveNumber);

	// 5. 상태 변수 설정
	bIsDuringInterlude = true;
}

void ASOWGameMode::PrepareWave()
{
	// 1. 초기화
	NextMonsterSpawnDataIndex = 0;
	EnemiesRemaining = 0;
	TotalEnemies = 0;
	MonsterSpawnData.Empty();

	// 2. Wave 데이터 가져오기
	if (WaveDataTable)
	{
		static const FString ContextString(TEXT("WaveDataContext"));
		FName RowName = FName(*FString::FromInt(WaveNumber));

		F_Wave *FoundRow = WaveDataTable->FindRow<F_Wave>(RowName, ContextString, true);
		if (FoundRow)
		{
			// 3. F_Wave 데이터 적용
			MonsterSpawnData = FoundRow->MonsterSpawnData;
			WaveDuration = FoundRow->WaveDuration;
			PreInterludeDuration = FoundRow->PreInterludeTime;

			// 4. 타이머 설정
			SetWaveTimer(WaveDuration);
			SetInterludeTimer(PreInterludeDuration);

			// 5. 적 수 계산
			int32 TotalMonsterCount = GetNumberOfEnemy(MonsterSpawnData);
			EnemiesRemaining = TotalMonsterCount;
			TotalEnemies = TotalMonsterCount;
		}
	}
}

void ASOWGameMode::SetWaveTimer(float InWaveDuration)
{
	WaveDuration = InWaveDuration;

	int32 Minutes = FMath::TruncToInt(InWaveDuration / 60.0f);
	int32 Seconds = FMath::TruncToInt(FMath::Fmod(InWaveDuration, 60.0f));

	CallOnWaveTimerUpdated(Minutes, Seconds);
}

void ASOWGameMode::SetInterludeTimer(float InInterludeTime)
{
	PreInterludeDuration = InInterludeTime;

	CallOnInterludeStart(PreInterludeDuration, WaveNumber);
}

int32 ASOWGameMode::GetNumberOfEnemy(const TArray<F_MonsterSpawnData> &SpawnData) const
{
	int32 MonsterCount = 0;

	for (const F_MonsterSpawnData &Data : SpawnData)
	{
		MonsterCount += Data.MonsterCount;
	}

	return MonsterCount;
}

void ASOWGameMode::PrepareEnemySpawns()
{
	SetNextSpawnTime();
	SetEnemySpawnersToUse();
	QueueEnemiesToEnemySpawners();
}

void ASOWGameMode::SetNextSpawnTime()
{
	// NextMonsterSpawnDataIndex가 유효한지 확인
	if (MonsterSpawnData.IsValidIndex(NextMonsterSpawnDataIndex))
	{
		// MonsterSpawnData 배열에서 해당 인덱스의 데이터를 가져옵니다.
		const F_MonsterSpawnData &NextData = MonsterSpawnData[NextMonsterSpawnDataIndex];

		// 가져온 데이터의 SpawnTime을 NextSpawnTime 변수에 설정합니다.
		NextSpawnTime = NextData.SpawnTime;
	}
}

void ASOWGameMode::SetEnemySpawnersToUse()
{
	// EnemySpawnersToUse 배열을 비웁니다.
	EnemySpawnersToUse.Empty();

	// NextMonsterSpawnDataIndex가 유효한지 확인합니다.
	if (MonsterSpawnData.IsValidIndex(NextMonsterSpawnDataIndex))
	{
		// 해당 인덱스의 몬스터 스폰 데이터를 가져옵니다.
		const F_MonsterSpawnData &NextData = MonsterSpawnData[NextMonsterSpawnDataIndex];

		// 전체 EnemySpawners 배열을 순회합니다.
		for (ASOWEnemySpawner *Spawner : EnemySpawners)
		{
			// 해당 스포너의 인덱스를 SpawnPointIndices 배열에서 찾습니다.
			// (TArray::Find는 인덱스를 반환, INDEX_NONE은 찾지 못했음을 의미)
			// 현재 예제에서는 스포너 배열의 인덱스와 스폰포인트 인덱스가 동일하다고 가정합니다.
			// 블루프린트 로직에서는 `Enemy Spawners`의 인덱스를 사용하여 `Spawn Point Indices`에 포함되어 있는지 확인합니다.

			// 다음 코드는 블루프린트의 `Contains` 노드를 C++로 구현한 것입니다.
			int32 FoundIndex;
			if (NextData.SpawnPointIndices.Find(EnemySpawners.IndexOfByKey(Spawner), FoundIndex))
			{
				EnemySpawnersToUse.Add(Spawner);
			}
		}
	}
}

void ASOWGameMode::QueueEnemiesToEnemySpawners()
{
	// NextMonsterSpawnDataIndex가 유효한지 확인합니다.
	if (MonsterSpawnData.IsValidIndex(NextMonsterSpawnDataIndex))
	{
		// 몬스터 스폰 데이터를 가져옵니다.
		const F_MonsterSpawnData &NextData = MonsterSpawnData[NextMonsterSpawnDataIndex];

		// Enemy Spawners to Use 배열이 비어있지 않은지 확인합니다.
		if (EnemySpawnersToUse.Num() > 0)
		{
			// 몬스터 수만큼 반복합니다.
			for (int32 i = 0; i < NextData.MonsterCount; ++i)
			{
				// 현재 사용할 스포너를 선택합니다.
				ASOWEnemySpawner *CurrentSpawner = EnemySpawnersToUse[CurrentSpawnerToUseIndex];

				if (CurrentSpawner)
				{
					// 스포너의 큐에 몬스터를 추가하는 함수를 호출합니다.
					// 이 함수는 Spawner 클래스에 정의되어 있어야 합니다.
					CurrentSpawner->AddEnemyToQueue(NextData.MonsterType);
				}

				// 다음 스포너 인덱스로 업데이트합니다. (순환)
				CurrentSpawnerToUseIndex = (CurrentSpawnerToUseIndex + 1) % EnemySpawnersToUse.Num();
			}
		}
	}
}

void ASOWGameMode::UpdateInterlude(float DeltaSeconds)
{
	CurrentInterludeTime += DeltaSeconds;
}

// EndInterlude 함수 구현
void ASOWGameMode::EndInterlude()
{
	// 1. Current Interlude Time을 0.0으로 초기화합니다.
	CurrentInterludeTime = 0.0f;

	// 2. bIsDuringInterlude를 false로 설정합니다.
	bIsDuringInterlude = false;

	// 3. CallOnInterludeEnd 이벤트를 호출합니다.
	OnInterludeEnd.Broadcast();

	// 4. StartWave 함수를 호출합니다.
	StartWave();
}

// StartWave 함수는 아직 구현되지 않았습니다.
void ASOWGameMode::StartWave()
{
	// 1. Call On Wave Start 이벤트를 호출합니다.
	OnWaveStart.Broadcast(WaveNumber, EnemiesKilled, TotalEnemies, GoldCollected);

	// 2. bIsDuringWave를 true로 설정합니다.
	bIsDuringWave = true;

	// 3. bDoesNextSpawnExist를 true로 설정합니다.
	bDoesNextSpawnExist = true;
}

// UpdateWave 함수 구현
void ASOWGameMode::UpdateWave(float DeltaSeconds)
{
	CurrentWaveTime += DeltaSeconds;
}

// UpdateWaveTimer 함수 구현
void ASOWGameMode::UpdateWaveTimer()
{
	// Wave Duration을 분으로 변환 (나눗셈 후 정수형으로 변환)
	int32 Minutes = FMath::TruncToInt(WaveDuration / 60.0f);

	// Wave Duration을 초로 변환 (나머지 연산 후 정수형으로 변환)
	int32 Seconds = FMath::TruncToInt(FMath::Fmod(WaveDuration, 60.0f));

	// CallOnWaveTimerUpdated 이벤트를 호출하고 계산된 분과 초를 전달합니다.
	CallOnWaveTimerUpdated(Minutes, Seconds);
}

// SpawnEnemies 함수 구현
void ASOWGameMode::SpawnEnemies()
{
	// For Each Loop와 동일한 로직
	for (ASOWEnemySpawner *Spawner : EnemySpawnersToUse)
	{
		if (Spawner)
		{
			// Spawner 클래스의 SpawnEnemy 함수 호출
			Spawner->SpawnEnemy();
		}
	}

	// For Each Loop의 Completed 핀 이후 로직
	if (CheckNextSpawnExists())
	{
		PrepareEnemySpawns();
	}
	else
	{
		bDoesNextSpawnExist = false;
	}
}

// CheckNextSpawnExists 함수 구현
bool ASOWGameMode::CheckNextSpawnExists()
{
	// 1. NextMonsterSpawnDataIndex를 1 증가시킵니다.
	NextMonsterSpawnDataIndex++;

	// 2. 증가된 인덱스가 MonsterSpawnData 배열의 길이보다 작으면 다음 스폰 데이터가 존재합니다.
	// 배열의 길이는 .Num() 함수로 가져옵니다.
	if (NextMonsterSpawnDataIndex < MonsterSpawnData.Num())
	{
		// 다음 스폰 데이터가 존재하므로 true를 반환합니다.
		return true;
	}
	else
	{
		// 배열의 끝에 도달했으므로 false를 반환합니다.
		return false;
	}
}

// EndWave 함수 구현
void ASOWGameMode::EndWave()
{
	// 1. Current Wave Time을 0.0으로 초기화합니다.
	CurrentWaveTime = 0.0f;

	// 2. Is During Wave를 false로 설정합니다.
	bIsDuringWave = false;

	// 3. Call On Wave End (이벤트 디스패처 호출)
	OnWaveEnd.Broadcast();

	// 4. 다음 웨이브가 존재하는지 확인하고, 존재하면 Start Interlude를 호출합니다.
	if (CheckNextInterludeExists())
	{
		StartInterlude();
	}
}

// CheckNextInterludeExists 함수
bool ASOWGameMode::CheckNextInterludeExists();
{
	// 1. Wave Number를 1 증가시킵니다.
	WaveNumber++;

	// 2. 증가된 Wave Number를 기반으로 데이터 테이블에서 행을 찾습니다.
	if (WaveDataTable)
	{
		// FName으로 변환하기 위해 FString을 사용
		FName RowName = FName(*FString::FromInt(WaveNumber));

		// 데이터 테이블에서 행을 찾습니다.
		// FindRow 함수는 행을 찾으면 nullptr이 아닌 포인터를 반환합니다.
		const F_Wave *FoundRow = WaveDataTable->FindRow<F_Wave>(RowName, TEXT("CheckNextWaveExists"), false);

		// 3. 행이 존재하는지 확인하고 결과를 반환합니다.
		if (FoundRow)
		{
			return true;
		}
	}

	// 데이터 테이블이 없거나 행을 찾지 못한 경우
	return false;
}

// EventTick 함수 구현
void ASOWGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 인터루드 진행 중일 경우
	if (bIsDuringInterlude)
	{
		UpdateInterlude(DeltaSeconds);
	}

	// 웨이브 진행 중일 경우
	else if (bIsDuringWave)
	{
		UpdateWave(DeltaSeconds);
	}
}
