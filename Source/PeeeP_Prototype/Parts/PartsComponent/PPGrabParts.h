// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parts/PartsComponent/PPPartsBase.h"
#include "PPGrabParts.generated.h"

/*
 그랩 파츠

 */

UCLASS(meta = (BlueprintSpawnableComponent))
class PEEEP_PROTOTYPE_API UPPGrabParts : public UPPPartsBase
{
	GENERATED_BODY()
	

public:
	UPPGrabParts();
	
	virtual void SetUp() override;
	virtual void SetPartsActive(bool flag) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPhysicsHandleComponent> GrabHandle;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



	void Grab();
	void GrabRelease();


};
