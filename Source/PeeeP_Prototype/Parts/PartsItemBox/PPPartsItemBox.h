// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPPartsItemBox.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPPartsItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPPartsItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> ItemBoxMesh;

	//Material
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterialInterface> ItemBoxMaterial;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> TriggerBox;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundBase> EarnSound;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Parts")
	FName PartsClassName;
private:
	UPROPERTY()
	TObjectPtr<class UPPInventoryComponent> InventoryComponent;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
