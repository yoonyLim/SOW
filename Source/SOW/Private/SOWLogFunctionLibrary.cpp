// Fill out your copyright notice in the Description page of Project Settings.


#include "SOWLogFunctionLibrary.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "Engine/GameInstance.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"

DEFINE_LOG_CATEGORY(LogWave);
DEFINE_LOG_CATEGORY(LogDamage1);
DEFINE_LOG_CATEGORY(LogTurret);

void USOWLogFunctionLibrary::LogWaveToCSV(
		EElementalType Type,
		int32 Amount,
		const FName& Source,
		const FString& Context
		)
{
	// 콘솔/에디터 로그로도 확인하고 싶으면:
	UE_LOG(LogWave, Display, TEXT("Gain %d of %d (Source=%s) %s"),
		Amount, static_cast<int32>(Type), *Source.ToString(), *Context);

	const FString Now = FDateTime::Now().ToIso8601();
	const FString Line = FString::Printf(
		TEXT("%s,%d,%d,%s,%s"),
		*Now,
		static_cast<int32>(Type),
		Amount,
		*Source.ToString(),
		*Context.Replace(TEXT(","), TEXT(";")) // CSV 안전
	);
	WaveAppendLine(Line);
}

void USOWLogFunctionLibrary::LogDamageToCSV(
		const FString& TurretInstanceID,
		const FString& TurretID,
		const FString& TargetID,
		float BaseDamage,
		float FinalDamage,
		float TargetRemainingHP,
		const FVector& TargetPos,
		int32 Stage,
		AActor* AttackerActor,
		AActor* DamagedActor
)
{
	FString SessionID = GetCurrentSessionID();

	FString SaveDir = FPaths::ProjectSavedDir() / TEXT("DamageLogs");
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*SaveDir))
	{
		PlatformFile.CreateDirectory(*SaveDir);
	}

	FString FileName = SaveDir / FString::Printf(TEXT("Session_%s.csv"), *SessionID);

	// ù ���� �� ��� �߰�
	if (!PlatformFile.FileExists(*FileName))
	{
		FString Header = TEXT("SessionID,Timestamp,TurretInstanceID,TurretID,TargetID,BaseDamage,FinalDamage,TargetRemainingHP,TargetPosX,TargetPosY,TargetPosZ,Stage,BuffTags,EnemyTags\n");
		FFileHelper::SaveStringToFile(Header, *FileName);
	}

	FString Timestamp = FDateTime::Now().ToString(TEXT("%H:%M:%S.%s"));

	FString AttackerTags = GetActiveTagsAsString(AttackerActor);
	FString DamagedTags = GetActiveTagsAsString(DamagedActor);

	FString Line = FString::Printf(
		TEXT("%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%s,%s\n"),
		*SessionID,
		*Timestamp,
		*TurretInstanceID,
		*TurretID,
		*TargetID,
		BaseDamage,
		FinalDamage,
		TargetRemainingHP,
		TargetPos.X, TargetPos.Y, TargetPos.Z,
		Stage,
		*AttackerTags,
		*DamagedTags
	);

	FFileHelper::SaveStringToFile(Line, *FileName, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
};

void USOWLogFunctionLibrary::WaveInitialize()
{
	const FString Dir = FPaths::ProjectSavedDir() / TEXT("WaveLogs");
	IFileManager::Get().MakeDirectory(*Dir, /*Tree*/true);

	const FString Stamp = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
	FilePath = Dir / FString::Printf(TEXT("WaveLog_%s.csv"), *Stamp);

	// 헤더 작성
	WaveAppendLine(TEXT("Timestamp,Type,Amount,Source,Context"));
}

void USOWLogFunctionLibrary::WaveAppendLine(const FString& Line)
{
	FScopeLock Lock(&Mutex);
	FFileHelper::SaveStringToFile(Line + LINE_TERMINATOR, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);

}

FString USOWLogFunctionLibrary::GetActiveTagsAsString(AActor* Actor)
{
	if (!Actor) return TEXT("None");

	if (USOWAbilitySystemComponent* ASC = USOWBlueprintFunctionLibrary::GetSOWAbilitySystemComponentFromActorInfo(Actor))
	{
		FGameplayTagContainer ActiveTags;
		ASC->GetOwnedGameplayTags(ActiveTags);

		if (ActiveTags.Num() == 0)
		{
			return TEXT("None");
		}

		TArray<FString> TagStrings;
		for (const FGameplayTag& Tag : ActiveTags)
		{
			const FString TagName = Tag.ToString();

			if (TagName.StartsWith(TEXT("Turret.Status.Buff")) ||
				TagName.StartsWith(TEXT("Turret.Status.Debuff")) || 
				TagName.StartsWith(TEXT("Enemy.Status.Buff")) ||
				TagName.StartsWith(TEXT("Enemy.Status.Debuff"))||
				TagName.StartsWith(TEXT("Enemy.Status.Debuff.Sine")))
			{
				TagStrings.Add(TagName);
			}
		}

		if (TagStrings.Num() == 0)
		{
			return TEXT("None");
		}

		return FString::Join(TagStrings, TEXT("|"));  // �±� ������: |
	}

	return TEXT("NoASC");
}

FString USOWLogFunctionLibrary::GetCurrentSessionID()
{
	static FString SessionID;
	if (SessionID.IsEmpty())
	{
		// ���� ���� ���� ���� ID ����
		FDateTime Now = FDateTime::Now();
		SessionID = Now.ToString(TEXT("%Y%m%d_%H%M%S"));
	}
	return SessionID;
}