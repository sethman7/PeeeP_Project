// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parts/PartsComponent/PPPartsBase.h"
#include "PPGrabParts.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class PEEEP_PROTOTYPE_API UPPGrabParts : public UPPPartsBase
{
	GENERATED_BODY()
	

public:
	UPPGrabParts();
	
	virtual void BeginDestroy() override;

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPhysicsHandleComponent> GrabHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AActor> Owner;

	void GrabInteraction();
	void GrabRelease();
};
