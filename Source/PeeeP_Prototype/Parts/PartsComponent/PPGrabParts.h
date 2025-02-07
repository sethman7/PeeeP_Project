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
	
	void Grab(FHitResult& InHitResult);
	bool IsPressReleaseButton;
	class UPPGrabAnimInstance* GrabAnimInstance;
	virtual void OnComponentCreated() override;


protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPhysicsHandleComponent> GrabHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> GrabAnimMontage;

	

	void PlayGrabAnimation();
	void GrabRelease();
	void UpdateGrabbedObjectPosition();



};
