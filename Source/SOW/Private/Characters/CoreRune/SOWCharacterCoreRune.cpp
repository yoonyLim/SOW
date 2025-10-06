// SOWCharacterCoreRune.cpp

#include "Characters/CoreRune/SOWCharacterCoreRune.h"

// ASC / AttributeSet
#include "AbilitySystem/SOWAttributeSet.h"
#include "AbilitySystem/SOWAbilitySystemComponent.h"

// 위젯
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameModes/WaveGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/Enemy/EnemyHealthBarWidget.h" // 퍼센트 세터 재사용

// Sets default values
ASOWCharacterCoreRune::ASOWCharacterCoreRune()
{
	// 코어는 틱 불필요하면 끄는 걸 권장
	PrimaryActorTick.bCanEverTick = false;

	CharacterType = ESOWCharacterType::CoreRune;

	// === Healthbar Widget 생성 (EnemyBase 패턴 차용) ===
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CoreHealthBar"));
	if (HealthBarWidget)
	{
		HealthBarWidget->SetupAttachment(RootComponent);
		HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
		HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 120.f)); // EnemyBase는 Z=1.0이었음. 코어는 높여서 가독성↑

		// 우선은 적 체력바 위젯을 재사용 (필요 시 Core 전용 위젯으로 교체)
		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass{
			TEXT("/Game/01Blueprints/UI/Enemy/WBP_EnemyHealthBar")};
		if (WidgetClass.Succeeded())
		{
			HealthBarWidget->SetWidgetClass(WidgetClass.Class);
		}
	}
}

// Called when the game starts or when spawned
void ASOWCharacterCoreRune::BeginPlay()
{
	Super::BeginPlay();

	// 기본적으로 숨길지 여부
	if (!bShouldKeepHealthbarOn && HealthBarWidget)
	{
		HealthBarWidget->SetHiddenInGame(true);
	}

	// === ASC 바인딩 (EnemyBase 동일 패턴) ===
	if (AbilitySystemComponent) // EnemyBase도 ASC 델리게이트를 이렇게 바인딩합니다 ()
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
								  USOWAttributeSet::GetCurrentHealthAttribute())
			.AddUObject(this, &ASOWCharacterCoreRune::OnHealthChanged);
	}

	// AttributeSet 포인터 캐싱 (EnemyBase 동일 패턴, 이후 MaxHealth 조회에 사용, )
	ASCAttributes = AbilitySystemComponent
						? Cast<USOWAttributeSet>(AbilitySystemComponent->GetAttributeSet(USOWAttributeSet::StaticClass()))
						: nullptr;

	// 초기 체력바 값 세팅 (EnemyBase에서도 BeginPlay 끝부분에서 초기화, )
	if (ASCAttributes)
	{
		const float MaxHealth = ASCAttributes->GetMaxHealthBase();
		UpdateHealthBarValue(MaxHealth, MaxHealth);
	}
}

// 체력 변화 시 호출
void ASOWCharacterCoreRune::OnHealthChanged(const FOnAttributeChangeData &Data)
{
	// EnemyBase와 동일하게 자동 숨김 타이머 운영 ()
	if (GetWorldTimerManager().IsTimerActive(HideHealthBarHandle))
	{
		GetWorldTimerManager().ClearTimer(HideHealthBarHandle);
	}

	if (!bShouldKeepHealthbarOn && HealthBarWidget)
	{
		GetWorldTimerManager().SetTimer(
			HideHealthBarHandle,
			FTimerDelegate::CreateLambda([this]()
										 {
				if (HealthBarWidget) HealthBarWidget->SetHiddenInGame(true); }),
			1.f,
			false);
	}

	const float NewHealth = Data.NewValue;
	const float MaxHealth = (ASCAttributes ? ASCAttributes->GetMaxHealthBase() : 1.f);

	// 체력바 갱신 (EnemyBase의 UpdateHealthBarValue 로직과 동일, )
	UpdateHealthBarValue(NewHealth, MaxHealth);

	if (AWaveGameMode* WaveGM = Cast<AWaveGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		WaveGM->RuneUpdateHUD(NewHealth, MaxHealth);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game Mode Doesnt Match"));
	}

	// 표시/페이드 처리 (EnemyBase의 위젯 표시 로직 참조, )
	if (!bShouldKeepHealthbarOn && HealthBarWidget)
	{
		HealthBarWidget->SetHiddenInGame(false);

		if (!HealthBarWidget->bHiddenInGame)
		{
			if (UEnemyHealthBarWidget *const Widget = Cast<UEnemyHealthBarWidget>(HealthBarWidget->GetUserWidgetObject()))
			{
				Widget->PlayFadeAnimation();
			}
		}
	}

	// 체력이 0 이하면 코어 파괴 처리
	if (NewHealth <= 0.f)
	{
		HandleCoreDestroyed();
	}
}

void ASOWCharacterCoreRune::UpdateHealthBarValue(float NewHealth, float MaxHealth)
{
	if (!HealthBarWidget)
		return;

	// EnemyBase의 퍼센트 세터 방식 재사용 ()
	if (UEnemyHealthBarWidget *const Widget = Cast<UEnemyHealthBarWidget>(HealthBarWidget->GetUserWidgetObject()))
	{
		const float SafeDenom = (MaxHealth <= KINDA_SMALL_NUMBER) ? 1.f : MaxHealth;
		Widget->SetHealthBarPercent(NewHealth / SafeDenom);
	}
}

void ASOWCharacterCoreRune::HandleCoreDestroyed()
{
	// 외부(GameMode/Subsystem/UI)로 신호 전파
	OnCoreDestroyed.Broadcast();

	if (AWaveGameMode* WaveGM = Cast<AWaveGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		WaveGM->RuneDestroyed();
	}

	// 여기서 게임 오버 처리/이펙트/사운드 등을 호출하거나,
	// 코어를 파괴하지 않고 남겨둘 수도 있음. 필요하다면 Destroy() 호출:
	// K2_DestroyActor();
}
