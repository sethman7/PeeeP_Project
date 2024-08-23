// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PPElectricObjectInterface.h"
#include "PP_ElectricMovingPlatformTest.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APP_ElectricMovingPlatformTest : public AActor, public IPPElectricObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APP_ElectricMovingPlatformTest();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInterpToMovementComponent> InterpToMovement;

	bool bIsCharged;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Charge() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
