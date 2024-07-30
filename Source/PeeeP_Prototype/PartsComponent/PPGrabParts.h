// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PartsComponent/PPPartsBase.h"
#include "PPGrabParts.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPGrabParts : public UPPPartsBase
{
	GENERATED_BODY()
	

public:
	UPPGrabParts();

	

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupParts() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UPPGrabPartsData> GrabPartsData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPhysicsHandleComponent> GrabHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AActor> Owner;

	void GrabInteraction();
	void GrabRelease();
};
