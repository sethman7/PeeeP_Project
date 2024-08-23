// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PPInteractableObjectInterface.h"
#include "PPSpawnerButton.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPSpawnerButton : public AActor , public IPPInteractableObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPSpawnerButton();
	void Interact() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSpawner")
	TObjectPtr<class APPItemSpawner> ItemSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> Body;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
