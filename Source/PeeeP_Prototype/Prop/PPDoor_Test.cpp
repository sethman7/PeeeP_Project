// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/PPDoor_Test.h"

// Sets default values
APPDoor_Test::APPDoor_Test()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	trigger = false;
	Switch = false;
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
}

void APPDoor_Test::SwitchOnOff()
{
	Switch = true;
	if (!trigger)
	{
		//open 
		trigger = true;
	}
	else
	{
		//close
		trigger = false;
	}
}

void APPDoor_Test::MoveDoor(float value)
{
	if (GetActorLocation().Z >= Z_Loc + 300.0f && trigger == true) { return; }

	if (GetActorLocation().Z < Z_Loc)
	{
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, Z_Loc));
		Switch = false;
		return;
	}

	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + value));
}

// Called when the game starts or when spawned
void APPDoor_Test::BeginPlay()
{
	Super::BeginPlay();
	Z_Loc = GetActorLocation().Z;
}

// Called every frame
void APPDoor_Test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Switch)
	{
		if (trigger)
		{
			MoveDoor(3.0);
		}
		else
		{
			MoveDoor(-3.0);
		}
	}
}