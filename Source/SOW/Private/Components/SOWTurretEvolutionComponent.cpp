// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SOWTurretEvolutionComponent.h"
#include "Characters/Turrets/SOWCharacterTurretBase.h"
#include "Components/UI/SOWTurretUIComponent.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "GameModes/WaveGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "Engine/CurveTable.h"
#include "AbilitySystemBlueprintLibrary.h"

// Sets default values for this component's properties
USOWTurretEvolutionComponent::USOWTurretEvolutionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void USOWTurretEvolutionComponent::GetPropertyDescriptString(FString& OutString)
{
	if (EvolutionPropertyLevel >= PropertyMaxLevel) {
		OutString = TEXT("강화 완료");
	}
	else if (PropertyData.Num() == 0) {
		OutString = TEXT("강화 불가");
	}
	else {


		OutString = PropertyData[EvolutionPropertyLevel].EvolutionDescription;
		OutString.ReplaceInline(TEXT("\\n"), TEXT("\n"));
	}
	
}

void USOWTurretEvolutionComponent::GetPropertyResourceString(FString& OutCurrency, FString& OutPercentage)
{
	//checkf(PropertyResourceData, TEXT("PropertyResourceData not Assigned. Please check DataAsset"));
	if (!PropertyResourceData) return;
	if (EvolutionPropertyLevel >= PropertyMaxLevel) {
		OutCurrency = "Max"; OutPercentage = ""; return;
	}
	if (PropertyData.Num() == 0) {
		OutCurrency = "Disabled"; OutPercentage = ""; return;
	}

	FRealCurve* FoundCurve_Price = PropertyResourceData->FindCurve("Currency", TEXT(""));
	FRealCurve* FoundCurve_Percent = PropertyResourceData->FindCurve("Prob", TEXT(""));

	float PriceValue = FoundCurve_Price->Eval(EvolutionPropertyLevel + 1);
	float PercentValue = FoundCurve_Percent->Eval(EvolutionPropertyLevel + 1);

	if (PercentValue != 0) {
		OutCurrency = FString::FromInt(PriceValue);
		OutPercentage = TEXT("강화 확률") " : " + FString::FromInt(PercentValue) + "%";
	}
	else {
		OutCurrency = "-";
		OutPercentage = TEXT("강화 불가");
	}


}
	

void USOWTurretEvolutionComponent::GetStatusResourceString(FString& OutCurrency, FString& OutPercentage)
{
	if (!StatusResourceData) return;
	if (EvolutionStatusLevel >= StatusMaxLevel) {
		OutCurrency = "Max"; OutPercentage = ""; return;
	}
	if (StatusMaxLevel == 0) {
		OutCurrency = "Disabled"; OutPercentage = ""; return;
	}
	//checkf(StatusResourceData, TEXT("StatusResourceData not Assigned. Please check DataAsset"));
	FRealCurve* FoundCurve_Price = StatusResourceData->FindCurve("Currency", TEXT(""));
	FRealCurve* FoundCurve_Percent = StatusResourceData->FindCurve("Prob", TEXT(""));

	float PriceValue = FoundCurve_Price->Eval(EvolutionStatusLevel + 1);
	float PercentValue = FoundCurve_Percent->Eval(EvolutionStatusLevel + 1);

	OutCurrency = FString::FromInt(PriceValue);
	OutPercentage = TEXT("강화 확률") " : "+ FString::FromInt(PercentValue) + "%";
}

void USOWTurretEvolutionComponent::GetStatusNextValueString(FString& OutAtk, FString& OutSpd)
{
	if (!StatusResourceData) return;
	if (EvolutionStatusLevel >= StatusMaxLevel) {
		OutAtk = ""; OutSpd = ""; return;
	}
	if (StatusMaxLevel == 0) {
		OutAtk = ""; OutSpd = ""; return;
	}
	//checkf(StatusResourceData, TEXT("StatusResourceData not Assigned. Please check DataAsset"));
	FRealCurve* FoundCurve_Atk = StatusResourceData->FindCurve("AttackUp", TEXT(""));
	FRealCurve* FoundCurve_Spd = StatusResourceData->FindCurve("SpeedUp", TEXT(""));

	float AtkValue = FoundCurve_Atk->Eval(EvolutionStatusLevel + 1);
	float SpdValue = FoundCurve_Spd->Eval(EvolutionStatusLevel + 1);

	OutAtk = "+" + FString::FromInt(AtkValue);
	OutSpd = "+" + FString::SanitizeFloat(SpdValue);

}

