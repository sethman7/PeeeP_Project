// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_GrabHitCheck.h"
#include "Interface/PPAnimationGrabInterface.h"

void UAnimNotify_GrabHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	Super::Notify(MeshComp, Animation, EventReference);
	if (!MeshComp) return;
	IPPAnimationGrabInterface* AttackPawn = Cast<IPPAnimationGrabInterface>(MeshComp->GetOwner());
	if (AttackPawn)
	{
		AttackPawn->GrabHitCheck();
	}

}
