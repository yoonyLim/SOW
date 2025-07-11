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
	
	UPROPERTY(EditAnywhere)
	int32 GridWidth = 10;
	
	UPROPERTY(EditAnywhere)
	int32 GridHeight = 10;
	
	UPROPERTY(EditAnywhere)
	float TileWidth = 249.0f;
	
	UPROPERTY(EditAnywhere)
	float TileHeight = 249.0f;
	
protected:
	virtual void BeginPlay() override;
};