TArray<FString> USOWTurretEvolutionComponent::GetAllPropertyDescriptString()
{
	if (PropertyData.Num() == 0) return TArray<FString>();

	TArray<FString> StringCont;
	for (int i = 0; i < PropertyMaxLevel; i++) {
		FString NewStr = PropertyData[i].EvolutionDescription;
		NewStr.ReplaceInline(TEXT("\\n"), TEXT("\n"));

		StringCont.Add(NewStr);
	}
	return StringCont;
}

// Called when the game starts
void USOWTurretEvolutionComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedOwnerCharacter = Cast<ASOWCharacterTurretBase>(GetOwner());
	if (!CachedOwnerCharacter) {
		UE_LOG(LogTemp, Error, TEXT("Turret is Invalid"));
	}

	FName TurretName = CachedOwnerCharacter->GetTurretName();
	if (UDataTable* EvolutionData = LoadObject<UDataTable>(nullptr, TEXT("/Game/01Blueprints/DataTable/TurretEvolutionInfo.TurretEvolutionInfo"))) {

		FEvolutionData* EvolutionDataRow = EvolutionData->FindRow<FEvolutionData>(TurretName, TEXT(""));

		checkf(EvolutionDataRow, TEXT("No RowData found from TurretEvolutionInfo. Please check RowName : %s"), *TurretName.ToString());

		AlphaData = EvolutionDataRow->EvolutionStatusEffectA;
		BetaData = EvolutionDataRow->EvolutionStatusEffectB;
		PropertyData = EvolutionDataRow->EvolutionPropertyArray;
		StatusMaxLevel = EvolutionDataRow->StatusMaxLevel;

		UE_LOG(LogTemp, Warning, TEXT("[EvolutionComponent] %s : EvolutionData Loading Complete."), *TurretName.ToString());

		PropertyMaxLevel = PropertyData.Num();
	}
	

	if (UDataTable* ResourceData = LoadObject<UDataTable>(nullptr, TEXT("/Game/01Blueprints/DataTable/TurretEvolutionResourceCurve.TurretEvolutionResourceCurve"))) {
		
		FEvolutionResourceData* ResourceDataRow = ResourceData->FindRow<FEvolutionResourceData>(TurretName, TEXT(""));

		checkf(ResourceDataRow, TEXT("No RowData found from TurretEvolutionInfo. Please check RowName : %s"), *TurretName.ToString());
		PropertyResourceData = ResourceDataRow->PropertyResourceData.LoadSynchronous();
		StatusResourceData = ResourceDataRow->StatusResourceData.LoadSynchronous();

		checkf(PropertyResourceData, TEXT("No PropertyResourceData found from TurretEvolutionInfo. Please check RowName : %s"), *TurretName.ToString());
		checkf(StatusResourceData, TEXT("No StatusResourceData found from TurretEvolutionInfo. Please check RowName : %s"), *TurretName.ToString());


		UE_LOG(LogTemp, Warning, TEXT("[EvolutionComponent] %s : EvolutionResourceData Loading Complete."), *TurretName.ToString());
	}
	
	USOWTurretUIComponent* UIComponent = Cast< USOWTurretUIComponent>(CachedOwnerCharacter->GetCharacterUIComponent());

	if(!UIComponent) {
		UE_LOG(LogTemp, Error, TEXT("UIComponent is Invalid"));
	}
	UIComponent->OnTryToEvolveWith.AddDynamic(this, &USOWTurretEvolutionComponent::TryEvolution);
}

