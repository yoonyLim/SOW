#include "Tile/SOWTileSpawnerActor.h"
#include "Tile/SOWTilePlacementHelper.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "Interface/GridTileInterface.h"
#include "Utilities/EnemyIncomingRoute.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

void ATileSpawner::BeginPlay()
{
	if (DefaultTileClass)
	{
		if (GridTiles.Num() < GridWidth * GridHeight)
		{
			GridTiles.SetNum(GridWidth * GridHeight);
		}
		for (int32 i = 0; i < GridTiles.Num(); ++i)
		{
			if (!GridTiles[i])
			{
				GridTiles[i] = DefaultTileClass;
			}
		}
	}
	Super::BeginPlay();

	SpawnedTileLocations.Empty();
	SpawnedEnemyRoutes.Empty();

	int32 EnemySpawnerIndexToSpawn = 0;

	for (int32 X = 0; X < GridWidth; ++X)
	{
		for (int32 Y = 0; Y < GridHeight; ++Y)
		{
			const int32 Index = Y * GridWidth + X;

			if (!GridTiles.IsValidIndex(Index) || !GridTiles[Index])
			{
				continue;
			}

			FVector SpawnLocation = SOWTilePlacementHelper::GetTileWorldPosition(X, Y, TileWidth, TileHeight);
			
			if (GridTiles[Index] && GridTiles[Index]->GetName().Contains(TEXT("2x2")))
			{
				SpawnLocation += FVector( 0, TileHeight, 0.0f);
			}
			
			FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);

			if (AActor* SpawnedTile = GetWorld()->SpawnActor<AActor>(GridTiles[Index], SpawnLocation, Rotation))
			{
				SpawnedTileActors.Add(SpawnedTile);
				SpawnedTileLocations.Add(Index, SpawnLocation);

				if (SpawnedTile->Implements<UGridTileInterface>())
				{
					if (EnemySpawnerIndexToSpawn < EnemySpawnerIndexInOrder.Num() && GridTiles[Index]->GetName().Contains(TEXT("Enemy")))
					{
						IGridTileInterface::Execute_SetTileEnemySpawnerIndex(SpawnedTile, EnemySpawnerIndexInOrder[EnemySpawnerIndexToSpawn]); // Call the interface function
						// UE_LOG(LogTemp, Error, TEXT("Enemy Spawner '%d' set."), EnemySpawnerIndexInOrder[EnemySpawnerIndexToSpawn]);
						EnemySpawnerIndexToSpawn++;
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Enemy Spawner failed to set."));
					}
				}
			}
		}
	}

	SpawnIncomingRoutes();

	// 맵 중심 & 크기 계산
	FVector CenterWS = FVector::ZeroVector;
	FVector2D HalfSizeWorldXY = FVector2D::ZeroVector;
	ComputeMapBoundsFromTiles(SpawnedTileLocations, TileWidth, TileHeight, CenterWS, HalfSizeWorldXY);

	// 원래 쓰던 Z를 유지하고 싶다면:
	CenterWS.Z = SOWTilePlacementHelper::GetTileWorldPosition(
		(GridWidth - 1) / 2.0f, (GridHeight - 1) / 2.0f, TileWidth, TileHeight).Z;

	if (bSpawnGradientPlane)
	{
		SetupGradientPlaneAndMaterial(CenterWS, HalfSizeWorldXY);
	}
	const FVector Center = SOWTilePlacementHelper::GetTileWorldPosition((GridWidth - 1) / 2.0f, (GridHeight - 1) / 2.0f, TileWidth, TileHeight);
	const FVector Extent = FVector(
		(GridWidth * TileWidth * 0.5f) + TileWidth,
		(GridHeight * TileHeight * 0.5f) + TileHeight,
		2000.0f
	);

	ANavMeshBoundsVolume* Volume = nullptr;
	for (TActorIterator<ANavMeshBoundsVolume> It(GetWorld()); It; ++It)
	{
		Volume = *It;
		break;
	}

	if (!Volume)
	{
		FActorSpawnParameters SpawnParams;
		Volume = GetWorld()->SpawnActor<ANavMeshBoundsVolume>(ANavMeshBoundsVolume::StaticClass(), Center, FRotator::ZeroRotator, SpawnParams);
	}

	if (Volume)
	{
		Volume->SetActorLocation(Center);
		Volume->SetActorScale3D(FVector(1.0f));
		
		if (Volume->GetRootComponent())
			Volume->GetRootComponent()->SetWorldScale3D(Extent);
	}

	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		NavSys->Build();
	}
}

