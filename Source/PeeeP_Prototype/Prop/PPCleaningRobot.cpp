// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/PPCleaningRobot.h"
#include "InteractionObject/PPGrabableObject.h"

// Sets default values
APPCleaningRobot::APPCleaningRobot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetCollisionProfileName(TEXT("MovableObjectProfile"));

	ElectricLossRate = -1.0f;
	KnockbackStrength = 1000.0f;
	//Body->OnComponentHit.AddDynamic(this, &APPCleaningRobot::NotifyHit);
	IsGrab = false;
}

// Called when the game starts or when spawned
void APPCleaningRobot::BeginPlay()
{
	Super::BeginPlay();
	
}

void APPCleaningRobot::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<APPGrabableObject>(Other) && !IsGrab)
	{
		IsGrab = true;
	}
}


// Called every frame
void APPCleaningRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


