// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/ShadowTileGimmickManager.h"
#include "EngineUtils.h"
void AShadowTileGimmickManager::BeginPlay()
{
	Super::BeginPlay();
// 10초 간격으로 변경 수행
	GetWorld() -> GetTimerManager().SetTimer(TransformTimerHandle, this, &AShadowTileGimmickManager::PickRandomTileAndTransform, 10.0f, true, 5.0f);
}

void AShadowTileGimmickManager::PickRandomTileAndTransform()
{
	FGameplayTag FilterTag = FGameplayTag::RequestGameplayTag("Tile.Grass");
	TArray<ATileBase*> FilteredTiles;

	for (TActorIterator<ATileBase> It(GetWorld()); It; ++It)
	{
		ATileBase* Tile = *It;
		if (Tile && Tile->TileTag.MatchesTag(FilterTag))
		{
			FilteredTiles.Add(Tile);
		}
	}

	if (FilteredTiles.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, FilteredTiles.Num() - 1);
		ATileBase* SelectedTile = FilteredTiles[RandomIndex];

		if (SelectedTile->ReplacementClass)
		{
			
			FVector Location = SelectedTile->GetActorLocation();
			FRotator Rotation = SelectedTile->GetActorRotation();

			TSubclassOf<ATileBase> OriginalClass = SelectedTile->GetClass();

			SelectedTile->Destroy();

			ATileBase* NewTile = GetWorld()->SpawnActor<ATileBase>(
				SelectedTile->ReplacementClass,
				Location,
				Rotation
			);
			FTimerHandle RevertHandle;
			//5초 후 원복
			GetWorld()->GetTimerManager().SetTimer(
				RevertHandle, FTimerDelegate::CreateUObject(this, &AShadowTileGimmickManager::RevertTile,Location,Rotation,OriginalClass),5.0f,false
			);
		}
	}
}

void AShadowTileGimmickManager::RevertTile(FVector Location, FRotator Rotation, TSubclassOf<ATileBase> OriginalClass)
{
	if (OriginalClass)
	{
		GetWorld()->SpawnActor<ATileBase>(OriginalClass, Location, Rotation);
	}
}