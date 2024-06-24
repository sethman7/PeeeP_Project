// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPItemSpawner.h"

// Sets default values
APPItemSpawner::APPItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsOpen = true;
}

void APPItemSpawner::SpawnItem()
{
	if (!IsOpen) return;

	TSubclassOf<class UObject> block = (UClass*)Item->GeneratedClass;
	auto CurItem = GetWorld()->SpawnActor<AActor>(block, GetActorLocation(), GetActorRotation());
	IsOpen = false;

	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			IsOpen = true;
		}), 10, false);
}

// Called when the game starts or when spawned
void APPItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

