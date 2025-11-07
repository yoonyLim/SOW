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
#include "Core/SOWPlayerController.h"

#include "SOWGameInstance.h"

#include "Utilities/MapScriptLoader.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

void ATileSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnedTileLocations.Empty();
	SpawnedEnemyRoutes.Empty();
	SpawnedTileActors.Empty();

	bool bBuiltOk = false;

	if (bUseExternalMapScript)
	{
		bBuiltOk = LoadAndBuildFromScript(MapScriptFile.FilePath);
	}
	else
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

				const FRotator Rotation = FRotator::ZeroRotator;

				if (AActor* SpawnedTile = GetWorld()->SpawnActor<AActor>(GridTiles[Index], SpawnLocation, Rotation))
				{
					SpawnedTileActors.Add(SpawnedTile);
					SpawnedTileLocations.Add(Index, SpawnLocation);

					if (SpawnedTile->Implements<UGridTileInterface>())
					{
						if (EnemySpawnerIndexToSpawn < EnemySpawnerIndexInOrder.Num() && GridTiles[Index]->GetName().Contains(TEXT("Enemy")))
						{
							IGridTileInterface::Execute_SetTileEnemySpawnerIndex(SpawnedTile, EnemySpawnerIndexInOrder[EnemySpawnerIndexToSpawn]);
							EnemySpawnerIndexToSpawn++;
						}
					}
				}
			}
		}

		bBuiltOk = (SpawnedTileActors.Num() > 0);
	}

	if (!bBuiltOk)
	{
		UE_LOG(LogTemp, Error, TEXT("TileSpawner: Tile build failed."));
		return;
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

	/*ANavMeshBoundsVolume* Volume = nullptr;
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
	}*/

	USOWGameInstance* GI = Cast<USOWGameInstance>(GetGameInstance());
	GI->SetWorldTileSize(TileWidth);
}

void ATileSpawner::SpawnIncomingRoutes()
{
	if (RoutesFromScript.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("TileSpawner: [Routes] No routes defined in .ini; skipping."));
		return;
	}

	if (!EnemyIncomingRouteClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("TileSpawner: [Routes] EnemyIncomingRouteClass is not set; cannot spawn routes. "));
		return;
	}

	for (AEnemyIncomingRoute* R : SpawnedEnemyRoutes)
	{
		if (IsValid(R)) { R->Destroy(); }
	}

	SpawnedEnemyRoutes.Reset();

	for (int32 RouteIndex = 0; RouteIndex < RoutesFromScript.Num(); ++RouteIndex)
	{
		const FIncomingRouteDefinition& Def = RoutesFromScript[RouteIndex];

		TArray<FVector> Points;
		Points.Reserve(Def.TileIndices.Num());

		for (int32 TileIndex : Def.TileIndices)
		{
			if (const FVector* P = SpawnedTileLocations.Find(TileIndex))
			{
				Points.Add(*P);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("TileSpawner: [Routes] '%s': TileIndex %d not found in SpawnedTileLocations"),
					*Def.RouteName, TileIndex);
			}
		}

		if (Points.Num() < 2)
		{
			UE_LOG(LogTemp, Warning, TEXT("TileSpawner: [Routes] '%s': has only %d point(s). Skipped."),
				*Def.RouteName, Points.Num());
			continue;
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;

		AEnemyIncomingRoute* Route = GetWorld()->SpawnActor<AEnemyIncomingRoute>(
			EnemyIncomingRouteClass,
			FVector::ZeroVector, // Location doesn't matter much relative to spline points
			FRotator::ZeroRotator,
			SpawnParams
		);

		if (!Route)
		{
			UE_LOG(LogTemp, Error, TEXT("[Routes] Failed to spawn route actor for '%s'."), *Def.RouteName);
			continue;
		}

		Route->SetSplinePointsFromLocations(Points);
		// Route->SetActorLabel(Def.RouteName.IsEmpty() ? FString::Printf(TEXT("EnemyRoute_%d"), RouteIndex) : Def.RouteName);
		SpawnedEnemyRoutes.Add(Route);

	}
}