void USOWTurretEvolutionComponent::TryEvolution(EEvolutionType Type)
{
	//UE_LOG(LogTemp, Warning, TEXT("TryEvolution"));

	USOWAbilitySystemComponent* ASC = CachedOwnerCharacter->GetSOWAbilitySystemComponent();
	if (!ASC) return;
	FGameplayEffectContextHandle CH;
	FGameplayEffectSpecHandle ESH;

	if (!CheckResourceAndProb(Type)) {
		// 강화 실패 시 UI Floating을 대비
		UE_LOG(LogTemp, Error, TEXT("Evolution Failed"));
		
		return;
	} 
	switch (Type)
	{
		// A,B의 경우 커브 테이블 값이 1부터 시작하므로 미리 증가시킨 뒤 수행
	case EEvolutionType::EVO_ALPHA:
		ESH = ASC->MakeOutgoingSpec(AlphaData, ++EvolutionStatusLevel, CH);
		ASC->ApplyGameplayEffectSpecToSelf(*ESH.Data.Get());

		if(OnAlphaEvolutionSucceed.IsBound())
			OnAlphaEvolutionSucceed.Broadcast(true);

		CurrencySpentForStat = 0;
		break;
	case EEvolutionType::EVO_BETA:
		ESH = ASC->MakeOutgoingSpec(BetaData, ++EvolutionStatusLevel, CH);
		ASC->ApplyGameplayEffectSpecToSelf(*ESH.Data.Get());

		if (OnBetaEvolutionSucceed.IsBound())
			OnBetaEvolutionSucceed.Broadcast(true);

		CurrencySpentForStat = 0;
		break;
	case EEvolutionType::EVO_PROP:
		// P의 경우 배열 인덱스가 0부터 시작하므로 받은 뒤 증가
		if (UDA_TurretEvolutionData* Data = PropertyData[EvolutionPropertyLevel].EvolutionDataAsset.LoadSynchronous()) {
			Data->GiveToAbilitySystemComponent(ASC);

			if (OnPropEvolutionSucceed.IsBound())
				OnPropEvolutionSucceed.Broadcast(true);

			CurrencySpentForProp = 0;
			EvolutionPropertyLevel++;
		}
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Invalid Evolution Type"));
		break;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CachedOwnerCharacter, SOWGameplayTags::Turret_Event_EvolutionSuccessed, FGameplayEventData());
	//UE_LOG(LogTemp, Wa, TEXT("Evolution Successed"));
}

void USOWTurretEvolutionComponent::ForceEvolution(EEvolutionType Type)
{
	// 일반적으로 사용하지 않는 함수입니다.
	// Glocio 전용으로 표기를 목적으로 생성된 함수입니다.

	USOWAbilitySystemComponent* ASC = CachedOwnerCharacter->GetSOWAbilitySystemComponent();
	if (!ASC) return;
	FGameplayEffectContextHandle CH;
	FGameplayEffectSpecHandle ESH;

	switch (Type)
	{
		// A,B의 경우 커브 테이블 값이 1부터 시작하므로 미리 증가시킨 뒤 수행
	case EEvolutionType::EVO_ALPHA:
		ESH = ASC->MakeOutgoingSpec(AlphaData, ++EvolutionStatusLevel, CH);
		ASC->ApplyGameplayEffectSpecToSelf(*ESH.Data.Get());

		if (OnAlphaEvolutionSucceed.IsBound())
			OnAlphaEvolutionSucceed.Broadcast(true);

		CurrencySpentForStat = 0;
		break;
	case EEvolutionType::EVO_BETA:
		ESH = ASC->MakeOutgoingSpec(BetaData, ++EvolutionStatusLevel, CH);
		ASC->ApplyGameplayEffectSpecToSelf(*ESH.Data.Get());

		if (OnBetaEvolutionSucceed.IsBound())
			OnBetaEvolutionSucceed.Broadcast(true);

		CurrencySpentForStat = 0;
		break;
	case EEvolutionType::EVO_PROP:
		// P의 경우 배열 인덱스가 0부터 시작하므로 받은 뒤 증가
		if (UDA_TurretEvolutionData* Data = PropertyData[EvolutionPropertyLevel].EvolutionDataAsset.LoadSynchronous()) {
			Data->GiveToAbilitySystemComponent(ASC);

			if (OnPropEvolutionSucceed.IsBound())
				OnPropEvolutionSucceed.Broadcast(true);

			CurrencySpentForProp = 0;
			EvolutionPropertyLevel++;
		}
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Invalid Evolution Type"));
		break;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CachedOwnerCharacter, SOWGameplayTags::Turret_Event_EvolutionSuccessed, FGameplayEventData());
}

