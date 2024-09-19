// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/PPElectricObjectInterface.h"
#include "PPGear.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPGear : public AActor, public IPPElectricObjectInterface
{
	GENERATED_BODY()

	public:
	// Sets default values for this actor's properties
	APPGear();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> BoxComponent;

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ApplyKnockback(UCharacterMovementComponent* CharacterMovementComponent, FVector direction, float strength, bool bVelocityChange = true);

	void Charge() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool bIsCharged;

};