void ATileSpawner::SpawnIncomingRoutes()
{
	if (!EnemyIncomingRouteClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyIncomingRouteClass is not set in ATileSpawner! Cannot spawn enemy routes."));
		return;
	}
	
	SpawnedEnemyRoutes.Empty(); 

	for (int32 RouteIndex = 0; RouteIndex < CustomIncomingRoutes.Num(); ++RouteIndex)
	{
		const FIncomingRouteDefinition& CurrentRouteDefinition = CustomIncomingRoutes[RouteIndex];
		const TArray<int32>& CurrentRouteIndices = CurrentRouteDefinition.TileIndices;
		TArray<FVector> CurrentRouteWorldLocations;

		for (const int32 TileIndex : CurrentRouteIndices)
		{
			if (SpawnedTileLocations.Contains(TileIndex))
			{
				// FVector SpawnLocation = FVector(SpawnedTileLocations[TileIndex].X, SpawnedTileLocations[TileIndex].Y, SpawnedTileLocations[TileIndex].Z + 5.f);
				CurrentRouteWorldLocations.Add(SpawnedTileLocations[TileIndex]);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Tile index %d for route %d was not found in SpawnedTileLocations. Route may be incomplete."), TileIndex, RouteIndex);
			}
		}

		if (CurrentRouteWorldLocations.Num() > 1) // at least two points for a spline
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Owner = this;

			AEnemyIncomingRoute* NewRouteActor = GetWorld()->SpawnActor<AEnemyIncomingRoute>(
				EnemyIncomingRouteClass, 
				FVector::ZeroVector, // Location doesn't matter much relative to spline points
				FRotator::ZeroRotator, 
				SpawnParams
			);

			if (NewRouteActor)
			{
				NewRouteActor->SetSplinePointsFromLocations(CurrentRouteWorldLocations);
				NewRouteActor->SetActorLabel(FString::Printf(TEXT("EnemyRoute_%d"), RouteIndex));
				SpawnedEnemyRoutes.Add(NewRouteActor);
				UE_LOG(LogTemp, Log, TEXT("Spawned EnemyIncomingRoute Actor for Route %d with %d points. Total routes: %d"), 
					RouteIndex, CurrentRouteWorldLocations.Num(), SpawnedEnemyRoutes.Num());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to spawn AEnemyIncomingRoute actor for Route %d."), RouteIndex);
			}
		}
		else if (CurrentRouteWorldLocations.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Route %d has only %d point(s). Skipping spline creation (requires > 1 point)."), RouteIndex, CurrentRouteWorldLocations.Num());
		}
	}
}

TArray<AEnemyIncomingRoute*> ATileSpawner::GetSpawnedEnemyRoutes() const
{
	return SpawnedEnemyRoutes;
}


