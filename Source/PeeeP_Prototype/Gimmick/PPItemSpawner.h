// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPItemSpawner.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPItemSpawner();
	void SpawnItem();
	bool IsOpen;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<class UBlueprint> Item;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
