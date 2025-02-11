// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PPRun.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UAnimNotify_PPRun : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_PPRun();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr <class UNiagaraSystem> RunningEffect;

	UPROPERTY(EditAnywhere)
	float EffectPlayPosition;
};