// 평면 자동 세팅 + 머티리얼 파라미터 주입
void ATileSpawner::SetupGradientPlaneAndMaterial(const FVector& CenterWS, const FVector2D& HalfSizeWorldXY)
{
    UWorld* World = GetWorld();
    if (!World) return;

    if (!GradientPlaneMesh) {
        static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneSM(TEXT("/Engine/BasicShapes/Plane.Plane"));
        if (PlaneSM.Succeeded()) GradientPlaneMesh = PlaneSM.Object;
    }
    if (!GradientPlaneMesh) { UE_LOG(LogTemp, Warning, TEXT("GradientPlaneMesh is null.")); return; }
    if (!GradientMaterial)  { UE_LOG(LogTemp, Warning, TEXT("GradientMaterial is null."));  return; }

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    const FRotator PlaneRot = GetActorRotation() + FRotator(0.f, 45.f, 0.f); // 다이아 맵
    const FVector  PlaneLoc = CenterWS + FVector(0.f, 0.f, PlaneZOffset);

    GradientPlaneActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), PlaneLoc, PlaneRot, Params);
    if (!GradientPlaneActor) { UE_LOG(LogTemp, Error, TEXT("Failed to spawn GradientPlaneActor")); return; }

    UStaticMeshComponent* PlaneMC = GradientPlaneActor->GetStaticMeshComponent();
    PlaneMC->SetStaticMesh(GradientPlaneMesh);
    PlaneMC->SetMobility(EComponentMobility::Movable);
    PlaneMC->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
    PlaneMC->CastShadow = false;
    PlaneMC->bRenderCustomDepth = false;
    PlaneMC->SetMaterial(0, GradientMaterial);

    // 평면 목표 크기 = 맵 + 양쪽 Softness 여유
    const float TargetW = HalfSizeWorldXY.X * 5.f + 2.f * GradientSoftnessWorld;
    const float TargetH = HalfSizeWorldXY.Y * 5.f + 2.f * GradientSoftnessWorld;

    const FVector SMSize = GradientPlaneMesh->GetBounds().BoxExtent * 2.f; // 로컬
    const FVector NewScale(
        TargetW / FMath::Max(1.f, SMSize.X),
        TargetH / FMath::Max(1.f, SMSize.Y),
        1.f
    );
    PlaneMC->SetWorldScale3D(NewScale);

    // MID 생성 + 파라미터
    UMaterialInstanceDynamic* Dyn =
        PlaneMC->CreateAndSetMaterialInstanceDynamicFromMaterial(0, GradientMaterial);
    if (!Dyn) { UE_LOG(LogTemp, Error, TEXT("Failed to create MID for GradientPlane.")); return; }

    // 머티리얼 내부에서 World→Local + ObjectScale 보정 사용 가정
    Dyn->SetVectorParameterValue(TEXT("HalfSizeWorld"),
        FLinearColor(HalfSizeWorldXY.X, HalfSizeWorldXY.Y, 0, 0));
    Dyn->SetScalarParameterValue(TEXT("SoftnessWorld"), GradientSoftnessWorld);
	Dyn->SetVectorParameterValue(TEXT("MapCenterWS"), FLinearColor(CenterWS.X, CenterWS.Y, CenterWS.Z, 0));
	
	// 머티리얼 내부 innerColor 나타나는 범위
	Dyn->SetScalarParameterValue(TEXT("InnerRadiusColor"), GradientInnerRadius);
	
	// innercolor, outercolor 색상 지정
	Dyn->SetVectorParameterValue(TEXT("InnerColor"), GradientInnerColor);
	Dyn->SetVectorParameterValue(TEXT("OuterColor"), GradientOuterColor);
	
}


// 스폰된 타일 위치들(중심)로부터 실제 맵 경계를 계산
void ATileSpawner::ComputeMapBoundsFromTiles(
	const TMap<int32, FVector>& TileCenters,
	float TileWidth, float TileHeight,
	FVector& OutCenterWS,            // XY 중심 (Z는 호출부에서 원하는 값으로 보정)
	FVector2D& OutHalfSizeWorldXY)   // XY 반경 (월드 단위)
{
	if (TileCenters.Num() == 0) { OutCenterWS = FVector::ZeroVector; OutHalfSizeWorldXY = FVector2D::ZeroVector; return; }

	float minX =  FLT_MAX, maxX = -FLT_MAX;
	float minY =  FLT_MAX, maxY = -FLT_MAX;

	for (const auto& It : TileCenters)
	{
		const FVector& P = It.Value;   // 타일 중심 좌표
		minX = FMath::Min(minX, P.X);  maxX = FMath::Max(maxX, P.X);
		minY = FMath::Min(minY, P.Y);  maxY = FMath::Max(maxY, P.Y);
	}

	const float halfTileX = TileWidth  * 0.5f;   // 외곽 타일의 반쪽까지 포함
	const float halfTileY = TileHeight * 0.5f;

	OutCenterWS.X = 0.5f * (minX + maxX);
	OutCenterWS.Y = 0.5f * (minY + maxY);

	OutHalfSizeWorldXY.X = 0.5f * (maxX - minX) + halfTileX;
	OutHalfSizeWorldXY.Y = 0.5f * (maxY - minY) + halfTileY;
}
