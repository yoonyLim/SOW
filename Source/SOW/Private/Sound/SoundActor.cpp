// Fill out your copyright notice in the Description page of Project Settings.


#include "Sound/SoundActor.h"
#include "Components/AudioComponent.h"


// Sets default values
ASoundActor::ASoundActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BGMComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMComponent"));
	BGMComponent->bAutoActivate = false;
	RootComponent = BGMComponent;
}

// Called when the game starts or when spawned
void ASoundActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASoundActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

