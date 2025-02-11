// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPPoolableObjectDestroyerVolume.h"
#include "Components/BoxComponent.h"
#include "Interface/PPPoolableInterface.h"


// Sets default values
APPPoolableObjectDestroyerVolume::APPPoolableObjectDestroyerVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	RootComponent = Trigger;

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &APPPoolableObjectDestroyerVolume::OnTriggered);
	Trigger->SetCollisionProfileName(TEXT("PoolingObjectDestroy"));
}

// Called when the game starts or when spawned
void APPPoolableObjectDestroyerVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void APPPoolableObjectDestroyerVolume::OnTriggered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPPPoolableInterface* PoolingObject = Cast<IPPPoolableInterface>(OtherActor);
	if (PoolingObject)
	{
		PoolingObject->ReturnSelf();
	}
}

// Called every frame
void APPPoolableObjectDestroyerVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

