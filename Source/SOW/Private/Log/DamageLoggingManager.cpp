// Fill out your copyright notice in the Description page of Project Settings.


#include "Log/DamageLoggingManager.h"
#include "SOWBlueprintFunctionLibrary.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"

void UDamageLoggingManager::LogDamageToCSV(
    const FString& TurretInstanceID,
    const FString& TurretID,
    const FString& TargetID,
    float BaseDamage,
    float FinalDamage,
    float TargetRemainingHP,
    const FVector& TargetPos,
    int32 Stage,
    AActor* AttackerActor   // 추가
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

    // 첫 실행 시 헤더 추가
    if (!PlatformFile.FileExists(*FileName))
    {
        FString Header = TEXT("SessionID,Timestamp,TurretInstanceID,TurretID,TargetID,BaseDamage,FinalDamage,TargetRemainingHP,TargetPosX,TargetPosY,TargetPosZ,Stage,BuffTags\n");
        FFileHelper::SaveStringToFile(Header, *FileName);
    }

    FString Timestamp = FDateTime::Now().ToString(TEXT("%H:%M:%S.%s"));

    FString AttackerTags = GetActiveTagsAsString(AttackerActor);

    FString Line = FString::Printf(
        TEXT("%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%s\n"),
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
        *AttackerTags
    );

    FFileHelper::SaveStringToFile(Line, *FileName, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
}

FString UDamageLoggingManager::GetActiveTagsAsString(AActor* Actor)
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
				TagName.StartsWith(TEXT("Turret.Status.Debuff")))
			{
				TagStrings.Add(TagName);
			}
		}

		if (TagStrings.Num() == 0)
		{
			return TEXT("None");
		}

		return FString::Join(TagStrings, TEXT("|"));  // 태그 구분자: |
	}

	return TEXT("NoASC");
}

FString UDamageLoggingManager::GetCurrentSessionID()
{
	static FString SessionID;
	if (SessionID.IsEmpty())
	{
		// 실행 시점 기준 세션 ID 생성
		FDateTime Now = FDateTime::Now();
		SessionID = Now.ToString(TEXT("%Y%m%d_%H%M%S"));
	}
	return SessionID;
}
