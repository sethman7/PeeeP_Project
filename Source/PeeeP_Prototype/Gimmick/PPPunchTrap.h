// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPPunchTrap.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPPunchTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPPunchTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> DetectStartPosition;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> JabMontage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

//private:
//	UFUNCTION()
//	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
