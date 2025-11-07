#include "Tile/SOWTileSpawner.h"
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

void ASOWTileSpawner::BeginPlay()
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

				FVector SpawnLocation = GridToWorld_TopLeft(X, Y, TileWidth, TileHeight);
				SpawnLocation.Z = GetActorLocation().Z;

				const FRotator Rotation = FRotator::ZeroRotator;

				if (AActor* SpawnedTile = GetWorld()->SpawnActor<AActor>(GridTiles[Index], SpawnLocation, Rotation))
				{
					SpawnedTileActors.Add(SpawnedTile);
					SpawnedTileLocations.Add(Index, SpawnLocation);

					ApplyArcheTypeIfSet(SpawnedTile, X, Y, FName("BUILD"), TileWidth);

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

	// �� �߽� & ũ�� ���
	FVector CenterWS = FVector::ZeroVector;
	FVector2D HalfSizeWorldXY = FVector2D::ZeroVector;
	ComputeMapBoundsFromTiles(SpawnedTileLocations, TileWidth, TileHeight, CenterWS, HalfSizeWorldXY);

	// ���� ���� Z�� �����ϰ� �ʹٸ�:
	CenterWS.Z = GridToWorld_TopLeft(
		(GridWidth - 1) / 2.0f, (GridHeight - 1) / 2.0f, TileWidth, TileHeight).Z;

	if (bSpawnGradientPlane)
	{
		SetupGradientPlaneAndMaterial(CenterWS, HalfSizeWorldXY);
	}

	const FVector Center = GridToWorld_TopLeft(
		(GridWidth - 1) / 2.0f, (GridHeight - 1) / 2.0f, TileWidth, TileHeight);
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

	/*if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		NavSys->Build();
	}*/

	USOWGameInstance* GI = Cast<USOWGameInstance>(GetGameInstance());
	GI->SetWorldTileSize(TileWidth);
}

FVector ASOWTileSpawner::GridToWorld_TopLeft(int32 GX, int32 GY, float InTileW, float InTileH) const
{
	const FVector Origin = GetActorLocation();     
	const FVector Right = GetActorForwardVector();
	const FVector Down = GetActorRightVector(); 

	return Origin + Right * (GX * InTileW) + Down * (GY * InTileH);
}

void ASOWTileSpawner::SpawnIncomingRoutes()
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
		
		SpawnedEnemyRoutes.Add(Route);

	}
}

TArray<TObjectPtr<AEnemyIncomingRoute>> ASOWTileSpawner::GetSpawnedEnemyRoutes() const
{
	return SpawnedEnemyRoutes;
}


// === ��� �ڵ� ���� + ��Ƽ���� �Ķ���� ���� ===
// �����(.h)�� �Բ� ����:
// void SetupGradientPlaneAndMaterial(const FVector& CenterWS, float MapW, float MapH);
void ASOWTileSpawner::SetupGradientPlaneAndMaterial(const FVector& CenterWS, const FVector2D& HalfSizeWorldXY)
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

	const FRotator PlaneRot = GetActorRotation() + FRotator(0.f, 45.f, 0.f); // ���̾� ��
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

	// ��� ��ǥ ũ�� = �� + ���� Softness ����
	const float TargetW = HalfSizeWorldXY.X * 5.f + 2.f * GradientSoftnessWorld;
	const float TargetH = HalfSizeWorldXY.Y * 5.f + 2.f * GradientSoftnessWorld;

	const FVector SMSize = GradientPlaneMesh->GetBounds().BoxExtent * 2.f; // ����
	const FVector NewScale(
		TargetW / FMath::Max(1.f, SMSize.X),
		TargetH / FMath::Max(1.f, SMSize.Y),
		1.f
	);
	PlaneMC->SetWorldScale3D(NewScale);

	// MID ���� + �Ķ����
	UMaterialInstanceDynamic* Dyn =
		PlaneMC->CreateAndSetMaterialInstanceDynamicFromMaterial(0, GradientMaterial);
	if (!Dyn) { UE_LOG(LogTemp, Error, TEXT("Failed to create MID for GradientPlane.")); return; }

	// ��Ƽ���� ���ο��� World��Local + ObjectScale ���� ��� ����
	Dyn->SetVectorParameterValue(TEXT("HalfSizeWorld"),
		FLinearColor(HalfSizeWorldXY.X, HalfSizeWorldXY.Y, 0, 0));
	Dyn->SetScalarParameterValue(TEXT("SoftnessWorld"), GradientSoftnessWorld);
	Dyn->SetVectorParameterValue(TEXT("MapCenterWS"), FLinearColor(CenterWS.X, CenterWS.Y, CenterWS.Z, 0));


}


