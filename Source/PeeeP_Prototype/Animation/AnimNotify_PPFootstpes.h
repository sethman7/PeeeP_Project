// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PPFootstpes.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UAnimNotify_PPFootstpes : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_PPFootstpes();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
	TObjectPtr<class USoundBase> FootstepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
	float VolumeMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
	float PitchMultiplier;
};
