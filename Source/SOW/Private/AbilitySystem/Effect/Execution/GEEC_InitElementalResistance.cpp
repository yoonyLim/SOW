// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Execution/GEEC_InitElementalResistance.h"
#include "Engine/DataTable.h"
#include "SOWStructTypes.h"
#include "AbilitySystem/SOWAttributeSet.h"

struct FAttributeCapturesElement {
	DECLARE_ATTRIBUTE_CAPTUREDEF(NatureResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FlameResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DeathResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IceResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(WaveResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DivinityResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MadnessResistance);

	FAttributeCapturesElement() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, NatureResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, FlameResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, DeathResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, IceResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, WaveResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, DivinityResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOWAttributeSet, MadnessResistance, Target, false);

	}
};


static const FAttributeCapturesElement& GetCapturedProperties() {
	static FAttributeCapturesElement AttributeCaptures;
	return AttributeCaptures;
}
TArray<FGameplayTag> UGEEC_InitElementalResistance::GetMatchedElementTags(const FGameplayTagContainer* SourceTags) const
{
	// Find All Tags matched with RootTag : Shared.Element in SourTags.
	TArray<FGameplayTag> MatchedElementTags;

	FGameplayTag RootTag = FGameplayTag::RequestGameplayTag(FName("Shared.Element"));

	if (SourceTags)
	{
		for (const FGameplayTag& Tag : *SourceTags)
		{
			if (Tag.MatchesTag(RootTag)) // "Shared.Element" 하위 태그 포함 필터
			{
				MatchedElementTags.Add(Tag);
			}
		}
	}

	return MatchedElementTags;
}
FElementResistanceData* UGEEC_InitElementalResistance::GetResistanceDataRow(const TArray<FGameplayTag>& MatchedElementTags) const
{
	// Retrieves RowData matching key properties from a DataTable : ElementalResistanceData.

	FElementResistanceData* ResistanceDataRow = nullptr;
	for (const FGameplayTag& ElementTag : MatchedElementTags)
	{
		FString TagString = ElementTag.ToString();

		TArray<FString> TagParts;
		TagString.ParseIntoArray(TagParts, TEXT("."));

		FString ExtractedTag = TagParts[2];
		FName NameValue(*ExtractedTag);

		if (TagParts.Num() > 3 && TagParts[3] != TEXT("Major")) continue;

		ResistanceDataRow = ResistanceData->FindRow<FElementResistanceData>(NameValue, TEXT(""));
	}
	if (!ResistanceDataRow) {
		UE_LOG(LogTemp, Warning, TEXT("No Active Element Tag Found."));
	}
	return ResistanceDataRow;
}
UGEEC_InitElementalResistance::UGEEC_InitElementalResistance()
{
	FString CharacterDataPath = TEXT("DataTable'/Game/01Blueprints/DataTable/ElementalResistanceData.ElementalResistanceData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MYCHARACTER(*CharacterDataPath);
	if (DT_MYCHARACTER.Succeeded()) {
		ResistanceData = DT_MYCHARACTER.Object;
	}
	else {
		ResistanceData = nullptr;
	}

	RelevantAttributesToCapture.Add(GetCapturedProperties().NatureResistanceDef);
	RelevantAttributesToCapture.Add(GetCapturedProperties().FlameResistanceDef);
	RelevantAttributesToCapture.Add(GetCapturedProperties().DeathResistanceDef);
	RelevantAttributesToCapture.Add(GetCapturedProperties().IceResistanceDef);
	RelevantAttributesToCapture.Add(GetCapturedProperties().WaveResistanceDef);
	RelevantAttributesToCapture.Add(GetCapturedProperties().DivinityResistanceDef);
	RelevantAttributesToCapture.Add(GetCapturedProperties().MadnessResistanceDef);
}

void UGEEC_InitElementalResistance::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	check(ResistanceData);

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();

	const TArray<FGameplayTag> MatchedElementTags = GetMatchedElementTags(SourceTags);
	if (MatchedElementTags.IsEmpty()) return;

	const FElementResistanceData* ResistanceDataRow = GetResistanceDataRow(MatchedElementTags);
	if (!ResistanceDataRow) return;

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedProperties().NatureResistanceDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->Nature)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedProperties().FlameResistanceDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->Flame)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedProperties().WaveResistanceDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->Wave)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedProperties().IceResistanceDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->Ice)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedProperties().DeathResistanceDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->Death)
	);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedProperties().DivinityResistanceDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->Divinity)
	);
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			GetCapturedProperties().MadnessResistanceDef.AttributeToCapture,
			EGameplayModOp::Override,
			ResistanceDataRow->Madness)
	);
}
