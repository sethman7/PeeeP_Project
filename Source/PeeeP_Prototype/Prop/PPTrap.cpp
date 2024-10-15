// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/PPTrap.h"

// Sets default values
APPTrap::APPTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
}

// Called when the game starts or when spawned
void APPTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FRotator Rotator = FRotator(0.0f, DeltaTime * Rotation, 0.0f);
	//AddActorLocalRotation(Rotator);
}

