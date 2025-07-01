// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretPreviewActor.generated.h"

UCLASS()
class SOW_API ATurretPreviewActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Decal")
	class UDecalComponent* InstallationRangeDecal;
	
public:	
	// Sets default values for this actor's properties
	ATurretPreviewActor();

	void SetCanPlace(bool bPlaceable);

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* ValidMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* InvalidMaterial;

protected:
	UPROPERTY()
	UMaterialInterface* PreviewBaseMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPreviewActor(USkeletalMesh* NewMesh, float AttackRange);

};