// ������ Ÿ�� ��ġ��(�߽�)�κ��� ���� �� ��踦 ���
void ASOWTileSpawner::ComputeMapBoundsFromTiles(
	const TMap<int32, FVector>& TileCenters,
	float TileWidth, float TileHeight,
	FVector& OutCenterWS,            // XY �߽� (Z�� ȣ��ο��� ���ϴ� ������ ����)
	FVector2D& OutHalfSizeWorldXY)   // XY �ݰ� (���� ����)
{
	if (TileCenters.Num() == 0) { OutCenterWS = FVector::ZeroVector; OutHalfSizeWorldXY = FVector2D::ZeroVector; return; }

	float minX = FLT_MAX, maxX = -FLT_MAX;
	float minY = FLT_MAX, maxY = -FLT_MAX;

	for (const auto& It : TileCenters)
	{
		const FVector& P = It.Value;   // Ÿ�� �߽� ��ǥ
		minX = FMath::Min(minX, P.X);  maxX = FMath::Max(maxX, P.X);
		minY = FMath::Min(minY, P.Y);  maxY = FMath::Max(maxY, P.Y);
	}

	const float halfTileX = TileWidth * 0.5f;   // �ܰ� Ÿ���� ���ʱ��� ����
	const float halfTileY = TileHeight * 0.5f;

	OutCenterWS.X = 0.5f * (minX + maxX);
	OutCenterWS.Y = 0.5f * (minY + maxY);

	OutHalfSizeWorldXY.X = 0.5f * (maxX - minX) + halfTileX;
	OutHalfSizeWorldXY.Y = 0.5f * (maxY - minY) + halfTileY;
}

bool ASOWTileSpawner::ResolveMapScriptPath(const FString& Input, FString& OutAbsPath)
{
	const FString BaseName = FPaths::GetCleanFilename(Input);

	const FString NameNoExt = FPaths::GetBaseFilename(BaseName, false);
	const FString FinalName = NameNoExt + TEXT(".ini");

	OutAbsPath = FPaths::ConvertRelativePathToFull(
		FPaths::ProjectContentDir() / TEXT("04Scripts/Map") / FinalName
	);

	return FPaths::FileExists(OutAbsPath);
}

bool ASOWTileSpawner::LoadAndBuildFromScript(const FString& InFilePathRelOrAbs)
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
			FVector SpawnLocation = GridToWorld_TopLeft(X, Y, Spec.TileWidth, Spec.TileHeight);
			SpawnLocation.Z = GetActorLocation().Z;

			UE_LOG(LogTemp, Warning, TEXT("[Spawn] X=%d Y=%d Loc=%s  TileW=%.1f TileH=%.1f"),
				X, Y, *SpawnLocation.ToString(), Spec.TileWidth, Spec.TileHeight);
		
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
			AActor* SpawnedTile = GetWorld()->SpawnActor<AActor>(TileClass, SpawnLocation, FRotator::ZeroRotator);
			if (!SpawnedTile) { continue; }

			SpawnedTileActors.Add(SpawnedTile);
			SpawnedTileLocations.Add(Index, SpawnLocation);

			ApplyArcheTypeIfSet(SpawnedTile, X, Y, UseToken, Spec.TileWidth);

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

void ASOWTileSpawner::ClearSpawnedTiles()
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

void ASOWTileSpawner::ClearSpawnedRoutes()
{
	for (AEnemyIncomingRoute* R : SpawnedEnemyRoutes)
	{
		if (IsValid(R)) { R->Destroy(); }
	}
	SpawnedEnemyRoutes.Reset();
}

ETileRole ASOWTileSpawner::ResolveRoleFromToken(const FName& Token) const
{
	if (Token.IsNone()) return ETileRole::Empty;
	auto Eq = [&](const TCHAR* S) { return Token.IsEqual(FName(S), ENameCase::IgnoreCase);  };

	if (Eq(TEXT("PATH")) || Eq(TEXT("P"))) return ETileRole::Path;
	if (Eq(TEXT("TURRET1")) || Eq(TEXT("B"))) return ETileRole::Buildable;
	if (Eq(TEXT("TURRET2")) || Eq(TEXT("B"))) return ETileRole::Buildable;
	if (Eq(TEXT("TURRET3")) || Eq(TEXT("B"))) return ETileRole::Buildable;
	if (Eq(TEXT("TURRET4")) || Eq(TEXT("B"))) return ETileRole::Buildable;
	if (Eq(TEXT("BLOCKED")) || Eq(TEXT("X"))) return ETileRole::Blocked;
	if (Eq(TEXT("SPAWN")) || Eq(TEXT("S"))) return ETileRole::Spawn;
	if (Eq(TEXT("RUNE")) || Eq(TEXT("G"))) return ETileRole::Goal;

	return ETileRole::Empty;

}

void ASOWTileSpawner::ApplyArcheTypeIfSet(
	AActor* SpawnedTile,
	int32 X, int32 Y,
	const FName& UseToken,
	float InTileWidth)
{
	if (!SpawnedTile) return;

	if (ATileBase* TB = Cast<ATileBase>(SpawnedTile))
	{
		TB->GridX = X;
		TB->GridY = Y;
		TB->TileSizeUU = InTileWidth;

		const ETileRole R = ResolveRoleFromToken(UseToken);
		if (R != ETileRole::Empty)
		{
			TB->SetRole(R);
		}

		if (bUseArcheTypeAfterSpawn)
		{
			if (const TSoftObjectPtr<UTileArcheType>* Ptr = TokenToArcheType.Find(UseToken))
			{
				if (Ptr && !Ptr->IsNull())
				{
					if (UTileArcheType* Data = Ptr->LoadSynchronous())
					{
						TB->ConfigureFromData(Data);
					}
				}
			}
		}

		TB->ApplyZPolicy();
		TB->ApplyVisualTransform();
	}
}