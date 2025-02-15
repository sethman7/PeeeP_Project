// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPAutoSpawner.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPAutoSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPAutoSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnPoolingActor();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> SpawnActorClass;

	UPROPERTY()
	TObjectPtr<class UPPPoolObject> PoolObject;

	FTimerHandle AutoSpawnTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
