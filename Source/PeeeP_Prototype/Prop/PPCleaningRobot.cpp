// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/PPCleaningRobot.h"

// Sets default values
APPCleaningRobot::APPCleaningRobot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APPCleaningRobot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPCleaningRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPCleaningRobot::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Robot Hit!"));
}

