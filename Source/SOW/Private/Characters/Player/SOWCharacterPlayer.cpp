// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SOWCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "SOWGameplayTags.h"

#include "Core/SOWPlayerController.h"

#include "AbilitySystem/Ability/SOWPlayerGameplayAbility.h"

#include "AbilitySystem/SOWAbilitySystemComponent.h"
#include "AbilitySystem/SOWAttributeSet.h"
#include "GameplayAbilities/Public/GameplayEffect.h"

#include "UI/PlayerHUD.h"
#include "UI/SkillSelectWidget.h"

// Sets default values
ASOWCharacterPlayer::ASOWCharacterPlayer()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	CharacterType = ESOWCharacterType::Player;
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1200.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	CharacterType = ESOWCharacterType::Player; // for player type recognition - by PGH

	/*static ConstructorHelpers::FClassFinder<UPlayerHUD> HUDClassFinder(TEXT("/Game/01Blueprints/UI/Player/WB_HUD"));
	if (HUDClassFinder.Succeeded())
	{
		MyHUDWidgetClass = HUDClassFinder.Class;
	}*/

	/* Turret Install Material */
	InstallationRangeDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("InstallationRangeDecal"));
	InstallationRangeDecal->SetupAttachment(RootComponent);
	InstallationRangeDecal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	InstallationRangeDecal->DecalSize = FVector(300.f, 300.f, 300.f);

	/*static ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalMat(TEXT("/Game/03Materials/M_Range_Decal.M_Range_Decal"));
	if (DecalMat.Succeeded())
	{
		InstallationRangeDecal->SetDecalMaterial(DecalMat.Object);
	}

	InstallationRangeDecal->SetVisibility(false);*/
}

// Called when the game starts or when spawned
void ASOWCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hello World!"));

	if (MyHUDWidgetClass)
	{
		MyHUD = CreateWidget<UPlayerHUD>(GetWorld(), MyHUDWidgetClass);
		MyHUD->AddToViewport();
		MyHUD->Init(AbilitySystemComponent);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HUDClass is NULL."));
	}
}

void ASOWCharacterPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && bCanMove)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASOWCharacterPlayer::Roll(const FInputActionValue& Value)
{

}

void ASOWCharacterPlayer::UseSkill(const FInputActionValue& Value)
{
	AbilitySystemComponent->OnAbilityInputPressed(CurrentSkillTag);
}

void ASOWCharacterPlayer::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASOWCharacterPlayer::InstallTurret(const FInputActionValue& Value)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	ASOWPlayerController* SOWPC = Cast<ASOWPlayerController>(PC);
	if (SOWPC)
	{
		SOWPC->ConfirmTurretPlacement();
		UE_LOG(LogTemp, Warning, TEXT(""));
	}
}

// Called to bind functionality to input
void ASOWCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASOWCharacterPlayer::Move);

		// Rolling
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &ASOWCharacterPlayer::Roll);

		EnhancedInputComponent->BindAction(UseSkillAction, ETriggerEvent::Triggered, this, &ASOWCharacterPlayer::UseSkill);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASOWCharacterPlayer::StartSprint);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASOWCharacterPlayer::StopSprint);

		EnhancedInputComponent->BindAction(SkillSelectAction, ETriggerEvent::Triggered, this, &ASOWCharacterPlayer::SelectSkill);

		EnhancedInputComponent->BindAction(InstallTurretAction, ETriggerEvent::Triggered, this, &ASOWCharacterPlayer::InstallTurret);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASOWCharacterPlayer::ShowInstallationRange(bool bShow)
{
	if (bShow)
	{
		InstallationRangeDecal->SetVisibility(true);
	}
	else
	{
		InstallationRangeDecal->SetVisibility(false);
	}
}

void ASOWCharacterPlayer::BindAttributeToCharacter()
{
	const USOWAttributeSet* AttributeSetRef = Cast<USOWAttributeSet>(AbilitySystemComponent->GetAttributeSet(USOWAttributeSet::StaticClass()));

	if (AbilitySystemComponent)
	{
		StaminaChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			USOWAttributeSet::GetCurrentStaminaAttribute()
		).AddUObject(this, &ASOWCharacterPlayer::CanApplyStaminaRegen);
	}

	if (!AttributeSetRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD : Fail to bind AttributeSet"));
	}
}

void ASOWCharacterPlayer::UnBindAttributeToCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("Clear"));

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			USOWAttributeSet::GetCurrentStaminaAttribute()
		).Remove(StaminaChangedHandle);
	}
}

void ASOWCharacterPlayer::StartStaminaRegen()
{
	const USOWAttributeSet* AttributeSetRef = Cast<USOWAttributeSet>(AbilitySystemComponent->GetAttributeSet(USOWAttributeSet::StaticClass()));

	float MaxStamina = AttributeSetRef->GetMaxStaminaBase();

	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(StaminaRegenEffect, 1.f, Context);

	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(
			FGameplayTag::RequestGameplayTag("Data.ModValue"),
			(MaxStamina * 5 / 1000)
		);
		hStaminaRegenEffect = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void ASOWCharacterPlayer::CanApplyStaminaRegen(const FOnAttributeChangeData& Data)
{
	const USOWAttributeSet* AttributeSetRef = Cast<USOWAttributeSet>(AbilitySystemComponent->GetAttributeSet(USOWAttributeSet::StaticClass()));

	float MaxStamina = AttributeSetRef->GetMaxStaminaBase();

	if (MaxStamina == Data.NewValue)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f : MaxStamina, %f : CurrentStamina"), MaxStamina, Data.NewValue);

		if (AbilitySystemComponent && hStaminaRegenEffect.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Success to remove"));

			AbilitySystemComponent->RemoveActiveGameplayEffect(hStaminaRegenEffect);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Fail to remove"));
		}

		UnBindAttributeToCharacter();
	}
}

void ASOWCharacterPlayer::StartSprint(const FInputActionValue& Value)
{
	if (AbilitySystemComponent && hStaminaRegenEffect.IsValid())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(hStaminaRegenEffect);
	}

	UnBindAttributeToCharacter();

	GetCharacterMovement()->MaxWalkSpeed = 700.f;

	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(SprintCostEffect, 1.f, Context);

	if (SpecHandle.IsValid())
	{
		hSprintCostEffect = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void ASOWCharacterPlayer::StopSprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	if (AbilitySystemComponent && hSprintCostEffect.IsValid())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(hSprintCostEffect);
	}

	BindAttributeToCharacter();
	StartStaminaRegen();
}

void ASOWCharacterPlayer::SelectSkill(const FInputActionValue& Value)
{
	if (MyHUD->SkillSelectWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		MyHUD->SkillSelectWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		MyHUD->SkillSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}