TArray<TObjectPtr<AEnemyIncomingRoute>> ATileSpawner::GetSpawnedEnemyRoutes() const
{
	return SpawnedEnemyRoutes;
}


// === 평면 자동 세팅 + 머티리얼 파라미터 주입 ===
// 선언부(.h)도 함께 변경:
// void SetupGradientPlaneAndMaterial(const FVector& CenterWS, float MapW, float MapH);
void ATileSpawner::SetupGradientPlaneAndMaterial(const FVector& CenterWS, const FVector2D& HalfSizeWorldXY)
{
	UWorld* World = GetWorld();
	if (!World) return;

	if (!GradientPlaneMesh) {
		static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneSM(TEXT("/Engine/BasicShapes/Plane.Plane"));
		if (PlaneSM.Succeeded()) GradientPlaneMesh = PlaneSM.Object;
	}
	if (!GradientPlaneMesh) { UE_LOG(LogTemp, Warning, TEXT("GradientPlaneMesh is null.")); return; }
	if (!GradientMaterial) { UE_LOG(LogTemp, Warning, TEXT("GradientMaterial is null."));  return; }

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FRotator PlaneRot = GetActorRotation() + FRotator(0.f, 0.f, 0.f); // 다이아 맵
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


}


// 스폰된 타일 위치들(중심)로부터 실제 맵 경계를 계산
void ATileSpawner::ComputeMapBoundsFromTiles(
	const TMap<int32, FVector>& TileCenters,
	float TileWidth, float TileHeight,
	FVector& OutCenterWS,            // XY 중심 (Z는 호출부에서 원하는 값으로 보정)
	FVector2D& OutHalfSizeWorldXY)   // XY 반경 (월드 단위)
{
	if (TileCenters.Num() == 0) { OutCenterWS = FVector::ZeroVector; OutHalfSizeWorldXY = FVector2D::ZeroVector; return; }

	float minX = FLT_MAX, maxX = -FLT_MAX;
	float minY = FLT_MAX, maxY = -FLT_MAX;

	for (const auto& It : TileCenters)
	{
		const FVector& P = It.Value;   // 타일 중심 좌표
		minX = FMath::Min(minX, P.X);  maxX = FMath::Max(maxX, P.X);
		minY = FMath::Min(minY, P.Y);  maxY = FMath::Max(maxY, P.Y);
	}

	const float halfTileX = TileWidth * 0.5f;   // 외곽 타일의 반쪽까지 포함
	const float halfTileY = TileHeight * 0.5f;

	OutCenterWS.X = 0.5f * (minX + maxX);
	OutCenterWS.Y = 0.5f * (minY + maxY);

	OutHalfSizeWorldXY.X = 0.5f * (maxX - minX) + halfTileX;
	OutHalfSizeWorldXY.Y = 0.5f * (maxY - minY) + halfTileY;
}

bool ATileSpawner::ResolveMapScriptPath(const FString& Input, FString& OutAbsPath)
{
	const FString BaseName = FPaths::GetCleanFilename(Input);

	const FString NameNoExt = FPaths::GetBaseFilename(BaseName, false);
	const FString FinalName = NameNoExt + TEXT(".ini");

	OutAbsPath = FPaths::ConvertRelativePathToFull(
		FPaths::ProjectContentDir() / TEXT("04Scripts/Map") / FinalName
	);

	return FPaths::FileExists(OutAbsPath);
}

