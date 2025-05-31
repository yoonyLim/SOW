// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SOWCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
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

#include "UI/PlayerHUD.h"

// Sets default values
ASOWCharacterPlayer::ASOWCharacterPlayer()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
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
}

// Called when the game starts or when spawned
void ASOWCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hello World!"));

	MyHUDWidgetClass = LoadClass<UPlayerHUD>(nullptr, TEXT("/Game/01Blueprints/UI/Player/WB_HUD.WB_HUD_C"));

	if (MyHUDWidgetClass)
	{
		MyHUD = Cast<UPlayerHUD>(CreateWidget(GetWorld(), MyHUDWidgetClass));

		if (MyHUD)
		{
			MyHUD->AddToViewport();
			checkf(AbilitySystemComponent, TEXT("ASC is not valid in HUD"));

			MyHUD->Init(AbilitySystemComponent);
			MyHUD->SetVisibility(ESlateVisibility::Visible);
			UE_LOG(LogTemp, Warning, TEXT("HUD : Create HUD"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HUD : Fail to create HUD"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD : Can't find UHUDWidget class"));
		return;
	}
}


void ASOWCharacterPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
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
	UE_LOG(LogTemp, Log, TEXT("Roll triggered"));
}

void ASOWCharacterPlayer::UseSkill(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Roll triggered"));
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

		// Rolling
		EnhancedInputComponent->BindAction(UseSkillAction, ETriggerEvent::Triggered, this, &ASOWCharacterPlayer::UseSkill);

		EnhancedInputComponent->BindAction(InstallTurretAction, ETriggerEvent::Triggered, this, &ASOWCharacterPlayer::InstallTurret);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
