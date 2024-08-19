// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/PPSpawnerButton.h"
#include "Gimmick/PPItemSpawner.h"


// Sets default values
APPSpawnerButton::APPSpawnerButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
}


// Called when the game starts or when spawned
void APPSpawnerButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPSpawnerButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APPSpawnerButton::Interact()
{
	APPItemSpawner* SpawnerClass = Cast<APPItemSpawner>(ItemSpawner);
	SpawnerClass->SpawnItem();
}