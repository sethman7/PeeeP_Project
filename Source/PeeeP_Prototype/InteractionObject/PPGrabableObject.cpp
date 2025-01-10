// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionObject/PPGrabableObject.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
APPGrabableObject::APPGrabableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionProfileName(TEXT("GrabObjectProfile"));

}

// Called when the game starts or when spawned
void APPGrabableObject::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APPGrabableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

