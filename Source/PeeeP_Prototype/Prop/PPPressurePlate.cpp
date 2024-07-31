// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/PPPressurePlate.h"
#include "Prop/PPDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
APPPressurePlate::APPPressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	//RootComponent = Body;
	CollisionBox->SetupAttachment(Body);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APPPressurePlate::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &APPPressurePlate::OnOverlapEnd);
}

// Called when the game starts or when spawned
void APPPressurePlate::BeginPlay()
{
	Super::BeginPlay();
	
}

void APPPressurePlate::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Door->OnInteract();
}

void APPPressurePlate::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Door->OnInteract();
}

// Called every frame
void APPPressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

