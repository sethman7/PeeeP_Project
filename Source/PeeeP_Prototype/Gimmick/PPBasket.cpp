// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPBasket.h"
#include "Containers/PPPoolObject.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APPBasket::APPBasket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionObjectType(ECC_GameTraceChannel8);
	//Mesh->SetMassScale(,10000.0f)
}

// Called when the game starts or when spawned
void APPBasket::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPBasket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPBasket::SetObjectPool(UPPPoolObject* InObjedtPool)
{
	ObjectPool = InObjedtPool;
}

void APPBasket::ReturnSelf()
{
	if (ObjectPool != nullptr)
	{
		ObjectPool->ReturnObject(this);
	}
	this->SetActive(false);
}

void APPBasket::SetActive(bool IsActive)
{
	this->SetActorHiddenInGame(!IsActive);
}

