#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SOWTileSpawnerActor.generated.h"
UCLASS()
class SOW_API ATileSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AActor>> GridTiles;

	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> SpawnedTileActors;
	// 현재 맵에 스폰된 타일 인스턴스를 저장하는 배열
	// GridTiles[index]로 생성된 액터와 동일한 index
	// (x,y) 의 index = y * GridWidth + x
	
	UPROPERTY(EditAnywhere)
	int32 GridWidth = 10;
	
	UPROPERTY(EditAnywhere)
	int32 GridHeight = 10;
	
	UPROPERTY(EditAnywhere)
	float TileWidth = 124.5f;
	
	UPROPERTY(EditAnywhere)
	float TileHeight = 124.5f;
	
protected:
	virtual void BeginPlay() override;
};