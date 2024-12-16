// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPElevatorDoorTrigger.generated.h"

enum class EDoorState
{
	Open,
	Close,
	Stay
};

UCLASS()
class PEEEP_PROTOTYPE_API APPElevatorDoorTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPElevatorDoorTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere, Category = Door)
	TObjectPtr<AActor> LeftDoor;

	UPROPERTY(EditAnywhere, Category = Door)
	TObjectPtr<AActor> RightDoor;

	float Duration;
	float ElapsedTime;

	UPROPERTY(EditAnywhere, Category = Door)
	TObjectPtr<class UBoxComponent> Trigger;

	FVector LeftDoorStartPos;
	FVector RightDoorStartPos;

	EDoorState DoorState;

	float DoorSpeed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
