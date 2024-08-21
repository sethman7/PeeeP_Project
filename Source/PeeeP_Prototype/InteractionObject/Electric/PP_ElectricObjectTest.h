// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PPElectricObjectInterface.h"
#include "PP_ElectricObjectTest.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APP_ElectricObjectTest : public AActor, public IPPElectricObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APP_ElectricObjectTest();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	//UPROPERTY(EditAnywhere)
	//TObjectPtr<class UInterpToMovementComponent> InterpToMovement;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Charge() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
