// SOWCharacterCoreRune.h

#pragma once

#include "CoreMinimal.h"
#include "Characters/SOWCharacter.h"
#include "SOWCharacterCoreRune.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCoreDestroyed);

class UWidgetComponent;
class USOWAttributeSet;

UCLASS()
class SOW_API ASOWCharacterCoreRune : public ASOWCharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> RuneHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Shake", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> HitCameraShake;
	
	bool bIsDead = false;

public:
	ASOWCharacterCoreRune();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	// === Health / UI ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent *HealthBarWidget;

	// EnemyBase처럼 일정 시간 후 숨기고 싶을 때 사용 (옵션)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	bool bShouldKeepHealthbarOn = true;

	FTimerHandle HideHealthBarHandle;

	// ASC에서 가져오는 실제 속성 컨테이너
	UPROPERTY()
	const USOWAttributeSet *ASCAttributes;

	// 체력 변화 콜백 (ASC 바인딩)
	void OnHealthChanged(const FOnAttributeChangeData& Data);

	void OnShieldChanged(const FOnAttributeChangeData& Data);

	// 체력바 퍼센트 갱신
	virtual void UpdateHealthBarValue(float NewHealth, float MaxHealth);

public:
	// 코어 파괴 알림 (게임오버 트리거 등 외부에서 바인딩해서 사용)
	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnCoreDestroyed OnCoreDestroyed;

	// 체력이 0 이하일 때 호출되는 처리(게임 오버 신호 보내기 등)
	UFUNCTION(BlueprintCallable, Category = "Core")
	void HandleCoreDestroyed();
};
