// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PPElectricObjectInterface.h"
#include "PPRotateCylinderWall.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPRotateCylinderWall : public AActor, public IPPElectricObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPRotateCylinderWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Mesh
	UPROPERTY(EditAnywhere, Category = Cylinder)
	TObjectPtr<class UStaticMeshComponent> Cylinder;

protected:

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Charge();

};
