// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PPGrabAnimInstance.h"
#include "Character/PPCharacterPlayer.h"
#include "Parts/PartsComponent/PPGrabParts.h"

UPPGrabAnimInstance::UPPGrabAnimInstance()
{
	bIsGrabbing = false;
	
}

void UPPGrabAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	APPCharacterPlayer* TempPlayer = Cast<APPCharacterPlayer>(Owner);
	if (nullptr != TempPlayer)
	{
		CharacterPlayer = TempPlayer;
	}
}

void UPPGrabAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(nullptr != CharacterPlayer)
	{
		UPPGrabParts* GrabParts = Cast<UPPGrabParts>(CharacterPlayer->GetParts());
		if (nullptr != GrabParts)
		{
			bIsGrabbing = GrabParts->GetIsGrabbed();
		}
	}
}
