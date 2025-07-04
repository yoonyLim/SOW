#include "Characters/Turrets/TurretPreviewActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ATurretPreviewActor::ATurretPreviewActor()
{
    PrimaryActorTick.bCanEverTick = true;

    TurretMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TurretMesh"));
    RootComponent = TurretMesh;

    TurretMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/Game/03Materials/M_TurretPreviewTransparent.M_TurretPreviewTransparent"));
    if (Material.Succeeded())
    {
        PreviewBaseMaterial = Material.Object;
    }

    InstallationRangeDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("InstallationRangeDecal"));
    InstallationRangeDecal->SetupAttachment(RootComponent);
    InstallationRangeDecal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalMat(TEXT("/Game/03Materials/M_Range_Decal_Turret.M_Range_Decal_Turret"));
    if (DecalMat.Succeeded())
    {
        InstallationRangeDecal->SetDecalMaterial(DecalMat.Object);
    }

    InstallationRangeDecal->SetVisibility(false);
}

// Called when the game starts or when spawned
void ATurretPreviewActor::BeginPlay()
{
    Super::BeginPlay();
}

void ATurretPreviewActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ATurretPreviewActor::SetCanPlace(bool bPlaceable)
{
    if (!DynamicMaterial || !TurretMesh) return;

    // 모든 머티리얼 슬롯에 색상 적용
    int32 NumMaterials = TurretMesh->GetNumMaterials();
    for (int32 i = 0; i < NumMaterials; ++i)
    {
        TurretMesh->SetMaterial(i, DynamicMaterial);
    }

    DynamicMaterial->SetVectorParameterValue("BaseColor", bPlaceable ? FLinearColor::Green : FLinearColor::Red);
    DynamicMaterial->SetScalarParameterValue("Opacity", 0.4f);
}

void ATurretPreviewActor::SetPreviewActor(USkeletalMesh* NewMesh, float AttackRange)
{
    if (!TurretMesh || !NewMesh) return;

    TurretMesh->SetSkeletalMesh(NewMesh);

    // 메시가 바뀌면 머티리얼도 재적용
    if (PreviewBaseMaterial)
    {
        int32 NumMaterials = TurretMesh->GetNumMaterials();
        for (int32 i = 0; i < NumMaterials; ++i)
        {
            TurretMesh->SetMaterial(i, PreviewBaseMaterial);
        }

        DynamicMaterial = UMaterialInstanceDynamic::Create(PreviewBaseMaterial, this);
    }

    /* Turret Install Material */
    InstallationRangeDecal->DecalSize = FVector(AttackRange, AttackRange, AttackRange);
    InstallationRangeDecal->SetVisibility(true);
}