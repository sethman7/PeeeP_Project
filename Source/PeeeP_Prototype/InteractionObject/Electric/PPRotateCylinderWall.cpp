// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Electric/PPRotateCylinderWall.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APPRotateCylinderWall::APPRotateCylinderWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder"));
	SetRootComponent(Cylinder);

}

// Called when the game starts or when spawned
void APPRotateCylinderWall::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APPRotateCylinderWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPRotateCylinderWall::Charge()
{
	UE_LOG(LogTemp, Log, TEXT("Cylinder!"));
}


