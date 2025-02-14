// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PPSavepointInterface.h"
#include "PPElectricCharageStation.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPElectricCharageStation : public AActor, public IPPSavepointInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPElectricCharageStation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> TriggerFloorMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> TriggerBox;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterialInterface> ActiveMaterial;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> RootSceneComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAudioComponent> AudioComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundBase> SaveSound;

	bool bIsActivate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr <class UPPElectricDischargeComponent> ElectricDischargeComponent;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SaveGame(class APPCharacterPlayer* InPlayer) override;
};