void USOWTurretEvolutionComponent::RollbackForceEvolution(EEvolutionType Type)
{
	// 일반적으로 사용하지 않는 함수입니다.
	// Glocio 전용으로 표기를 목적으로 생성된 함수입니다.

	USOWAbilitySystemComponent* ASC = CachedOwnerCharacter->GetSOWAbilitySystemComponent();
	if (!ASC) return;

	switch (Type)
	{
		case EEvolutionType::EVO_PROP:
			if (OnPropEvolutionSucceed.IsBound())
				OnPropEvolutionSucceed.Broadcast(true);

			EvolutionPropertyLevel--;
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Invalid Evolution Type"));
			break;
	}
}

bool USOWTurretEvolutionComponent::CheckResourceAndProb(EEvolutionType Type)
{

	AWaveGameMode* GM = Cast<AWaveGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (Type == EEvolutionType::EVO_PROP) {
		checkf(PropertyResourceData, TEXT("PropertyResourceData not Assigned. Please check DataAsset"));
		if (EvolutionPropertyLevel >= PropertyMaxLevel) return false; // Max LV : 4

		FRealCurve* FoundCurve_Price = PropertyResourceData->FindCurve("Currency", TEXT(""));
		FRealCurve* FoundCurve_Percent = PropertyResourceData->FindCurve("Prob", TEXT(""));

		float PriceValue = FoundCurve_Price->Eval(EvolutionPropertyLevel + 1);
		float PercentValue = FoundCurve_Percent->Eval(EvolutionPropertyLevel + 1);

		if (PercentValue == 0) return false;

		if (!USOWBlueprintFunctionLibrary::QueryForCurrencyCountSufficient(this, FGameplayTag::RequestGameplayTag("Shared.Element.Nature"), PriceValue)) {
			UE_LOG(LogTemp, Error, TEXT("PriceValue Condition Failed"));
			GM->AlertNotEnoughCurrency();
			//CurrencySpentForProp += PriceValue;
			return false;
		}
		int32 value = FMath::RandRange(1, 100);
		if (value > PercentValue) {
			if (OnPropEvolutionSucceed.IsBound())
				OnPropEvolutionSucceed.Broadcast(false);
			UE_LOG(LogTemp, Error, TEXT("PercentValue Condition Failed : %s"), *FString::FromInt(value));
			CurrencySpentForProp += PriceValue;
			return false;
		}
	}
	else {
		checkf(StatusResourceData, TEXT("PropertyResourceData not Assigned. Please check DataAsset"));
		if (EvolutionStatusLevel >= StatusMaxLevel) return false; // Max LV : 10

		FRealCurve* FoundCurve_Price = StatusResourceData->FindCurve("Currency", TEXT(""));
		FRealCurve* FoundCurve_Percent = StatusResourceData->FindCurve("Prob", TEXT(""));

		float PriceValue = FoundCurve_Price->Eval(EvolutionStatusLevel + 1);
		float PercentValue = FoundCurve_Percent->Eval(EvolutionStatusLevel + 1);

		if (!USOWBlueprintFunctionLibrary::QueryForCurrencyCountSufficient(this, FGameplayTag::RequestGameplayTag("Shared.Element.Nature"), PriceValue)) {
			UE_LOG(LogTemp, Warning, TEXT("PriceValue Condition Failed"));
			GM->AlertNotEnoughCurrency();
			return false;
		}

		int32 value = FMath::RandRange(1, 100);
		if (value > PercentValue) {

			if (OnBetaEvolutionSucceed.IsBound())
				OnBetaEvolutionSucceed.Broadcast(false);
			UE_LOG(LogTemp, Warning, TEXT("PercentValue Condition Failed : %s"), *FString::FromInt(value));
			CurrencySpentForStat += PriceValue;
			return false; 
		}
	}




	return true;
}

void USOWTurretEvolutionComponent::MakeAndSendEvolutionLog()
{
}



