// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PPAutoSpawner.h"
#include "Containers/PPPoolObject.h"
#include "Interface/PPPoolableInterface.h"

// Sets default values
APPAutoSpawner::APPAutoSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PoolObject = CreateDefaultSubobject<UPPPoolObject>(TEXT("ObjectPool"));

	SpawnActorBlueprint = CreateDefaultSubobject<UBlueprint>(TEXT("SpawnActorObject"));

}

// Called when the game starts or when spawned
void APPAutoSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(SpawnActorBlueprint))
	{
		TSubclassOf<UObject> SpawnObjectClass = SpawnActorBlueprint->GeneratedClass;
		if (IsValid(SpawnObjectClass))
		{
			TSubclassOf<AActor> CastSpawnActorClass = TSubclassOf<AActor>(SpawnObjectClass);
			if (IsValid(CastSpawnActorClass))
			{
				SpawnActorClass = CastSpawnActorClass;
				PoolObject->SetPoolingObjectClass(SpawnActorClass);
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(AutoSpawnTimer, this, &APPAutoSpawner::SpawnPoolingActor, 5.0f, true);
}

void APPAutoSpawner::SpawnPoolingActor()
{
	AActor* SpawnedActor = PoolObject->GetPoolObject();
	IPPPoolableInterface* PoolableActor = CastChecked<IPPPoolableInterface>(SpawnedActor);

	SpawnedActor->SetActorLocation(this->GetActorLocation());
	PoolableActor->SetActive(true);
}

// Called every frame
void APPAutoSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

