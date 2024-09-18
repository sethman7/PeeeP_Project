// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObject/Electric/PP_ElectricObjectTest.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
APP_ElectricObjectTest::APP_ElectricObjectTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionProfileName(TEXT("ElectricObjectProfile"));

}

// Called when the game starts or when spawned
void APP_ElectricObjectTest::BeginPlay()
{
	Super::BeginPlay();
	
}

void APP_ElectricObjectTest::Charge()
{
	UE_LOG(LogTemp, Log, TEXT("Interact Success!"));

	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(GetRootComponent());
	if (PrimitiveComponent)
	{
		PrimitiveComponent->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 500.f), true);
	}
}

// Called every frame
void APP_ElectricObjectTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