bool ATileSpawner::LoadAndBuildFromScript(const FString& InFilePathRelOrAbs)
{
	FString AbsPath;
	if (!ResolveMapScriptPath(InFilePathRelOrAbs, AbsPath))
	{
		UE_LOG(LogTemp, Error, TEXT("TileSpawner: [MapScript] File not found under Content/04Scripts/Map: %s"), *InFilePathRelOrAbs);
		return false;
	}

	FLoadedMapSpec Spec;
	FString Err;

	if (!MapScriptLoader::LoadFromFile(AbsPath, Spec, Err))
	{
		UE_LOG(LogTemp, Error, TEXT("TileSpawner: [MapScript] Load failed: %s (%s)"), *Err, *AbsPath);
		return false;
	}

	ClearSpawnedTiles();
	RoutesFromScript.Reset();
	RoutesFromScript.Reserve(Spec.EnemyRoutes.Num());

	GridWidth = Spec.GridWidth;
	GridHeight = Spec.GridHeight;
	TileWidth = Spec.TileWidth;
	TileHeight = Spec.TileHeight;

	int32 EnemySpawnerIndexToSpawn = 0;

	for (int32 Y = 0; Y < Spec.GridHeight; ++Y)
	{
		for (int32 X = 0; X < Spec.GridWidth; ++X)
		{
			const FName RawToken = Spec.GridTokens[Y][X];
			const FName UseToken = RawToken.IsNone() ? Spec.DefaultToken : RawToken;

			const TSoftClassPtr<AActor>* Found = Spec.TokenToClass.Find(UseToken);
			const TSoftClassPtr<AActor>* Fallback = Spec.TokenToClass.Find(Spec.DefaultToken);

			TSubclassOf<AActor> TileClass = nullptr;
			if (Found && !Found->IsNull()) TileClass = Found->LoadSynchronous();
			else if (Fallback && !Fallback->IsNull()) TileClass = Fallback->LoadSynchronous();

			if (!TileClass)
			{
				UE_LOG(LogTemp, Error, TEXT("TileSpawner: [MapScript] Missing class '%s' (no default). Skip. "), *UseToken.ToString());
				continue;
			}

			const int32 Index = Y * Spec.GridWidth + X;
			const FVector SpawnLocation = SOWTilePlacementHelper::GetTileWorldPosition(X, Y, Spec.TileWidth, Spec.TileHeight);

			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AActor* SpawnedTile = GetWorld()->SpawnActor<AActor>(TileClass, SpawnLocation, FRotator::ZeroRotator);
			if (!SpawnedTile) { continue; }

			SpawnedTileActors.Add(SpawnedTile);
			SpawnedTileLocations.Add(Index, SpawnLocation);

			if (SpawnedTile->Implements<UGridTileInterface>() && UseToken == FName("ENEMY"))
			{
				if (EnemySpawnerIndexToSpawn < EnemySpawnerIndexInOrder.Num())
				{
					IGridTileInterface::Execute_SetTileEnemySpawnerIndex(
						SpawnedTile, EnemySpawnerIndexInOrder[EnemySpawnerIndexToSpawn++]
					);
				}
			}
		}
	}

	for (const FEnemyRoute& R : Spec.EnemyRoutes)
	{
		FIncomingRouteDefinition Def;
		Def.RouteName = R.RouteName.IsEmpty() ? TEXT("Route") : R.RouteName;

		for (const FIntPoint& P : R.Points)
		{
			const int32 X = P.X;
			const int32 Y = P.Y;

			const int32 TileIndex = Y * Spec.GridWidth + X;
			Def.TileIndices.Add(TileIndex);
		}
		RoutesFromScript.Add(MoveTemp(Def));
	}

	UE_LOG(LogTemp, Log, TEXT("TileSpawner: [MapScript] Build %d x %d tiles from %s"), GridWidth, GridHeight, *AbsPath);

	ASOWPlayerController* SOWPC = Cast<ASOWPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	SOWPC->InitTileMap(SpawnedTileActors);

	return SpawnedTileActors.Num() > 0;
}

void ATileSpawner::ClearSpawnedTiles()
{
	if (GradientPlaneActor && IsValid(GradientPlaneActor))
	{
		GradientPlaneActor->Destroy();
		GradientPlaneActor = nullptr;
	}

	for (AActor* A : SpawnedTileActors)
	{
		if (IsValid(A)) { A->Destroy(); }
	}
	SpawnedTileActors.Reset();
	SpawnedTileLocations.Reset();
}

void ATileSpawner::ClearSpawnedRoutes()
{
	for (AEnemyIncomingRoute* R : SpawnedEnemyRoutes)
	{
		if (IsValid(R)) { R->Destroy(); }
	}
	SpawnedEnemyRoutes.Reset();
}