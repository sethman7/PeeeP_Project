// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PPElectricObjectInterface.h"
#include "PP_ElectricMovingPlatform.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APP_ElectricMovingPlatform : public AActor, public IPPElectricObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APP_ElectricMovingPlatform();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInterpToMovementComponent> InterpToMovement;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterialInterface> NewMaterial;

	bool bIsCharged;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Charge() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
