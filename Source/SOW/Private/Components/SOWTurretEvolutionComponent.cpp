// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SOWTurretEvolutionComponent.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Components/UI/SOWTurretUIComponent.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "Engine/CurveTable.h"

// Sets default values for this component's properties
USOWTurretEvolutionComponent::USOWTurretEvolutionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USOWTurretEvolutionComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedOwnerCharacter = Cast<ASOWCharacterTurretBase>(GetOwner());
	// ...
	

	
	if (UDataTable* EvolutionData = LoadObject<UDataTable>(nullptr, TEXT("/Game/01Blueprints/DataTable/TurretEvolutionData.TurretEvolutionData"))) {
		FName TurretName = CachedOwnerCharacter->GetTurretName();
		EvolutionDataRow = EvolutionData->FindRow<FTurretEvolutionData>(TurretName, TEXT(""));
		InitEvolutionItems();
	}
	USOWTurretUIComponent* UIComponent = Cast< USOWTurretUIComponent>(CachedOwnerCharacter->GetCharacterUIComponent());
	UIComponent->OnTryToEvolveWith.AddDynamic(this, &USOWTurretEvolutionComponent::TryEvolution);
}

void USOWTurretEvolutionComponent::TryEvolution(EEvolutionType Type)
{
	
	

	if (USOWAbilitySystemComponent* ASC = CachedOwnerCharacter->GetSOWAbilitySystemComponent()) {
		
		if (!EvolutionDataRow || EvolutionLevel >= 4) {
			return;
		}

		UCurveTable* PriceData = LoadObject<UCurveTable>(nullptr, TEXT("/Game/01Blueprints/Turret/CT_TurretEvolutionPricesByRank.CT_TurretEvolutionPricesByRank"));
		UCurveTable* PercentData = LoadObject<UCurveTable>(nullptr, TEXT("/Game/01Blueprints/Turret/CT_TurretEvolutionPercenttage.CT_TurretEvolutionPercenttage"));
		if (!PriceData || !PercentData) return;

		FName TRank = CachedOwnerCharacter->GetTurretRank();
		FGameplayTag ETag = CachedOwnerCharacter->GetTurretElementTag();
		FRealCurve* FoundCurve_Price = PriceData->FindCurve(TRank, TEXT(""));
		FRealCurve* FoundCurve_Percent = PriceData->FindCurve(TRank, TEXT(""));

		if (!FoundCurve_Price || !FoundCurve_Percent) return;

		float PriceValue = FoundCurve_Price->Eval(EvolutionLevel + 1);
		float PercentValue = FoundCurve_Percent->Eval(EvolutionLevel + 1);

		if (!USOWBlueprintFunctionLibrary::QueryForCurrencyCountSufficient(CachedOwnerCharacter, ETag, PriceValue)) {
			UE_LOG(LogTemp, Warning, TEXT("Not Enough Currency"));
			return;
		}

		FRandomStream FS;

		if (FS.RandRange(1, 100) > PercentValue) {
			// Notify about failed evolution
			return;
		}

		// executes after all conditions are met
		if (EvolutionState == EEvolutionType::EVO_NONE) {
			EvolutionState = Type;
		}

		TSoftObjectPtr<UDA_TurretEvolutionData> Data;
		
		switch (Type)
		{
			case EEvolutionType::EVO_ALPHA:
				Data = Alpha.EvolutionDataAsset;
				break;
			case EEvolutionType::EVO_BETA:
				Data = Beta.EvolutionDataAsset;
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("Type is invalid"));
				break;
		}

		if (UDA_TurretEvolutionData* LoadData = Data.LoadSynchronous()) {
			LoadData->GiveToAbilitySystemComponent(ASC);
			EvolutionLevel++;
			InitEvolutionItems();
		}
	}
}

void USOWTurretEvolutionComponent::GetTurretEvolutionDescriptions(FString& AlphaDesc, FString& BetaDesc) {
	AlphaDesc = Alpha.EvolutionDescription;
	BetaDesc = Beta.EvolutionDescription;
}

void USOWTurretEvolutionComponent::InitEvolutionItems()
{
	if (EvolutionLevel >= EvolutionDataRow->EvolutionAlpha.Num()) {
		Alpha.EvolutionDataAsset = nullptr;
		Alpha.EvolutionDescription = "Evolution Complete";

		Beta.EvolutionDataAsset = nullptr;
		Beta.EvolutionDescription = "Evolution Complete";
	}
	else {
		Alpha = EvolutionDataRow->EvolutionAlpha[EvolutionLevel];
		Beta = EvolutionDataRow->EvolutionBeta[EvolutionLevel];
	}
	
}
